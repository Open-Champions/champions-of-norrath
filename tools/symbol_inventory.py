#!/usr/bin/env python3
"""Inventory ELF32 symbols privately; symbol extents are not proven functions.

No disassembly, executable loading, inferred function sizes, or game assets.
Reports contain retained names and must remain private until individually reviewed.
"""
import argparse
from collections import Counter, defaultdict
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import sys

from baseline import IntakeError, inspect_elf, span


def inspect_symbols(data, expected_sha256=None):
    """Return deterministic metadata; reject malformed tables, classify extents.

    Numeric symbol types/bindings are retained, including processor extensions.
    Reserved section indexes are classified without assuming ordinary sections.
    Range checks are metadata checks, never a complete-function or ABI claim.
    """
    baseline = inspect_elf(data)
    if expected_sha256 is not None:
        if not re.fullmatch(r'[0-9a-f]{64}', expected_sha256):
            raise IntakeError('Expected SHA-256 must be 64 lowercase hexadecimal characters')
        if baseline['executable']['sha256'] != expected_sha256:
            raise IntakeError('Executable SHA-256 does not match the pinned input')
    sections = baseline['sections']
    symbols, tables = [], []
    for table in sections:
        if table['type'] not in (2, 11):
            continue
        if table['entry_size'] != 16 or table['size'] < 16 or table['size'] % 16:
            raise IntakeError('Symbol table has an invalid entry size or payload size')
        if not 0 < table['link'] < len(sections) or sections[table['link']]['type'] != 3:
            raise IntakeError('Symbol table does not link to a string table')
        strings = sections[table['link']]
        names = span(data, strings['offset'], strings['size'], 'Symbol string table')
        if not names or names[0] != 0 or names[-1] != 0:
            raise IntakeError('Symbol string table must begin and end with a null byte')
        count = table['size'] // 16
        if not 1 <= table['info'] <= count:
            raise IntakeError('Symbol table has an invalid local symbol boundary')
        tables.append(dict(section_index=table['index'], string_section_index=table['link'],
                           entry_count=count, first_nonlocal_index=table['info']))
        for index in range(count):
            record = struct.unpack_from('<IIIBBH', data, table['offset'] + index * 16)
            nameoff, value, size, info, other, section_index = record
            if index == 0 and any(record):
                raise IntakeError('Symbol table entry zero must be entirely null')
            if nameoff >= len(names):
                raise IntakeError('Symbol name offset exceeds its string table')
            end = names.find(b'\0', nameoff)
            if end < 0:
                raise IntakeError('Symbol name is unterminated')
            raw_name = names[nameoff:end]
            # Omit file/path strings, control characters and arbitrarily long names.
            name = raw_name.decode('ascii') if re.fullmatch(rb'[A-Za-z0-9_.$@]{1,1024}', raw_name) else None
            binding, stype = info >> 4, info & 15
            if stype == 4:  # STT_FILE names can reveal source filenames.
                name = None
            if (binding == 0) != (index < table['info']):
                raise IntakeError('Symbol binding violates the table local/nonlocal boundary')
            if section_index == 0xffff:
                raise IntakeError('Extended symbol section indexes are unsupported')
            if section_index == 0:
                definition = 'undefined'
            elif section_index == 0xfff1:
                definition = 'absolute'
            elif section_index == 0xfff2:
                definition = 'common'
            elif section_index >= 0xff00:
                definition = 'reserved'
            elif section_index >= len(sections):
                raise IntakeError('Symbol references a nonexistent section')
            else:
                definition = 'section'
            symbol = dict(table_section=table['index'], index=index, name=name,
                          name_sha256=hashlib.sha256(raw_name).hexdigest() if raw_name else None,
                          value=value, size=size, type=stype, binding=binding,
                          visibility=other & 3, other=other, section_index=section_index,
                          definition=definition)
            if stype == 2:
                status = definition
                if definition == 'section':
                    section = sections[section_index]
                    offset = value - section['address']
                    if value + size > 0x100000000:
                        status = 'address_overflow'
                    elif offset < 0 or offset > section['size'] or size > section['size'] - offset:
                        status = 'outside_section'
                    elif not size:
                        status = 'zero_size'
                    elif not section['flags'] & 4:
                        status = 'nonexecutable_section'
                    elif section['type'] in (0, 8):
                        status = 'not_file_backed'
                    else:
                        status = 'bounded_executable_extent'
                        symbol['file_offset'] = section['offset'] + offset
                        symbol['allocated'] = bool(section['flags'] & 2)
                        symbol['executable_load_mapping'] = any(
                            segment['type'] == 1 and segment['flags'] & 1
                            and segment['virtual_address'] <= value
                            and value + size <= segment['virtual_address'] + segment['file_size']
                            and symbol['file_offset'] == segment['offset'] + value - segment['virtual_address']
                            for segment in baseline['segments'])
                symbol['extent_status'] = status
            symbols.append(symbol)
    if not tables:
        raise IntakeError('No symbol table is present; no symbol inventory is available')

    # Work with unique section-specific extents so aliases cannot inflate coverage.
    extents = defaultdict(list)
    for symbol in symbols:
        if symbol.get('extent_status') == 'bounded_executable_extent':
            extents[(symbol['section_index'], symbol['value'], symbol['size'])].append(
                [symbol['table_section'], symbol['index']])
    aliases = [dict(section_index=key[0], address=key[1], size=key[2], symbols=refs)
               for key, refs in sorted(extents.items()) if len(refs) > 1]
    by_section = defaultdict(list)
    for key in sorted(extents):
        by_section[key[0]].append(key)
    overlaps, coverage = [], {}
    for section_index, keys in sorted(by_section.items()):
        cluster, cluster_end, covered, start = [], 0, 0, 0
        for key in keys:
            _, address, size = key
            if cluster and address >= cluster_end:
                covered += cluster_end - start
                if len(cluster) > 1:
                    overlaps.append(dict(section_index=section_index, extents=cluster))
                cluster = []
            if not cluster:
                start, cluster_end = address, address + size
            else:
                cluster_end = max(cluster_end, address + size)
            cluster.append(dict(address=address, size=size, symbols=extents[key]))
        if cluster:
            covered += cluster_end - start
            if len(cluster) > 1:
                overlaps.append(dict(section_index=section_index, extents=cluster))
        coverage[str(section_index)] = covered
    functions = [s for s in symbols if s['type'] == 2]
    section_summary = []
    for section in sections:
        if not section['flags'] & 4:
            continue
        section_functions = [s for s in functions if s['section_index'] == section['index']]
        section_summary.append(dict(section_index=section['index'], name=section['name'],
                                    section_bytes=section['size'], function_symbols=len(section_functions),
                                    unique_bounded_extents=len(by_section[section['index']]),
                                    exact_alias_groups=sum(a['section_index'] == section['index'] for a in aliases),
                                    overlapping_extent_groups=sum(o['section_index'] == section['index'] for o in overlaps),
                                    covered_union_bytes=coverage.get(str(section['index']), 0),
                                    load_mapped_function_symbols=sum(s.get('executable_load_mapping', False) for s in section_functions)))
    return dict(schema_version=1, status='private_symbol_metadata_inventory',
                executable=baseline['executable'], hash_pinned=expected_sha256 is not None,
                tables=tables, executable_sections=[s for s in sections if s['flags'] & 4],
                summary=dict(symbol_entries=len(symbols), function_symbols=len(functions),
                             undefined_symbols=sum(s['definition'] == 'undefined' for s in symbols),
                             undefined_nonnull_symbols=sum(s['definition'] == 'undefined' and s['index'] != 0 for s in symbols),
                             function_extent_status=dict(sorted(Counter(s['extent_status'] for s in functions).items())),
                             function_symbols_by_section=dict(sorted(Counter(str(s['section_index']) for s in functions).items())),
                             unique_bounded_function_extents=len(extents), exact_alias_groups=len(aliases),
                             overlapping_extent_groups=len(overlaps), covered_bytes_by_section=coverage,
                             executable_sections=section_summary),
                symbols=symbols, aliases=aliases, overlaps=overlaps,
                limitations=['Symbol metadata does not establish complete function boundaries or behavior.',
                             'Undefined symbols include the mandatory null entry of each symbol table.',
                             'Exact aliases share section, address and size; overlaps group connected distinct extents.',
                             'Executable section flags do not establish CPU architecture or runtime reachability.',
                             'No game-wide denominator, compiler, ABI, retail pressing or runtime compatibility is established.'])


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('executable', type=Path)
    parser.add_argument('--expected-sha256', required=True)
    parser.add_argument('--output', required=True, type=Path, help='New private report; never overwritten')
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
        report = inspect_symbols(data, args.expected_sha256)
        with args.output.open('x', encoding='utf-8') as output:
            json.dump(report, output, sort_keys=True, indent=2)
            output.write('\n')
        print(json.dumps(report['summary'], sort_keys=True))
    except (OSError, ValueError) as error:
        print(f'Inventory failed: {error}', file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
