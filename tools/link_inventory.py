#!/usr/bin/env python3
"""Inspect original ELF layout requirements without copying any payload bytes.

Ranges are half-open. Mapping diagnostics describe the ELF declarations, not an
emulated loader, linker script, reconstructed executable, or disposal policy.
"""
import argparse
from collections import Counter
import json
import os
from pathlib import Path
import re
import sys

from baseline import IntakeError, inspect_elf


def intersection(a, b):
    lo, hi = max(a[0], b[0]), min(a[1], b[1])
    return (lo, hi) if lo < hi else None


def coverage(bounds, ranges):
    """Union clipped ranges; retain precise gaps rather than summing overlaps."""
    merged = []
    for lo, hi in sorted(hit for part in ranges if (hit := intersection(bounds, part))):
        if merged and lo <= merged[-1][1]:
            merged[-1][1] = max(merged[-1][1], hi)
        else:
            merged.append([lo, hi])
    gaps, cursor = [], bounds[0]
    for lo, hi in merged:
        if cursor < lo:
            gaps.append([cursor, lo])
        cursor = hi
    if cursor < bounds[1]:
        gaps.append([cursor, bounds[1]])
    return dict(covered_bytes=sum(hi - lo for lo, hi in merged),
                uncovered_bytes=sum(hi - lo for lo, hi in gaps), covered_ranges=merged,
                gaps=gaps)


def overlapping_ranges(items):
    """Report pairwise nonempty overlaps; do not mistake adjacency for overlap."""
    result = []
    ordered = sorted(items, key=lambda item: (item[1][0], item[1][1], item[0]))
    active = []
    for index, bounds in ordered:
        if bounds[0] == bounds[1]:
            continue
        active = [(other, part) for other, part in active if part[1] > bounds[0]]
        for other, part in active:
            hit = intersection(part, bounds)
            if hit:
                result.append(dict(first=other, second=index, range=list(hit), bytes=hit[1] - hit[0]))
        active.append((index, bounds))
    return result


def classification(section):
    if section['type'] == 0:
        return 'null'
    if section['flags'] & 2:
        if section['type'] == 8:
            return 'allocated_nobits'
        return 'allocated_executable' if section['flags'] & 4 else 'allocated_data'
    if section['flags'] & 4:
        return 'nonallocated_executable'
    if (section['name'] or '').startswith('.DVP.'):
        return 'nonallocated_platform_metadata'
    if section['type'] in (2, 3, 11):
        return 'nonallocated_symbol_or_string_metadata'
    if (section['name'] or '').startswith(('.debug', '.mdebug', '.stab')):
        return 'nonallocated_debug_metadata'
    if section['type'] >= 0x60000000:
        return 'nonallocated_platform_metadata'
    return 'nonallocated_other'


