"""Game-free synthetic checks of linked symbol closure and relocation controls."""
import struct
import unittest

import link_smoke as smoke
import rebuild as r
from test_rebuild import scalar_elf_fixture

GP = r.address(r.GP_SPEC['gp_symbol']['address'])
OWNER = {'id': 'state', 'symbol': 'state', 'address': '0x%08x' % (GP - 32), 'size': 4}
FUNCTION = {'id': 'consumer', 'symbol': 'consumer', 'address': '0x00100000', 'size': 12,
            'comparison_mode': r.SCALAR_V2_MODE,
            'data_binding': {'owner_id': 'state', 'operation': 'set-one'}}
ORIGINAL = struct.pack('<3I', 0x24020001, 0x03e00008, 0xaf82ffe0)


def fixture(shift=0, code=None, initializer=bytes(4), undefined=False, common=False,
            relocation=False, gp=GP, owner_address=None, function_address=0x100000,
            extra_payload=False, writable=True, owner_size=4):
    """Construct ELF sections/symbols independently of production code."""
    if code is None:
        code = struct.pack('<3I', 0x24020001, 0x03e00008, 0xaf820000 | ((-32 + shift) & 65535))
    data_address = GP - 32 + shift if owner_address is None else owner_address
    names = ['', '.text.consumer', '.sdata.state', '.symtab', '.strtab', '.shstrtab', '.rel.text', '.extra']
    string_table = b'\0consumer\0state\0_gp\0missing\0'
    symbols = bytes(16)
    for name, value, size, typ, section in [
            ('consumer', function_address, len(code), 2, 1),
            ('state', data_address, owner_size, 1, 2), ('_gp', gp, 0, 0, 0xfff1)]:
        symbols += struct.pack('<IIIBBH', string_table.index(name.encode()), value, size, 0x10 | typ, 0, section)
    if undefined or common:
        symbols += struct.pack('<IIIBBH', string_table.index(b'missing'), 0, 0, 0x10, 0, 0xfff2 if common else 0)
    shstrings = b'\0' + b''.join(n.encode() + b'\0' for n in names[1:])
    rows = [(b'', 0, 0, 0, 0, 0, 0), (code, 1, 6, 0x100000, 0, 0, 0),
            (initializer, 1, 3 if writable else 2, data_address, 0, 0, 0),
            (symbols, 2, 0, 0, 4, 1, 16), (string_table, 3, 0, 0, 0, 0, 0),
            (shstrings, 3, 0, 0, 0, 0, 0),
            (struct.pack('<II', 8, 0x207) if relocation else b'', 9, 0, 0, 3, 1, 8),
            (b'abcd' if extra_payload else b'', 1, 2, 0x600000, 0, 0, 0)]
    data, sections = bytearray(52), []
    for name, (blob, typ, flags, address, link, info, entry) in zip(names, rows):
        data.extend(bytes(-len(data) % 4))
        sections.append((shstrings.index(name.encode() + b'\0'), typ, flags, address,
                         len(data), len(blob), link, info, 4, entry))
        data.extend(blob)
    data.extend(bytes(-len(data) % 4))
    shoff = len(data)
    for section in sections:
        data.extend(struct.pack('<10I', *section))
    data[:16] = b'\x7fELF\x01\x01\x01' + bytes(9)
    struct.pack_into('<HHIIIIIHHHHHH', data, 16, 2, 8, 1, 0x100000, 0,
                     shoff, r.BASELINE_FLAGS, 52, 0, 0, 40, len(sections), 5)
    return r.Elf32(bytes(data))


def check(elf, shift=0):
    return smoke.check_linked(elf, [FUNCTION], [OWNER], {'consumer': ORIGINAL}, shift)


