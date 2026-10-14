"""Synthetic layouts only; no original executable bytes or target addresses."""
import contextlib
import hashlib
import io
import json
from pathlib import Path
import struct
import tempfile
import unittest

from baseline import IntakeError
from link_inventory import coverage, inspect_link_layout, main


def fixture(loads=None, sections=None):
    # Loads: offset, virtual address, file size, memory size, flags.
    loads = loads if loads is not None else [(512, 0x1000, 64, 96, 7)]
    # Sections: name, type, flags, virtual address, file offset, size.
    sections = sections if sections is not None else [
        ('.text', 1, 6, 0x1000, 512, 32),
        ('.data', 1, 3, 0x1020, 544, 32),
        ('.bss', 8, 3, 0x1040, 9000, 32),
    ]
    names = b'\0'
    offsets = []
    for name, *_ in sections:
        offsets.append(len(names))
        names += name.encode('ascii') + b'\0'
    shstr_name = len(names)
    names += b'.shstrtab\0'
    data = bytearray(4096)
    data[:16] = b'\x7fELF\x01\x01\x01' + bytes(9)
    struct.pack_into('<HHIIIIIHHHHHH', data, 16, 2, 8, 1, 0x1000, 52, 2048, 0,
                     52, 32, len(loads), 40, len(sections) + 2, len(sections) + 1)
    for index, (offset, address, file_size, memory_size, flags) in enumerate(loads):
        struct.pack_into('<8I', data, 52 + index * 32, 1, offset, address, address,
                         file_size, memory_size, flags, 1)
    for index, (nameoff, section) in enumerate(zip(offsets, sections), 1):
        _, stype, flags, address, offset, size = section
        struct.pack_into('<10I', data, 2048 + index * 40, nameoff, stype, flags,
                         address, offset, size, 0, 0, 1, 0)
    data[1500:1500 + len(names)] = names
    struct.pack_into('<10I', data, 2048 + (len(sections) + 1) * 40,
                     shstr_name, 3, 0, 0, 1500, len(names), 0, 0, 1, 0)
    return data


