"""Synthetic ELF and symbol metadata only; no game bytes or identifiers."""
import contextlib
import hashlib
import io
import json
from pathlib import Path
import struct
import tempfile
import unittest

from baseline import IntakeError
from symbol_inventory import inspect_symbols, main


def fixture(records=None):
    # name offset, value, size, info, other, section index
    records = records if records is not None else [(1, 0x1000, 8, 0x12, 0, 1)]
    records = [(0, 0, 0, 0, 0, 0)] + records
    data = bytearray(2048)
    data[:16] = b'\x7fELF\x01\x01\x01' + bytes(9)
    struct.pack_into('<HHIIIIIHHHHHH', data, 16, 2, 8, 1, 0x1000, 52, 1024,
                     0, 52, 32, 1, 40, 6, 5)
    struct.pack_into('<8I', data, 52, 1, 128, 0x1000, 0x1000, 64, 96, 5, 16)
    names = b'\0.text\0.symtab\0.strtab\0.bss\0.shstrtab\0'
    data[900:900 + len(names)] = names
    string_names = b'\0alpha\0beta\0../secret\0gamma\0'
    data[800:800 + len(string_names)] = string_names
    structs = [
        (1, 1, 6, 0x1000, 128, 64, 0, 0, 4, 0),
        (7, 2, 0, 0, 256, len(records) * 16, 3, 1, 4, 16),
        (15, 3, 0, 0, 800, len(string_names), 0, 0, 1, 0),
        (23, 8, 3, 0x1040, 4096, 32, 0, 0, 4, 0),
        (28, 3, 0, 0, 900, len(names), 0, 0, 1, 0),
    ]
    for i, record in enumerate(structs, 1):
        struct.pack_into('<10I', data, 1024 + i * 40, *record)
    for i, record in enumerate(records):
        struct.pack_into('<IIIBBH', data, 256 + i * 16, *record)
    return data


def set_section(data, index, field, value):
    struct.pack_into('<I', data, 1024 + index * 40 + field * 4, value)


