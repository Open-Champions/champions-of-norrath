#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed init__10StaticFire at 0x28df70, 276 bytes, emitted here as
// norrath_static_fire_init (the name the StaticFire constructor imports).
// Stores, in object fields: single 16.0 at +28; halfword 0 at +56, +120 and
// +122; byte 60 at +40; single 25000.0 (the small-data constant at gp-21460,
// 0x46c35000) at +104; word -1 at +124; single 1.0 at +84 and +100; byte 0 at
// +108; flag word at +12 gets 0x40001 or'd in. One eRand__10GameObject call
// (before those stores) gives a roll; the single at +76 is the roll modulo 300
// (unsigned) converted from a signed integer. Then
// newWind__10StaticFirei(object, the halfword at +56 read as signed),
// objectAddToRunList__FP10GameObject, objectAlwaysDraw__FP10GameObject. The
// tail depends on the signed byte at +111:
//   nonzero: halfword 250 at +118; memAlloc__FiPc(2500, "smokebomb") result
//     stored at +1188; single 600.0 at +80;
//   zero: single 480.0 at +80; halfword 100 at +118; the address of object+188
//     stored at +1188; when the signed byte at +112 is nonzero the +80 single
//     becomes 240.0.
// The "smokebomb" string is compared by content; the original passes the
// address of its own copy at 0x4778e8.
// Unknown: what the fields are, and what newWind sets up.
extern "C" {
unsigned eRand__10GameObject(void* object);
void newWind__10StaticFirei(void* object, int value);
void objectAddToRunList__FP10GameObject(void* object);
void objectAlwaysDraw__FP10GameObject(void* object);
void* memAlloc__FiPc(int size, const char* name);
}

extern "C" void norrath_static_fire_init(void* self) {
    using namespace rtti_c;
    const Address object = address(self);
    write_single(object + 28u, single_from_bits(0x41800000u));
    write_u16(object + 56u, 0u);
    write_u8(object + 40u, 60u);
    write_u16(object + 120u, 0u);
    const unsigned roll = eRand__10GameObject(self);
    write_u16(object + 122u, 0u);
    write_single(object + 104u, single_from_bits(0x46c35000u));
    write_word(object + 124u, 0xffffffffu);
    write_single(object + 84u, single_from_bits(0x3f800000u));
    write_single(object + 100u, single_from_bits(0x3f800000u));
    write_word(object + 12u, read_word(object + 12u) | 0x40001u);
    write_u8(object + 108u, 0u);
    write_single(object + 76u, static_cast<float>(static_cast<int>(roll % 300u)));
    newWind__10StaticFirei(self, static_cast<short>(read_u16(object + 56u)));
    objectAddToRunList__FP10GameObject(self);
    objectAlwaysDraw__FP10GameObject(self);
    if (read_s8(object + 111u) != 0) {
        write_u16(object + 118u, 250u);
        write_word(object + 1188u, address_bits(memAlloc__FiPc(2500, "smokebomb")));
        write_single(object + 80u, single_from_bits(0x44160000u));
        return;
    }
    write_single(object + 80u, single_from_bits(0x43f00000u));
    write_u16(object + 118u, 100u);
    write_word(object + 1188u, address_bits(bytes(object + 188u)));
    if (read_s8(object + 112u) != 0) write_single(object + 80u, single_from_bits(0x43700000u));
}
