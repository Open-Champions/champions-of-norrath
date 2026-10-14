"""Synthetic regression cases for the partial-build dependency measurements."""
from pathlib import Path
import struct
import tempfile
import unittest

import build_partial as build
from test_rebuild import scalar_elf_fixture


def fixture(extra=()):
    return bytearray(scalar_elf_fixture(kind='consumer', operation='store-float-argument',
                                      extra_symbols=extra))


def inventory(blob):
    with tempfile.TemporaryDirectory() as directory:
        path = Path(directory) / 'fixture.o'
        path.write_bytes(blob)
        return build.elf_inventory(path)


def section(blob, index, field, value):
    shoff = struct.unpack_from('<I', blob, 32)[0]
    struct.pack_into('<I', blob, shoff + index * 40 + field * 4, value)


class PartialInventoryTests(unittest.TestCase):
    def test_common_is_storage_not_an_import(self):
        result = inventory(fixture([('pool', 16, 64, 1, 1, 0, 0xfff2),
                                    ('small', 4, 4, 1, 1, 0, 0xff03),
                                    ('missing', 0, 0, 2, 1, 0, 0xff04)]))
        self.assertEqual(result['undefined_symbols'], ['missing', 'state'])
        self.assertEqual([s['name'] for s in result['common_symbols']], ['pool', 'small'])
        self.assertEqual(result['function_count'], 1)

    def test_rel_and_rela_fields(self):
        blob = fixture()
        # Repurpose 24-byte metadata as two synthetic RELA entries.
        section(blob, 3, 1, 4)
        section(blob, 3, 9, 12)
        self.assertEqual(inventory(blob)['relocation_fields'], 3)

    def test_malformed_relocation_stride(self):
        for stride in (0, 4, 12):
            blob = fixture()
            section(blob, 6, 9, stride)
            with self.assertRaisesRegex(ValueError, 'relocation'):
                inventory(blob)

    def test_nonconcrete_function_and_invalid_extent(self):
        for index, value, size in ((0xfff1, 0, 4), (0xfff2, 4, 4),
                                   (1, 4, 8), (2, 0, 4)):
            with self.assertRaisesRegex(ValueError, 'Function'):
                inventory(fixture([('bad', value, size, 2, 1, 0, index)]))

    def test_equal_summed_size_cannot_hide_replaced_body(self):
        before = {'functions': [{'name': 'first', 'compiled_body_bytes': 8}]}
        after = {'functions': [{'name': 'other', 'compiled_body_bytes': 8}]}
        with self.assertRaisesRegex(ValueError, 'symbols/extents'):
            build.validate_linked_functions([before], after)

    def test_multiset_preserves_duplicate_local_names_and_zero_markers(self):
        before = {'functions': [{'name': 'local', 'compiled_body_bytes': 8},
                                {'name': 'marker', 'compiled_body_bytes': 0}]}
        build.validate_linked_functions([before, before],
                                        {'functions': before['functions'] * 2})
        with self.assertRaises(ValueError):
            build.validate_linked_functions([before, before], before)


if __name__ == '__main__':
    unittest.main()
