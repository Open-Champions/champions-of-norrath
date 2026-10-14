#include "../w9-rtti-vtables-entities-c-bytes/weapon_slot.hpp"
// Observed initType__3Orcb at 0x2270b8, 1408 bytes, emitted here as
// norrath_orc_init_type (the name the Orc constructor imports). The argument is
// a flag. The byte at +156 becomes 0 and bits 33 to 35 of the 64-bit word at
// +160 are cleared. The word at +1116 (kind) then selects
// setParameters__8Creaturefffffi(object, a, b, c, d, e, flag ^ 1) and one more
// bit of the +160 word (singles given as their bit patterns):
//   0 and 4: (0x3f8ccccc, 0x3f4ccccc, 0x3f8ccccc, 0x3f4ccccc, 0x3f4ccccc); bit 33.
//   1: (2.0, 2.0, 0x3fa66666, 2.0, 2.0); bit 35.
//   2: (0x3fe66666, 0x3f999999, 0x3fa66666, 0x3f999999, 0x3f999999); bit 35.
//   3: (0x40266666, 0x3f999999, 0x3fa66666, 0x3f999999, 0x3fb33333); bit 35;
//      the singles at +28 and +224 become 22.0 and 0x3fa66666.
//   5: (0x3fa66666, 1.0, 0x3fa66666, 1.0, 1.0); bit 34; the byte at +156 = 6.
//   anything else: none of that.
// Then the halfwords at +1076, +1084, +1092 and +1100 become -1 (four weapon
// slots, see weapon_slot.hpp) and the word at +208 selects, with the factor
// 0x3e4ccccc:
//   1: kind 4: byte +156 = 3; else slot (skills word 12, type 24), byte 1.
//   2: kind 4: byte +156 = 2; else slot (14, 22), byte 3.
//   3: kind 4: byte +156 = 2; else slot (13, 21), byte 2.
//   4: slot (26, 23), byte 4.
//   anything else: nothing more.
// Unknown: what the kinds and the numbers mean.
extern "C" void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int flag);

namespace {
using namespace rtti_c;
__attribute__((always_inline)) inline void params(void* self, unsigned a, unsigned b, unsigned c, unsigned d, unsigned e, int flag) {
    setParameters__8Creaturefffffi(self, single_from_bits(a), single_from_bits(b), single_from_bits(c), single_from_bits(d), single_from_bits(e), flag ^ 1);
}
}

extern "C" void norrath_orc_init_type(void* self, int flag) {
    using namespace rtti_c;
    const Address object = address(self);
    write_u8(object + 156u, 0u);
    write_u64(object + 160u, read_u64(object + 160u) & 0xfffffff1ffffffffull);
    const unsigned kind = read_word(object + 1116u);
    if (kind == 0u || kind == 4u) {
        params(self, 0x3f8cccccu, 0x3f4cccccu, 0x3f8cccccu, 0x3f4cccccu, 0x3f4cccccu, flag);
        write_u64(object + 160u, read_u64(object + 160u) | (1ull << 33));
    } else if (kind == 1u) {
        params(self, 0x40000000u, 0x40000000u, 0x3fa66666u, 0x40000000u, 0x40000000u, flag);
        write_u64(object + 160u, read_u64(object + 160u) | (1ull << 35));
    } else if (kind - 2u < 2u) {
        if (kind == 3u) {
            write_single(object + 28u, single_from_bits(0x41b00000u));
            write_single(object + 224u, single_from_bits(0x3fa66666u));
            params(self, 0x40266666u, 0x3f999999u, 0x3fa66666u, 0x3f999999u, 0x3fb33333u, flag);
        } else {
            params(self, 0x3fe66666u, 0x3f999999u, 0x3fa66666u, 0x3f999999u, 0x3f999999u, flag);
        }
        write_u64(object + 160u, read_u64(object + 160u) | (1ull << 35));
    } else if (kind == 5u) {
        params(self, 0x3fa66666u, 0x3f800000u, 0x3fa66666u, 0x3f800000u, 0x3f800000u, flag);
        write_u64(object + 160u, read_u64(object + 160u) | (1ull << 34));
        write_u8(object + 156u, 6u);
    }
    for (unsigned slot = 0; slot < 4u; ++slot) write_u16(object + 1076u + 8u * slot, 0xffffu);
    const int variant = static_cast<int>(read_word(object + 208u));
    const unsigned now = read_word(object + 1116u);
    switch (variant) {
    case 1:
        if (now == 4u) {
            write_u8(object + 156u, 3u);
        } else {
            add_weapon_slot(self, 1076u, 12u, 24u, 0x3e4ccccc);
            write_u8(object + 156u, 1u);
        }
        break;
    case 2:
        if (now == 4u) {
            write_u8(object + 156u, 2u);
        } else {
            add_weapon_slot(self, 1076u, 14u, 22u, 0x3e4ccccc);
            write_u8(object + 156u, 3u);
        }
        break;
    case 3:
        if (now == 4u) {
            write_u8(object + 156u, 2u);
        } else {
            add_weapon_slot(self, 1076u, 13u, 21u, 0x3e4ccccc);
            write_u8(object + 156u, 2u);
        }
        break;
    case 4:
        add_weapon_slot(self, 1076u, 26u, 23u, 0x3e4ccccc);
        write_u8(object + 156u, 4u);
        break;
    default:
        break;
    }
}
