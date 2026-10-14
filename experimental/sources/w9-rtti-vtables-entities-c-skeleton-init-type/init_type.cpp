#include "../w9-rtti-vtables-entities-c-bytes/weapon_slot.hpp"
// Observed initType__8Skeletonb at 0x2442b8, 1088 bytes, emitted here as
// norrath_skeleton_init_type (the name the Skeleton constructor imports).
// The argument is a flag. In order:
//  1. setParameters__8Creaturefffffi(object, 1.3 (0x3fa66666), 1.2 (0x3f999999),
//     1.0, 1.0, 1.0, flag ^ 1).
//  2. The 64-bit word at +160 loses bits 35, 37 and 40, and the byte at +156
//     becomes 0. Then, by the word at +244: any of the bits 0x21e00 sets bit 35;
//     else any of the bits 0x1de000 sets bit 37; else bit 40 is set.
//  3. The single at +788 becomes 200.0 when the signed byte at +972 is nonzero,
//     else 1000.0.
//  4. The halfwords at +1084, +1092, +1100 and +1108 become -1 (four 8-byte
//     weapon slots). Then the word at +208 selects (see weapon_slot.hpp for the
//     slot routine; "row" gives skills word, slot type and factor 0x3e4ccccc):
//       1: +968 equal to 4: byte +156 = 3; +968 equal to 2 or 3: slot row
//          (13, 21) and byte 2; any other +968: slot row (12, 24) and byte 1.
//       2: +968 equal to 4: byte +156 = 2; otherwise slot row (14, 22) and
//          byte 3.
//       3: +968 equal to 4: byte +156 = 2.
//       4: +968 equal to 4: byte +156 = 8.
//       anything else: nothing more.
// Unknown: the meaning of the flag word bits, the numbers 1.3 and 1.2, and what
// +968 selects.
extern "C" void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int flag);

extern "C" void norrath_skeleton_init_type(void* self, int flag) {
    using namespace rtti_c;
    const Address object = address(self);
    setParameters__8Creaturefffffi(self, single_from_bits(0x3fa66666u), single_from_bits(0x3f999999u),
                                   single_from_bits(0x3f800000u), single_from_bits(0x3f800000u),
                                   single_from_bits(0x3f800000u), flag ^ 1);
    const unsigned long long cleared = read_u64(object + 160u) & 0xfffffed7ffffffffull;
    const unsigned mask = read_word(object + 244u);
    write_u64(object + 160u, cleared);
    write_u8(object + 156u, 0u);
    if ((mask & 0x21e00u) != 0u) write_u64(object + 160u, cleared | (1ull << 35));
    else if ((mask & 0x1de000u) != 0u) write_u64(object + 160u, cleared | (1ull << 37));
    else write_u64(object + 160u, cleared | (1ull << 40));
    write_single(object + 788u, read_s8(object + 972u) != 0 ? single_from_bits(0x43480000u) : single_from_bits(0x447a0000u));
    for (unsigned slot = 0; slot < 4u; ++slot) write_u16(object + 1084u + 8u * slot, 0xffffu);
    const int variant = static_cast<int>(read_word(object + 208u));
    const unsigned selector = read_word(object + 968u);
    switch (variant) {
    case 1:
        if (selector == 4u) {
            write_u8(object + 156u, 3u);
        } else if (selector - 2u < 2u) {
            add_weapon_slot(self, 1084u, 13u, 21u, 0x3e4ccccc);
            write_u8(object + 156u, 2u);
        } else {
            add_weapon_slot(self, 1084u, 12u, 24u, 0x3e4ccccc);
            write_u8(object + 156u, 1u);
        }
        break;
    case 2:
        if (selector == 4u) {
            write_u8(object + 156u, 2u);
        } else {
            add_weapon_slot(self, 1084u, 14u, 22u, 0x3e4ccccc);
            write_u8(object + 156u, 3u);
        }
        break;
    case 3:
        if (selector == 4u) write_u8(object + 156u, 2u);
        break;
    case 4:
        if (selector == 4u) write_u8(object + 156u, 8u);
        break;
    default:
        break;
    }
}