class SymbolInventoryTests(unittest.TestCase):
    def test_one_function_and_hash(self):
        data = fixture()
        report = inspect_symbols(data, hashlib.sha256(data).hexdigest())
        symbol = report['symbols'][1]
        self.assertTrue(report['hash_pinned'])
        self.assertEqual(symbol['name'], 'alpha')
        self.assertEqual(symbol['extent_status'], 'bounded_executable_extent')
        self.assertEqual(symbol['file_offset'], 128)
        self.assertTrue(symbol['executable_load_mapping'])
        self.assertEqual(report['summary']['covered_bytes_by_section'], {'1': 8})
        for wrong in ('a' * 64, 'bad'):
            with self.subTest(wrong=wrong), self.assertRaises(IntakeError):
                inspect_symbols(data, wrong)

    def test_missing_symbol_table(self):
        data = fixture()
        set_section(data, 2, 1, 1)
        with self.assertRaises(IntakeError):
            inspect_symbols(data)

    def test_table_bounds_links_and_sizes(self):
        for index, field, value in ((2, 4, 2040), (2, 5, 31), (2, 5, 0),
                                    (2, 9, 8), (2, 6, 6), (2, 6, 1),
                                    (2, 7, 0), (2, 7, 3), (3, 5, 0), (3, 4, 2040)):
            data = fixture()
            set_section(data, index, field, value)
            with self.subTest(index=index, field=field, value=value), self.assertRaises(IntakeError):
                inspect_symbols(data)

    def test_bad_string_table_and_offsets(self):
        for offset in (800, 827):
            data = fixture()
            # Last byte is obtained from the section's recorded size.
            if offset != 800:
                offset = 800 + struct.unpack_from('<I', data, 1024 + 3 * 40 + 20)[0] - 1
            data[offset] = 65
            with self.subTest(offset=offset), self.assertRaises(IntakeError):
                inspect_symbols(data)
        with self.assertRaises(IntakeError):
            inspect_symbols(fixture([(999, 0x1000, 8, 0x12, 0, 1)]))

    def test_null_entry_and_binding_partition(self):
        data = fixture()
        data[256] = 1
        with self.assertRaises(IntakeError):
            inspect_symbols(data)
        with self.assertRaises(IntakeError):
            inspect_symbols(fixture([(1, 0x1000, 8, 2, 0, 1)]))
        data = fixture()
        set_section(data, 2, 7, 2)
        with self.assertRaises(IntakeError):
            inspect_symbols(data)

    def test_definition_classes_and_unknown_types(self):
        records = [(1, 0, 0, 0x12, 0, section) for section in (0, 0xfff1, 0xfff2, 0xff01)]
        records.append((1, 0, 0, 0xd, 0, 0))
        # Put extension-type local symbol before all nonlocals.
        records = records[-1:] + records[:-1]
        data = fixture(records)
        set_section(data, 2, 7, 2)
        result = inspect_symbols(data)
        self.assertEqual([s['definition'] for s in result['symbols'][2:]],
                         ['undefined', 'absolute', 'common', 'reserved'])
        for section in (6, 0xffff):
            with self.subTest(section=section), self.assertRaises(IntakeError):
                inspect_symbols(fixture([(1, 0, 0, 0x12, 0, section)]))

    def test_extent_validation(self):
        cases = [(0x1000, 0, 1, 'zero_size'), (0x1040, 0, 1, 'zero_size'),
                 (0x1040, 4, 1, 'outside_section'), (0xffc, 8, 1, 'outside_section'),
                 (0xfffffffc, 8, 1, 'address_overflow'), (0x1040, 4, 4, 'nonexecutable_section')]
        for value, size, section, status in cases:
            report = inspect_symbols(fixture([(1, value, size, 0x12, 0, section)]))
            with self.subTest(status=status):
                self.assertEqual(report['symbols'][1]['extent_status'], status)
                self.assertNotIn('file_offset', report['symbols'][1])
        data = fixture([(1, 0x1040, 4, 0x12, 0, 4)])
        set_section(data, 4, 2, 7)
        self.assertEqual(inspect_symbols(data)['symbols'][1]['extent_status'], 'not_file_backed')

    def test_valid_section_without_load_mapping(self):
        data = fixture()
        set_section(data, 1, 4, 132)
        symbol = inspect_symbols(data)['symbols'][1]
        self.assertFalse(symbol['executable_load_mapping'])
        self.assertEqual(symbol['extent_status'], 'bounded_executable_extent')

    def test_alias_overlap_and_coverage_union(self):
        records = [(1, 0x1000, 8, 0x12, 0, 1), (7, 0x1000, 8, 0x12, 0, 1),
                   (1, 0x1004, 8, 0x12, 0, 1), (1, 0x100c, 4, 0x12, 0, 1)]
        report = inspect_symbols(fixture(records))
        self.assertEqual(report['summary']['unique_bounded_function_extents'], 3)
        self.assertEqual(report['summary']['exact_alias_groups'], 1)
        self.assertEqual(report['summary']['overlapping_extent_groups'], 1)
        self.assertEqual(len(report['overlaps'][0]['extents']), 2)
        self.assertEqual(report['summary']['covered_bytes_by_section'], {'1': 16})

    def test_no_cross_section_aliases(self):
        data = fixture([(1, 0x1000, 8, 0x12, 0, 1), (7, 0x1000, 8, 0x12, 0, 4)])
        set_section(data, 4, 1, 1)
        set_section(data, 4, 2, 6)
        set_section(data, 4, 3, 0x1000)
        set_section(data, 4, 4, 192)
        report = inspect_symbols(data)
        self.assertEqual(report['summary']['exact_alias_groups'], 0)
        self.assertEqual(report['summary']['covered_bytes_by_section'], {'1': 8, '4': 8})

    def test_paths_suppressed_and_output_deterministic(self):
        data = fixture([(12, 0x1000, 8, 0x12, 0, 1)])
        report = inspect_symbols(data)
        self.assertIsNone(report['symbols'][1]['name'])
        self.assertNotIn('secret', json.dumps(report))
        self.assertEqual(json.dumps(report, sort_keys=True), json.dumps(inspect_symbols(data), sort_keys=True))

    def test_cli_pinning_and_no_overwrite(self):
        with tempfile.TemporaryDirectory() as tmp:
            source, output = Path(tmp) / 'input', Path(tmp) / 'report.json'
            data = fixture()
            source.write_bytes(data)
            args = [str(source), '--expected-sha256', hashlib.sha256(data).hexdigest(), '--output', str(output)]
            with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
                self.assertEqual(main([str(source), '--expected-sha256', 'a' * 64, '--output', str(output)]), 1)
                self.assertFalse(output.exists())
                self.assertEqual(main(args), 0)
                saved = output.read_bytes()
                self.assertEqual(main(args), 1)
            self.assertEqual(output.read_bytes(), saved)
            self.assertEqual(source.read_bytes(), data)
            self.assertNotIn(tmp, output.read_text())


if __name__ == '__main__':
    unittest.main()