class LinkSmokeTests(unittest.TestCase):
    def test_float_and_byte_consumer_abi(self):
        for mode, operation, opcode, width in ((r.FLOAT_MODE, 'store-float-argument', 0xe78c0000, 4),
                                                (r.BYTE_MODE, 'store-byte-argument', 0xa3840000, 1)):
            with self.subTest(mode=mode):
                item = dict(FUNCTION, size=8, comparison_mode=mode,
                            data_binding={'owner_id': 'state', 'object_symbol': 'state', 'operation': operation})
                owner = dict(OWNER, comparison_mode=mode, size=width)
                original = struct.pack('<2I', 0x03e00008, opcode | 0xffe0)
                blob = scalar_elf_fixture(kind='consumer', operation=operation, candidate_name='consumer')
                smoke.check_consumer_object(r.Elf32(blob), item, original, owner)
                for section in ('.MIPS.abiflags', '.gnu.attributes'):
                    elf = r.Elf32(blob)
                    _, metadata = elf.named_section(section)
                    changed = bytearray(blob)
                    changed[metadata[4]] ^= 1
                    with self.subTest(section=section), self.assertRaisesRegex(ValueError, 'ABI metadata'):
                        smoke.check_consumer_object(r.Elf32(bytes(changed)), item, original, owner)

    def test_group_cannot_expand_silently(self):
        ledger = r.load_ledger()
        functions, owners = smoke.select_group(ledger)
        self.assertEqual((len(functions), len(owners)), (4, 4))
        functions[0]['size'] += 4
        with self.assertRaisesRegex(ValueError, 'contract changed'):
            smoke.select_group(ledger)

    def test_original_complete_function_and_initializer(self):
        self.assertEqual(check(fixture())[0]['fixups'], 1)

    def test_shifted_owner_and_consumer(self):
        check(fixture(shift=256), shift=256)

    def test_named_data_movement_must_relocate_code(self):
        with self.assertRaisesRegex(ValueError, 'complete linked function differs'):
            check(fixture(shift=256, code=ORIGINAL), shift=256)

    def test_no_undefined_imports(self):
        with self.assertRaisesRegex(ValueError, 'undefined or COMMON'):
            check(fixture(undefined=True))

    def test_no_common_storage(self):
        with self.assertRaisesRegex(ValueError, 'undefined or COMMON'):
            check(fixture(common=True))

    def test_no_remaining_relocations(self):
        with self.assertRaisesRegex(ValueError, 'unresolved relocations'):
            check(fixture(relocation=True))

    def test_pinned_gp(self):
        with self.assertRaisesRegex(ValueError, 'linked GP differs'):
            check(fixture(gp=GP + 4))

    def test_complete_extent(self):
        with self.assertRaisesRegex(ValueError, 'function identity differs'):
            check(fixture(code=ORIGINAL + bytes(4)))

    def test_nonrelocation_instruction_is_checked(self):
        code = struct.pack('<I', 0x24020002) + ORIGINAL[4:]
        with self.assertRaisesRegex(ValueError, 'complete linked function differs'):
            check(fixture(code=code))

    def test_owner_address(self):
        with self.assertRaisesRegex(ValueError, 'owner identity differs'):
            check(fixture(owner_address=GP - 28))

    def test_owner_width(self):
        with self.assertRaisesRegex(ValueError, 'owner identity differs'):
            check(fixture(owner_size=1))

    def test_owner_initializer(self):
        with self.assertRaisesRegex(ValueError, 'initializer differs'):
            check(fixture(initializer=b'\x01\0\0\0'))

    def test_writable_owner(self):
        with self.assertRaisesRegex(ValueError, 'owner storage differs'):
            check(fixture(writable=False))

    def test_extra_allocated_payload(self):
        with self.assertRaisesRegex(ValueError, 'unexpected allocated'):
            check(fixture(extra_payload=True))

    def test_missing_group_member(self):
        with self.assertRaisesRegex(ValueError, 'missing or duplicate'):
            smoke.select_group({'functions': [], 'data_objects': []})

    def test_out_of_range_placement(self):
        with self.assertRaisesRegex(ValueError, 'GP relocation range'):
            smoke.linker_script([FUNCTION], [OWNER], 65536)

    def test_misaligned_placement(self):
        with self.assertRaisesRegex(ValueError, 'misaligned'):
            smoke.linker_script([FUNCTION], [OWNER], 1)

    def test_script_resolves_owners_from_input_files(self):
        script = smoke.linker_script([FUNCTION], [OWNER])
        self.assertIn('state.o(.sdata)', script)
        self.assertNotIn('state =', script)

    def test_two_fixups_change_and_opcodes_stay(self):
        raw = struct.pack('<4I', 0x8f820000, 0x2442000a, 0x03e00008, 0xaf820000)
        result = smoke.shifted_code(raw, [0, 12], -128)
        self.assertEqual(struct.unpack('<4I', result), (0x8f82ff80, 0x2442000a, 0x03e00008, 0xaf82ff80))

    def test_bad_fixup_offsets_and_overflow(self):
        for offsets, displacement in (([2], 0), ([-4], 0), ([12], 0), ([8], 32768), ([8], -32769)):
            with self.subTest(offsets=offsets, displacement=displacement), self.assertRaises(ValueError):
                smoke.shifted_code(ORIGINAL, offsets, displacement)

    def test_wrong_executable_flags(self):
        elf = fixture()
        elf.flags = 0
        with self.assertRaisesRegex(ValueError, 'type or flags'):
            check(elf)


if __name__ == '__main__':
    unittest.main()
