#include "../w9-rtti-vtables-entities-c-bytes/weapon_slot.hpp"
// Observed initType__11MaleDarkElfb at 0x2870d8, 1336 bytes, emitted here as
// norrath_male_dark_elf_init_type (the name the MaleDarkElf constructor
// imports). The argument is a flag. In order:
//  1. setParameters__8Creaturefffffi(object, 0x3fb33333, 0x3f999999, 1.0, 1.0,
//     1.0, flag ^ 1).
//  2. The signed byte at +960 (kind): a negative kind leaves the byte at +156
//     untouched; the byte at +156 is cleared in every case (the store sits in
//     a branch delay slot). A kind of exactly 5 stores the single 0x3e4ccccc at
//     +96.
//  3. The halfwords at +1076, +1084, +1092 and +1100 become -1 (four weapon
//     slots, see weapon_slot.hpp). Then the word at +208 (1 to 5) selects, with
//     factor 0x3e4ccccc; "kind" is the signed byte at +960:
//       1: kind 0: byte +156 = 3; else slot (skills word 26, type 23), byte 4.
//       2: kind 0: byte +156 = 2; else byte +156 = 8.
//       3: kind 0: byte +156 = 2; else byte +156 = 8 and the word +212 = 1.
//       4: kind 0: byte +156 = 4; kind 1: slot (14, 22), byte 3; kind 2: slot
//          (12, 24), byte 1; kind 3: slot (13, 21), byte 2; other kinds nothing.
//       5: kind 0: byte +156 = 8; else nothing.
//     Any other value of +208 does nothing here.
//  4. The 64-bit word at +160 loses bits 33, 34, 36 and 37; then by the word at
//     +244: any of 0x4112e6b sets bit 36; else any of 0xd100 sets bit 37; else
//     any of 0x1a00014 sets bit 34; else bit 33 is set.
// Unknown: the meaning of the kinds and of the flag bits.
extern "C" void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int flag);

extern "C" void norrath_male_dark_elf_init_type(void* self, int flag) {
    using namespace rtti_c;
    const Address object = address(self);
    setParameters__8Creaturefffffi(self, single_from_bits(0x3fb33333u), single_from_bits(0x3f999999u), single_from_bits(0x3f800000u),
                                   single_from_bits(0x3f800000u), single_from_bits(0x3f800000u), flag ^ 1);
    const int kind = read_s8(object + 960u);
    write_u8(object + 156u, 0u);
    if (kind == 5) write_single(object + 96u, single_from_bits(0x3e4ccccc));
    for (unsigned slot = 0; slot < 4u; ++slot) write_u16(object + 1076u + 8u * slot, 0xffffu);
    switch (read_word(object + 208u) - 1u) {
    case 0u:
        if (kind != 0) {
            add_weapon_slot(self, 1076u, 26u, 23u, 0x3e4ccccc);
            write_u8(object + 156u, 4u);
        } else {
            write_u8(object + 156u, 3u);
        }
        break;
    case 1u:
        write_u8(object + 156u, kind != 0 ? 8u : 2u);
        break;
    case 2u:
        if (kind != 0) {
            write_word(object + 212u, 1u);
            write_u8(object + 156u, 8u);
        } else {
            write_u8(object + 156u, 2u);
        }
        break;
    case 3u:
        if (kind == 0) {
            write_u8(object + 156u, 4u);
        } else if (kind == 1) {
            add_weapon_slot(self, 1076u, 14u, 22u, 0x3e4ccccc);
            write_u8(object + 156u, 3u);
        } else if (kind == 2) {
            add_weapon_slot(self, 1076u, 12u, 24u, 0x3e4ccccc);
            write_u8(object + 156u, 1u);
        } else if (kind == 3) {
            add_weapon_slot(self, 1076u, 13u, 21u, 0x3e4ccccc);
            write_u8(object + 156u, 2u);
        }
        break;
    case 4u:
        if (kind == 0) write_u8(object + 156u, 8u);
        break;
    default:
        break;
    }
    const unsigned long long cleared = read_u64(object + 160u) & 0xffffffc9ffffffffull;
    const unsigned mask = read_word(object + 244u);
    write_u64(object + 160u, cleared);
    if ((mask & 0x4112e6bu) != 0u) write_u64(object + 160u, cleared | (1ull << 36));
    else if ((mask & 0xd100u) != 0u) write_u64(object + 160u, cleared | (1ull << 37));
    else if ((mask & 0x1a00014u) != 0u) write_u64(object + 160u, cleared | (1ull << 34));
    else write_u64(object + 160u, cleared | (1ull << 33));
}
