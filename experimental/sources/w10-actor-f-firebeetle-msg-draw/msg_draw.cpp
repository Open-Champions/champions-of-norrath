#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_draw__10FireBeetle at 0x00210338, 744 bytes. Let p be the position floats at object+16, +20,
// +24, state the signed halfword at +572, t the word at +808, group the word at +36 and n = frameNm.
//  1. A 12-byte copy of p is kept on the stack.
//  2. State 13: p moves by x += icos(1500 t) * 3, y += isin(1500 t) * 3, z += icos(1500 t) * 5. The angle is
//     recomputed from the word at +808 before each trig call, and the three calls run in that order.
//  3. State 14: with d = n - (signed word at +248), fxRunCreatureGlow is called with the object, the model at
//     group+12, the animation state at object+288, a copy of p, the signed halfword at +56, the glow state at
//     object+824, the floats 0.0, 1.0, 0.0, two four-short tables (250, 221, 61, 96 and 150, 132, 37, 23), an
//     integer selector, a fourth float and the integer -2000. d below 16: selector 0, fourth float 0.25;
//     16 up to 40: selector 1, fourth float 1.0; 41 and above: selector 2, fourth float 0.25. Then the four
//     floats at object+108..120 are truncated to shorts and passed to lightSetAmbientOffset.
//  4. For every state: Creature's draw with the model at group+12, the texture at group+20 and a null
//     fourth argument, then lightResetOptionalParams.
//  5. The saved 12 bytes are written back to the position.
// Unknown: what the two short tables, the selector, the fourth float and the -2000 mean, and what
// states 13 and 14 are.
extern "C" {
extern int frameNm;
float icos__Fi(int angle);
float isin__Fi(int angle);
void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra);
void lightResetOptionalParams__Fv();
void lightSetAmbientOffset__FPs(short* values);
void fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi(
    void* object, void* model, void* animation, const void* point, short heading, void* glow, float first,
    float second, float third, short* table_a, short* table_b, int selector, float fourth, int last);
}

namespace {
inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }
}

extern "C" void msg_draw__10FireBeetle(void* self) {
    using namespace vslots_b;
    const Address object = address(self);
    const Address position = object + 16u;
    unsigned char saved[12];
    for (unsigned i = 0; i < 12u; ++i) saved[i] = bytes(position)[i];

    if (read_i16(object + 572u) == 13) {
        const float three = constant(0x40400000u);
        write_single(position, read_single(position) +
                                   icos__Fi(static_cast<int>(read_word(object + 808u)) * 1500) * three);
        write_single(position + 4u, read_single(position + 4u) +
                                        isin__Fi(static_cast<int>(read_word(object + 808u)) * 1500) * three);
        write_single(position + 8u, read_single(position + 8u) +
                                        icos__Fi(static_cast<int>(read_word(object + 808u)) * 1500) *
                                            constant(0x40a00000u));
    }
    if (read_i16(object + 572u) == 14) {
        short table_a[4] = {250, 221, 61, 96};
        short table_b[4] = {150, 132, 37, 23};
        const int elapsed = frameNm - static_cast<int>(read_word(object + 248u));
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = bytes(position)[i];
        void* model = pointer(read_word(address(pointer(read_word(object + 36u))) + 12u));
        const short heading = read_i16(object + 56u);
        const float zero = 0.0f;
        const float one = constant(0x3f800000u);
        if (elapsed < 16) {
            fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi(
                self, model, bytes(object + 288u), point, heading, bytes(object + 824u), zero, one, zero, table_a,
                table_b, 0, constant(0x3e800000u), -2000);
        } else if (elapsed < 41) {
            fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi(
                self, model, bytes(object + 288u), point, heading, bytes(object + 824u), zero, one, zero, table_a,
                table_b, 1, one, -2000);
        } else {
            fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi(
                self, model, bytes(object + 288u), point, heading, bytes(object + 824u), zero, one, zero, table_a,
                table_b, 2, constant(0x3e800000u), -2000);
        }
        short ambient[4];
        for (unsigned i = 0; i < 4u; ++i) ambient[i] = static_cast<short>(static_cast<int>(read_single(object + 108u + 4u * i)));
        lightSetAmbientOffset__FPs(ambient);
    }
    const Address group = address(pointer(read_word(object + 36u)));
    draw__8CreatureP12_modelHeaderP8_textureT1(self, pointer(read_word(group + 12u)), pointer(read_word(group + 20u)), nullptr);
    lightResetOptionalParams__Fv();
    for (unsigned i = 0; i < 12u; ++i) bytes(position)[i] = saved[i];
}
