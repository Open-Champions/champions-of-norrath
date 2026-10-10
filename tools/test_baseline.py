"""Synthetic inputs only. No retail bytes or game-specific addresses."""
import contextlib
import hashlib
import io
import json
from pathlib import Path
import struct
import tempfile
import unittest

from baseline import IntakeError, boot_metadata, inspect_elf, main


def fixture(sections=False):
    data = bytearray(512)
    data[:16] = b'\x7fELF\x01\x01\x01' + bytes(9)
    struct.pack_into('<HHIIIIIHHHHHH', data, 16, 2, 8, 1, 0x1000,
                     52, 256 if sections else 0, 0, 52, 32, 1,
                     40 if sections else 0, 4 if sections else 0, 3 if sections else 0)
    struct.pack_into('<8I', data, 52, 1, 128, 0x1000, 0x1000, 16, 32, 5, 16)
    if sections:
        names = b'\0.text\0.bss\0.shstrtab\0'
        data[192:192+len(names)] = names
        struct.pack_into('<10I', data, 296, 1, 1, 6, 0x1000, 128, 16, 0, 0, 4, 0)
        # NOBITS section may have a logical file offset beyond EOF.
        struct.pack_into('<10I', data, 336, 7, 8, 3, 0x1010, 4096, 16, 0, 0, 4, 0)
        struct.pack_into('<10I', data, 376, 12, 3, 0, 0, 192, len(names), 0, 0, 1, 0)
    return data


class BaselineTests(unittest.TestCase):
    def test_sectionless_executable(self):
        data = fixture()
        result = inspect_elf(data)
        self.assertEqual(result['executable']['sha256'], hashlib.sha256(data).hexdigest())
        self.assertEqual(result['sections'], [])
        self.assertEqual(result['status'], 'structurally_validated_candidate')
        self.assertNotIn('functions', result)

    def test_sections_and_nobits(self):
        result = inspect_elf(fixture(True))
        self.assertEqual([s['name'] for s in result['sections']], [None, '.text', '.bss', '.shstrtab'])
        self.assertEqual(result['sections'][2]['offset'], 4096)

    def test_truncations(self):
        for length in (0, 16, 51, 60, 140, 400):
            with self.subTest(length=length), self.assertRaises(IntakeError):
                inspect_elf(fixture(True)[:length])

    def test_wrong_format(self):
        for offset, value in ((0, 0), (4, 2), (5, 2), (6, 0), (16, 1), (18, 62)):
            data = fixture()
            data[offset] = value
            with self.subTest(offset=offset), self.assertRaises(IntakeError):
                inspect_elf(data)

    def test_invalid_program_layout(self):
        # Header overlap, program table past EOF, file size > memory size,
        # wrapping virtual address, bad alignment, misaligned segment offset.
        for offset, value in ((28, 16), (28, 500), (68, 33), (60, 0xfffffff0), (80, 3), (56, 129)):
            data = fixture()
            struct.pack_into('<I', data, offset, value)
            with self.subTest(offset=offset, value=value), self.assertRaises(IntakeError):
                inspect_elf(data)

    def test_entry_must_be_backed_by_executable_bytes(self):
        for entry in (0, 0x1001, 0x1010, 0x1020):
            data = fixture()
            struct.pack_into('<I', data, 24, entry)
            with self.subTest(entry=entry), self.assertRaises(IntakeError):
                inspect_elf(data)
        data = fixture()
        struct.pack_into('<I', data, 76, 4)
        with self.assertRaises(IntakeError):
            inspect_elf(data)

    def test_invalid_sections(self):
        for offset, fmt, value in ((50, '<H', 4), (48, '<H', 0), (296, '<I', 999), (380, '<I', 1), (312, '<I', 500)):
            data = fixture(True)
            struct.pack_into(fmt, data, offset, value)
            with self.subTest(offset=offset), self.assertRaises(IntakeError):
                inspect_elf(data)

    def test_arbitrary_embedded_strings_are_not_reported(self):
        data = fixture(True)
        data[193:198] = b'/home'
        result = inspect_elf(data)
        self.assertIsNone(result['sections'][1]['name'])
        self.assertNotIn('/home', json.dumps(result))

    def test_boot_identity(self):
        result = boot_metadata('BOOT2 = cdrom0:\\TEST_000.00;1\nVER = 1.00\nVMODE = NTSC\n', 'TEST_000.00')
        self.assertEqual(result, {'boot_executable': 'TEST_000.00', 'ver': '1.00', 'vmode': 'NTSC'})

    def test_boot_mismatch_duplicates_and_untrusted_text(self):
        for text in ('BOOT2=cdrom0:\\OTHER;1', 'BOOT2=../private',
                     'BOOT2=cdrom0:\\TEST_000.00;1\nBOOT2=cdrom0:\\TEST_000.00;1',
                     'BOOT2=cdrom0:\\TEST_000.00;1\nVER=/private/path'):
            with self.subTest(text=text), self.assertRaises(IntakeError):
                boot_metadata(text, 'TEST_000.00')

    def test_cli_preserves_existing_output_and_input(self):
        with tempfile.TemporaryDirectory() as tmp:
            source = Path(tmp) / 'TEST_000.00'
            out = Path(tmp) / 'report.json'
            source.write_bytes(fixture())
            self.assertEqual(main([str(source), '--output', str(out)]), 0)
            original = out.read_bytes()
            with contextlib.redirect_stderr(io.StringIO()):
                self.assertEqual(main([str(source), '--output', str(out)]), 1)
            self.assertEqual(out.read_bytes(), original)
            self.assertEqual(source.read_bytes(), fixture())
            self.assertNotIn(tmp, out.read_text())

    def test_cli_partial_and_invalid_input_produce_no_report(self):
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / 'report.json'
            for name, data in [('partial.crdownload', fixture()), ('bad.elf', b'bad')]:
                source = Path(tmp) / name
                source.write_bytes(data)
                with contextlib.redirect_stderr(io.StringIO()):
                    self.assertEqual(main([str(source), '--output', str(out)]), 1)
                self.assertFalse(out.exists())


if __name__ == '__main__':
    unittest.main()
