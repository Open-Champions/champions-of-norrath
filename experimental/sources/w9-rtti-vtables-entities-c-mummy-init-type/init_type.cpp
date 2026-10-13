#include "../w9-rtti-vtables-entities-c-bytes/weapon_slot.hpp"
// Observed initType__5Mummyb at 0x2371a8, 1196 bytes, emitted here as
// norrath_mummy_init_type (the name the Mummy constructor imports). The
// argument is a flag. The byte at +156 becomes 0. The word at +960 (kind)
// below 6 selects setParameters__8Creaturefffffi(object, a, b, c, d, e,
// flag ^ 1) with singles given as bit patterns, followed by one of two sets of
// stores:
//   store set A: single -0.2 (0xbe4ccccc) at +88, 0.5 at +100, 0x3e999999 at +96;
//   store set B: A's three stores plus 0x3e4ccccc at +104 and byte +156 = 6.
//   kind 0: (1.0 x5), A.    kind 1: (1.0 x5), B.   kind 2: (0x3f999999 x5), A.
//   kind 3: (0x3f4ccccc, 1.0 x4), B.
//   kind 4: (1.5, 0x3fa66666, 0x3fb33333, 0x3f999999, 0x3fa66666), A.
//   kind 5: (0x3fa66666, 1.0, 1.0, 0x3f999999, 0x3f999999), B.
//   kind 6 and above: none of that.
// Then the halfwords at +1068, +1076, +1084 and +1092 become -1 (four weapon
// slots, see weapon_slot.hpp) and the word at +208 selects, with factor
// 0x3e4ccccc:
//   1: slot (skills word 25, type 38), byte +156 = 7.
//   2: kind 2: slot (14, 22), byte 3; any other kind: slot (13, 21), byte 2.
//   3: byte +156 = 8.
//   4: byte +156 = 8 and the word at +212 = 1.
//   anything else: nothing more.
// Unknown: what the kinds, the numbers and the word at +212 mean.
extern "C" void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int flag);

namespace {
using namespace rtti_c;
__attribute__((always_inline)) inline void params(void* self, unsigned a, unsigned b, unsigned c, unsigned d, unsigned e, int flag) {
    setParameters__8Creaturefffffi(self, single_from_bits(a), single_from_bits(b), single_from_bits(c), single_from_bits(d), single_from_bits(e), flag ^ 1);
}
__attribute__((always_inline)) inline void store_set_a(Address object) {
    write_single(object + 88u, single_from_bits(0xbe4ccccc));
    write_single(object + 100u, single_from_bits(0x3f000000u));
    write_single(object + 96u, single_from_bits(0x3e999999u));
}
__attribute__((always_inline)) inline void store_set_b(Address object) {
    store_set_a(object);
    write_single(object + 104u, single_from_bits(0x3e4ccccc));
    write_u8(object + 156u, 6u);
}
}

extern "C" void norrath_mummy_init_type(void* self, int flag) {
    const Address object = address(self);
    write_u8(object + 156u, 0u);
    switch (read_word(object + 960u)) {
    case 0u:
        params(self, 0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u, flag);
        store_set_a(object);
        break;
    case 1u:
        params(self, 0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u, flag);
        store_set_b(object);
        break;
    case 2u:
        params(self, 0x3f999999u, 0x3f999999u, 0x3f999999u, 0x3f999999u, 0x3f999999u, flag);
        store_set_a(object);
        break;
    case 3u:
        params(self, 0x3f4cccccu, 0x3f800000u, 0x3f800000u, 0x3f800000u, 0x3f800000u, flag);
        store_set_b(object);
        break;
    case 4u:
        params(self, 0x3fc00000u, 0x3fa66666u, 0x3fb33333u, 0x3f999999u, 0x3fa66666u, flag);
        store_set_a(object);
        break;
    case 5u:
        params(self, 0x3fa66666u, 0x3f800000u, 0x3f800000u, 0x3f999999u, 0x3f999999u, flag);
        store_set_b(object);
        break;
    default:
        break;
    }
    for (unsigned slot = 0; slot < 4u; ++slot) write_u16(object + 1068u + 8u * slot, 0xffffu);
    switch (static_cast<int>(read_word(object + 208u))) {
    case 1:
        add_weapon_slot(self, 1068u, 25u, 38u, 0x3e4ccccc);
        write_u8(object + 156u, 7u);
        break;
    case 2:
        if (read_word(object + 960u) == 2u) {
            add_weapon_slot(self, 1068u, 14u, 22u, 0x3e4ccccc);
            write_u8(object + 156u, 3u);
        } else {
            add_weapon_slot(self, 1068u, 13u, 21u, 0x3e4ccccc);
            write_u8(object + 156u, 2u);
        }
        break;
    case 3:
        write_u8(object + 156u, 8u);
        break;
    case 4:
        write_u8(object + 156u, 8u);
        write_word(object + 212u, 1u);
        break;
    default:
        break;
    }
}