class LinkInventoryTests(unittest.TestCase):
    def test_file_memory_and_nobits_are_separate(self):
        report = inspect_link_layout(fixture())
        self.assertEqual(report['mapping_anomalies'], [])
        self.assertEqual([s['classification'] for s in report['sections'][1:4]],
                         ['allocated_executable', 'allocated_data', 'allocated_nobits'])
        self.assertIsNone(report['sections'][3]['file_range'])
        self.assertFalse(report['sections'][3]['has_file_payload'])
        load = report['loads'][0]
        self.assertEqual(load['compatible_file_section_coverage']['covered_bytes'], 64)
        self.assertEqual(load['nobits_zero_fill_coverage']['covered_bytes'], 32)
        self.assertEqual(load['declared_section_memory_coverage']['uncovered_bytes'], 0)

    def test_zero_sized_sections_do_not_claim_bytes_or_mappings(self):
        data = fixture(sections=[('.empty', 8, 3, 0x9990, 9000, 0)])
        report = inspect_link_layout(data)
        self.assertEqual(report['sections'][1]['load_mappings'], [])
        self.assertEqual(report['mapping_anomalies'], [])
        self.assertEqual(report['loads'][0]['declared_section_memory_coverage']['uncovered_bytes'], 96)

    def test_empty_loads_do_not_enlarge_memory_envelope(self):
        report = inspect_link_layout(fixture(loads=[(512, 0x1000, 64, 96, 7),
                                                    (0, 0, 0, 0, 4),
                                                    (0, 0xf0000000, 0, 0, 4)]))
        self.assertEqual(report['summary']['load_count'], 3)
        self.assertEqual(report['load_memory_envelope'], [0x1000, 0x1060])
        self.assertEqual(report['load_memory_envelope_coverage']['uncovered_bytes'], 0)
        self.assertEqual(report['load_memory_overlaps'], [])

    def test_file_offset_mismatch_does_not_create_file_coverage(self):
        report = inspect_link_layout(fixture(sections=[('.text', 1, 6, 0x1000, 516, 32)]))
        self.assertIn('file_offset_mismatch', report['sections'][1]['mapping_issues'])
        self.assertIn('section_not_fully_file_mapped', report['sections'][1]['mapping_issues'])
        self.assertEqual(report['loads'][0]['compatible_file_section_coverage']['covered_bytes'], 0)
        self.assertEqual(report['loads'][0]['declared_section_memory_coverage']['covered_bytes'], 32)

    def test_file_section_in_memory_tail_is_not_file_backed(self):
        report = inspect_link_layout(fixture(sections=[('.tail', 1, 3, 0x1040, 576, 16)]))
        self.assertIn('file_section_overlaps_zero_fill_memory', report['sections'][1]['mapping_issues'])
        self.assertEqual(report['sections'][1]['compatible_mapping_coverage']['covered_bytes'], 0)

    def test_nobits_intersecting_file_bytes_is_reported(self):
        report = inspect_link_layout(fixture(sections=[('.bss', 8, 3, 0x1030, 9000, 32)]))
        section = report['sections'][1]
        self.assertIn('nobits_overlaps_file_backed_memory', section['mapping_issues'])
        self.assertEqual(section['compatible_mapping_coverage']['covered_bytes'], 16)
        self.assertEqual(section['compatible_mapping_coverage']['gaps'], [[0x1030, 0x1040]])

    def test_crossing_two_consistent_loads_is_not_single_load_mapping(self):
        report = inspect_link_layout(fixture(loads=[(512, 0x1000, 32, 32, 5), (544, 0x1020, 32, 32, 5)],
                                             sections=[('.text', 1, 6, 0x1000, 512, 64)]))
        self.assertEqual(report['sections'][1]['mapping_issues'], ['no_single_load_contains_section'])
        self.assertEqual(report['sections'][1]['compatible_mapping_coverage']['covered_bytes'], 64)
        self.assertEqual(report['load_memory_overlaps'], [])

    def test_missing_and_partial_loads(self):
        for address, expected in ((0x2000, 'no_intersecting_load'), (0x1050, 'no_single_load_contains_section')):
            report = inspect_link_layout(fixture(sections=[('.orphan', 8, 3, address, 9999, 32)]))
            with self.subTest(address=address):
                self.assertIn(expected, report['sections'][1]['mapping_issues'])
                self.assertIn('section_not_fully_zero_mapped', report['sections'][1]['mapping_issues'])

    def test_section_gaps_are_exact_and_clipped(self):
        report = inspect_link_layout(fixture(sections=[('.text', 1, 6, 0x1000, 512, 8),
                                                      ('.data', 1, 3, 0x1010, 528, 8)]))
        self.assertEqual(report['loads'][0]['compatible_file_section_coverage']['gaps'],
                         [[0x1008, 0x1010], [0x1018, 0x1040]])
        self.assertEqual(report['loads'][0]['nobits_zero_fill_coverage']['gaps'], [[0x1040, 0x1060]])

    def test_section_overlap_not_double_counted(self):
        report = inspect_link_layout(fixture(sections=[('.one', 1, 6, 0x1000, 512, 24),
                                                      ('.two', 1, 6, 0x1010, 528, 24)]))
        self.assertEqual(report['allocated_section_memory_overlaps'][0]['bytes'], 8)
        self.assertEqual(report['section_file_payload_overlaps'][0]['bytes'], 8)
        self.assertEqual(report['loads'][0]['compatible_file_section_coverage']['covered_bytes'], 40)

    def test_load_file_zero_overlap_and_multiple_mapping(self):
        report = inspect_link_layout(fixture(loads=[(512, 0x1000, 64, 96, 7), (1024, 0x1040, 16, 32, 6)]))
        overlap = report['load_memory_overlaps'][0]
        self.assertEqual(overlap['bytes'], 32)
        self.assertEqual([(p['first_storage'], p['second_storage'], p['bytes']) for p in overlap['storage_intersections']],
                         [('zero', 'file', 16), ('zero', 'zero', 16)])
        self.assertIn('multiple_containing_loads', report['sections'][3]['mapping_issues'])
        self.assertIn('nobits_overlaps_file_backed_memory', report['sections'][3]['mapping_issues'])

    def test_load_file_file_different_backing(self):
        report = inspect_link_layout(fixture(loads=[(512, 0x1000, 64, 64, 7), (1024, 0x1020, 32, 32, 6)]))
        piece = report['load_memory_overlaps'][0]['storage_intersections'][0]
        self.assertFalse(piece['same_file_mapping'])
        self.assertEqual(piece['bytes'], 32)

    def test_load_file_file_same_backing(self):
        report = inspect_link_layout(fixture(loads=[(512, 0x1000, 64, 64, 7), (544, 0x1020, 32, 32, 6)]))
        self.assertTrue(report['load_memory_overlaps'][0]['storage_intersections'][0]['same_file_mapping'])
        self.assertEqual(report['load_file_overlaps'][0]['bytes'], 32)

    def test_permission_conflicts(self):
        report = inspect_link_layout(fixture(loads=[(512, 0x1000, 32, 32, 5), (544, 0x1020, 32, 64, 4)],
                                             sections=[('.unusual', 1, 7, 0x1020, 544, 32)]))
        self.assertEqual(report['sections'][1]['mapping_issues'],
                         ['executable_section_in_nonexecutable_load', 'writable_section_in_nonwritable_load'])

    def test_nonallocated_metadata_and_executable_are_separate(self):
        report = inspect_link_layout(fixture(sections=[('.debug_info', 1, 0, 0, 700, 8),
                    ('.strings', 3, 0, 0, 708, 8), ('.overlay', 0x70000001, 4, 0, 716, 8),
                    ('.platform', 0x70000001, 0, 0, 724, 8), ('.DVP.ovlystrtab', 3, 1, 0, 732, 8)]))
        self.assertEqual([s['classification'] for s in report['sections'][1:6]],
                         ['nonallocated_debug_metadata', 'nonallocated_symbol_or_string_metadata',
                          'nonallocated_executable', 'nonallocated_platform_metadata', 'nonallocated_platform_metadata'])
        self.assertTrue(all(not s['load_mappings'] for s in report['sections']))
        self.assertEqual(report['allocated_section_memory_overlaps'], [])

    def test_coverage_empty_nested_and_adjacent_ranges(self):
        self.assertEqual(coverage((0, 8), [(0, 4), (2, 3), (4, 9)])['covered_bytes'], 8)
        self.assertEqual(coverage((0, 0), [(0, 8)])['gaps'], [])
        self.assertEqual(coverage((2, 5), [])['gaps'], [[2, 5]])

    def test_hash_and_deterministic_sanitized_output(self):
        data = fixture(sections=[('/private/name', 1, 0, 0, 700, 8)])
        report = inspect_link_layout(data, hashlib.sha256(data).hexdigest())
        self.assertTrue(report['hash_pinned'])
        self.assertNotIn('/private/name', json.dumps(report))
        self.assertEqual(report, inspect_link_layout(data, hashlib.sha256(data).hexdigest()))
        for wrong in ('x', 'a' * 64):
            with self.subTest(wrong=wrong), self.assertRaises(IntakeError):
                inspect_link_layout(data, wrong)

    def test_malformed_payload_is_rejected_but_nobits_eof_offset_allowed(self):
        with self.assertRaises(IntakeError):
            inspect_link_layout(fixture(sections=[('.bad', 1, 3, 0x1020, 4090, 32)]))
        inspect_link_layout(fixture(sections=[('.bss', 8, 3, 0x1040, 999999, 32)]))

    def test_cli_hash_check_no_overwrite_and_no_input_mutation(self):
        with tempfile.TemporaryDirectory() as tmp:
            source, report = Path(tmp) / 'input', Path(tmp) / 'output.json'
            data = fixture()
            source.write_bytes(data)
            args = [str(source), '--expected-sha256', hashlib.sha256(data).hexdigest(), '--output', str(report)]
            with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
                self.assertEqual(main([str(source), '--expected-sha256', 'a' * 64, '--output', str(report)]), 1)
                self.assertFalse(report.exists())
                self.assertEqual(main(args), 0)
                saved = report.read_bytes()
                self.assertEqual(main(args), 1)
            self.assertEqual(report.read_bytes(), saved)
            self.assertEqual(source.read_bytes(), data)


if __name__ == '__main__':
    unittest.main()