def inspect_link_layout(data, expected_sha256=None):
    baseline = inspect_elf(data)
    if expected_sha256 is not None:
        if not re.fullmatch(r'[0-9a-f]{64}', expected_sha256):
            raise IntakeError('Expected SHA-256 must be 64 lowercase hexadecimal characters')
        if baseline['executable']['sha256'] != expected_sha256:
            raise IntakeError('Executable SHA-256 does not match the pinned input')
    loads = [s for s in baseline['segments'] if s['type'] == 1]
    sections, anomalies = [], []
    per_load = {s['index']: dict(declared_memory=[], mapped_file=[], mapped_zero=[]) for s in loads}
    for original in baseline['sections']:
        section = dict(original)
        section['classification'] = classification(section)
        allocated = bool(section['flags'] & 2) and section['type'] != 0
        nobits = section['type'] == 8
        section['has_file_payload'] = section['type'] not in (0, 8) and section['size'] > 0
        section['memory_range'] = [section['address'], section['address'] + section['size']] if allocated else None
        section['file_range'] = [section['offset'], section['offset'] + section['size']] if section['has_file_payload'] else None
        section['load_mappings'] = []
        issues = []
        if allocated and section['size']:
            bounds = tuple(section['memory_range'])
            valid_ranges = []
            memory_containers = []
            for load in loads:
                lo = load['virtual_address']
                memory = (lo, lo + load['memory_size'])
                file_memory = (lo, lo + load['file_size'])
                zero_memory = (file_memory[1], memory[1])
                hit = intersection(bounds, memory)
                if hit is None:
                    continue
                contains = memory[0] <= bounds[0] and bounds[1] <= memory[1]
                if contains:
                    memory_containers.append(load['index'])
                per_load[load['index']]['declared_memory'].append(hit)
                mapped = dict(segment_index=load['index'], memory_intersection=list(hit),
                              contains_whole_section=contains)
                permissions = []
                if section['flags'] & 4 and not load['flags'] & 1:
                    permissions.append('executable_section_in_nonexecutable_load')
                if section['flags'] & 1 and not load['flags'] & 2:
                    permissions.append('writable_section_in_nonwritable_load')
                mapped['permission_conflicts'] = permissions
                issues.extend(permissions)
                if nobits:
                    backed = intersection(bounds, file_memory)
                    zero = intersection(bounds, zero_memory)
                    mapped['overlap_with_file_backed_memory'] = list(backed) if backed else None
                    mapped['mapped_zero_range'] = list(zero) if zero else None
                    if backed:
                        issues.append('nobits_overlaps_file_backed_memory')
                    if zero:
                        valid_ranges.append(zero)
                        per_load[load['index']]['mapped_zero'].append(zero)
                else:
                    consistent = section['offset'] - section['address'] == load['offset'] - lo
                    file_hit = intersection(bounds, file_memory) if consistent else None
                    mapped['file_offset_consistent'] = consistent
                    mapped['mapped_file_memory_range'] = list(file_hit) if file_hit else None
                    if not consistent:
                        issues.append('file_offset_mismatch')
                    if intersection(bounds, zero_memory):
                        issues.append('file_section_overlaps_zero_fill_memory')
                    if file_hit:
                        valid_ranges.append(file_hit)
                        per_load[load['index']]['mapped_file'].append(file_hit)
                section['load_mappings'].append(mapped)
            section['compatible_mapping_coverage'] = coverage(bounds, valid_ranges)
            if not section['load_mappings']:
                issues.append('no_intersecting_load')
            elif not memory_containers:
                issues.append('no_single_load_contains_section')
            if section['compatible_mapping_coverage']['uncovered_bytes']:
                issues.append('section_not_fully_zero_mapped' if nobits else 'section_not_fully_file_mapped')
            if len(memory_containers) > 1:
                issues.append('multiple_containing_loads')
        section['mapping_issues'] = sorted(set(issues))
        if issues:
            anomalies.append(dict(section_index=section['index'], issues=section['mapping_issues']))
        sections.append(section)

    load_reports = []
    for load in loads:
        lo = load['virtual_address']
        file_end, memory_end = lo + load['file_size'], lo + load['memory_size']
        buckets = per_load[load['index']]
        record = dict(load)
        record.update(file_range=[load['offset'], load['offset'] + load['file_size']],
                      file_backed_memory_range=[lo, file_end], zero_fill_range=[file_end, memory_end],
                      memory_range=[lo, memory_end],
                      declared_section_memory_coverage=coverage((lo, memory_end), buckets['declared_memory']),
                      compatible_file_section_coverage=coverage((lo, file_end), buckets['mapped_file']),
                      nobits_zero_fill_coverage=coverage((file_end, memory_end), buckets['mapped_zero']))
        load_reports.append(record)

    memory_overlaps = overlapping_ranges([(s['index'], (s['virtual_address'], s['virtual_address'] + s['memory_size'])) for s in loads])
    lookup = {s['index']: s for s in loads}
    for overlap in memory_overlaps:
        a, b = lookup[overlap['first']], lookup[overlap['second']]
        pieces = []
        for kind_a, bounds_a in (('file', (a['virtual_address'], a['virtual_address'] + a['file_size'])),
                                 ('zero', (a['virtual_address'] + a['file_size'], a['virtual_address'] + a['memory_size']))):
            for kind_b, bounds_b in (('file', (b['virtual_address'], b['virtual_address'] + b['file_size'])),
                                     ('zero', (b['virtual_address'] + b['file_size'], b['virtual_address'] + b['memory_size']))):
                hit = intersection(bounds_a, bounds_b)
                if hit:
                    piece = dict(first_storage=kind_a, second_storage=kind_b, range=list(hit), bytes=hit[1] - hit[0])
                    if kind_a == kind_b == 'file':
                        piece['same_file_mapping'] = a['offset'] - a['virtual_address'] == b['offset'] - b['virtual_address']
                    pieces.append(piece)
        overlap['storage_intersections'] = pieces
    section_memory_overlaps = overlapping_ranges([(s['index'], tuple(s['memory_range'])) for s in sections if s['memory_range']])
    section_file_overlaps = overlapping_ranges([(s['index'], tuple(s['file_range'])) for s in sections if s['file_range']])
    categories = Counter(s['classification'] for s in sections)
    category_bytes = Counter()
    for section in sections:
        category_bytes[section['classification']] += section['size']
    # Empty declarations have no address coverage and cannot enlarge the envelope.
    # inspect_elf requires a file-backed executable entry, so this is nonempty.
    nonempty_loads = [s for s in loads if s['memory_size']]
    memory_envelope = (min(s['virtual_address'] for s in nonempty_loads),
                       max(s['virtual_address'] + s['memory_size'] for s in nonempty_loads))
    memory_coverage = coverage(memory_envelope, [(s['virtual_address'], s['virtual_address'] + s['memory_size']) for s in loads])
    file_coverage = coverage((0, len(data)), [(s['offset'], s['offset'] + s['file_size']) for s in loads])
    return dict(schema_version=1, status='original_layout_inventory_only', executable=baseline['executable'],
                hash_pinned=expected_sha256 is not None, ranges='half-open',
                summary=dict(section_count=len(sections), load_count=len(loads),
                             section_classifications=dict(sorted(categories.items())),
                             section_bytes_by_classification=dict(sorted(category_bytes.items())),
                             allocated_section_bytes=sum(s['size'] for s in sections if s['memory_range']),
                             load_memory_union_bytes=memory_coverage['covered_bytes'],
                             load_file_union_bytes=file_coverage['covered_bytes'],
                             sections_with_mapping_issues=len(anomalies), load_memory_overlap_pairs=len(memory_overlaps),
                             allocated_section_overlap_pairs=len(section_memory_overlaps), section_payload_overlap_pairs=len(section_file_overlaps)),
                sections=sections, loads=load_reports,
                load_memory_envelope=list(memory_envelope), load_memory_envelope_coverage=memory_coverage,
                file_load_coverage=file_coverage,
                other_program_headers=[s for s in baseline['segments'] if s['type'] != 1],
                mapping_anomalies=anomalies, load_memory_overlaps=memory_overlaps,
                load_file_overlaps=overlapping_ranges([(s['index'], (s['offset'], s['offset'] + s['file_size'])) for s in loads]),
                allocated_section_memory_overlaps=section_memory_overlaps, section_file_payload_overlaps=section_file_overlaps,
                limitations=['No payload bytes are copied, linked, reconstructed or executed.',
                             'Coverage gaps can be padding, headers, metadata or unidentified content; they are not missing-source measurements.',
                             'Nonallocated sections are not PT_LOAD runtime mappings; this does not authorize discarding platform or overlay metadata.',
                             'Section flags classify declarations, not CPU architecture, data ownership or actual loader behavior.',
                             'Overlapping file and zero-fill load ranges require loader/linker investigation; load order is not simulated.',
                             'This inventory does not establish a linker script, startup behavior, whole-program ABI or a playable build.'])


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('executable', type=Path)
    parser.add_argument('--expected-sha256', required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args(argv)
    try:
        with args.executable.open('rb') as source:
            before = os.fstat(source.fileno())
            if before.st_size > 128 * 1024 * 1024:
                raise IntakeError('Executable exceeds the 128 MiB intake limit')
            data = source.read()
            after = os.fstat(source.fileno())
        if (before.st_size, before.st_mtime_ns) != (after.st_size, after.st_mtime_ns) or len(data) != after.st_size:
            raise IntakeError('Executable changed during inventory')
        report = inspect_link_layout(data, args.expected_sha256)
        with args.output.open('x', encoding='utf-8') as output:
            json.dump(report, output, sort_keys=True, indent=2)
            output.write('\n')
        print(json.dumps(report['summary'], sort_keys=True))
    except (OSError, ValueError) as error:
        print(f'Link inventory failed: {error}', file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
