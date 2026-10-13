#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__7FireFly at 0x00211ad0, 564 bytes. Let p be the position floats at object+16, +20, +24,
// t the integer word at object+808 (a clock), h the signed halfword at +56, and n = frameNm.
//  1. A 12-byte copy of p is kept on the stack.
//  2. p is moved by a small orbit: x += icos(1000 t) * 10, y += isin(1000 t) * 10, z += icos(1000 t) * 10,
//     then z += 20. The angle is recomputed from the word at +808 before each of the three trig calls.
//  3. A forward offset v = (icos(h) * -5, isin(h) * -5, 30) is added to p.
//  4. P_FaceSprite with a copy of p, 5.0, isin(n << 14) * 1000 + K (K is the single 0x467b0c00 from the
//     image's small-constant area), colour (250, 150, 0), alpha 40, haloTexture4, angle 0, mode 2.
//  5. v is recomputed the same way and subtracted from p (the trig calls run again, in the same order).
//  6. msg_draw__10FireBeetle runs on the object.
//  7. The saved 12 bytes are written back to the position.
// The restoring copy makes the net position change zero; only the float arithmetic in between is visible
// to the callees. Unknown: what the sprite's size and brightness arguments mean.
extern "C" {
extern int frameNm;
extern void* haloTexture4;
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* point, float first, float second, int red, int green,
                                               int blue, int alpha, void* texture, short angle, int mode);
void msg_draw__10FireBeetle(void* self);
}

namespace {
using namespace vslots_b;
inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }
inline int clock_angle(Address object) { return static_cast<int>(read_word(object + 808u)) * 1000; }
}

extern "C" void msg_draw__7FireFly(void* self) {
    const Address object = address(self);
    const Address position = object + 16u;
    unsigned char saved[12];
    for (unsigned i = 0; i < 12u; ++i) saved[i] = bytes(position)[i];
    const float ten = constant(0x41200000u);
    const float minus_five = constant(0xc0a00000u);
    const float thirty = constant(0x41f00000u);

    write_single(position, read_single(position) + icos__Fi(clock_angle(object)) * ten);
    write_single(position + 4u, read_single(position + 4u) + isin__Fi(clock_angle(object)) * ten);
    write_single(position + 8u, read_single(position + 8u) + icos__Fi(clock_angle(object)) * ten + constant(0x41a00000u));

    float offset[3];
    const float cosine = icos__Fi(static_cast<short>(read_i16(object + 56u)));
    const float sine = isin__Fi(static_cast<short>(read_i16(object + 56u)));
    offset[0] = cosine * minus_five;
    offset[1] = sine * minus_five;
    offset[2] = thirty;
    write_single(position + 8u, read_single(position + 8u) + thirty);
    write_single(position + 4u, read_single(position + 4u) + offset[1]);
    write_single(position, read_single(position) + offset[0]);

    const float pulse = isin__Fi(frameNm << 14) * constant(0x447a0000u);
    unsigned char sprite_point[12];
    for (unsigned i = 0; i < 12u; ++i) sprite_point[i] = bytes(position)[i];
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(sprite_point, constant(0x40a00000u), pulse + constant(0x467b0c00u), 250,
                                              150, 0, 40, haloTexture4, 0, 2);

    float back[3];
    const float cosine2 = icos__Fi(static_cast<short>(read_i16(object + 56u)));
    const float sine2 = isin__Fi(static_cast<short>(read_i16(object + 56u)));
    back[0] = cosine2 * minus_five;
    back[1] = sine2 * minus_five;
    back[2] = thirty;
    write_single(position + 8u, read_single(position + 8u) - thirty);
    write_single(position + 4u, read_single(position + 4u) - back[1]);
    write_single(position, read_single(position) - back[0]);

    msg_draw__10FireBeetle(self);
    for (unsigned i = 0; i < 12u; ++i) bytes(position)[i] = saved[i];
}
