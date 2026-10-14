#include "../w9-animation-object-c-common/mem.hpp"
// Observed slideBack__8Creaturef at 0x00111490, 924 bytes. Arguments: the creature and a
// float scale `t`. Does nothing unless t > 0, the float at +756 is nonzero and bit 1 of the
// word at +12 is clear. Then:
//  1. push = (icos(h) * v * t, isin(h) * v * t, 0) where h is the signed halfword at +748 and
//     v is the float at +756 (products taken as (trig * v) * t).
//  2. Saves the two floats at +180 and +184 (an 8-byte pair; +188 is not part of it), stores
//     (0, 0) there, saves the halfword at +188 and stores 0 in it, calls
//     move2__8CreatureG6Point3(creature, push) keeping its result, then restores the pair and
//     the halfword.
//  3. If v > 0 it is lowered by 2.0; if v is then below zero it becomes 0.0.
//  4. If the signed byte at +568 is nonzero, or any of the low two bits of move2's result is
//     set, it stops. Otherwise it makes two dust puffs. For each one:
//     base = (x - r/2 + (eRand % (int)r), y - r/2 + (eRand % (int)r), z + (eRand % 10)) where
//     x, y, z are the floats at +16, +20, +24 and r the float at +28 (the first two remainders
//     are signed, the third unsigned; r is converted to int by truncation);
//     velocity = a point drawn in the unit sphere by rejection (each component
//     2 * (eRand * 2^-16) - 1, redrawn while the squared length is above 1.0) scaled by 10.45;
//     position = base + (eRand % 12 - 6) on each axis (unsigned remainders).
//     If bit 0x100 of the word levelFlags (0x4844c0) is set it calls
//     addEffect__FiG6Point3(17, position), otherwise
//     P_AddParticle__FP11ParticleDefP6Point3T1f(&fxDust, position, velocity, 0.0).
// Inferred: this slides the creature along its facing while its push speed decays by 2.0 per
// call, kicking up dust only when the move was not blocked. Names of fields are not claimed.
// Unknown: what the low two bits of move2's result mean here and what +748/+756 are called.
extern "C" {
extern unsigned char fxDust[];
extern int levelFlags;
float icos__Fi(int angle);
float isin__Fi(int angle);
int move2__8CreatureG6Point3(unsigned char* self, const float* push);
unsigned eRand__10GameObject(unsigned char* self);
void addEffect__FiG6Point3(int kind, const float* position);
void P_AddParticle__FP11ParticleDefP6Point3T1f(unsigned char* definition, const float* position, const float* velocity, float age);
}

extern "C" void slideBack__8Creaturef(unsigned char* self, float t) {
    using namespace norrath_aoc;
    if (!(0.0f < t)) return;
    if (load_float(self + 756) == 0.0f) return;
    if ((load_word(self + 12) & 2u) != 0) return;
    const int angle = load_signed_half(self + 748);
    float push[3];
    push[0] = icos__Fi(angle) * load_float(self + 756) * t;
    push[1] = isin__Fi(angle) * load_float(self + 756) * t;
    push[2] = 0.0f;
    const unsigned saved_x = load_word(self + 180);
    const unsigned saved_y = load_word(self + 184);
    store_float(self + 180, 0.0f);
    store_float(self + 184, 0.0f);
    const unsigned half = load_half(self + 188);
    store_half(self + 188, 0);
    const int moved = move2__8CreatureG6Point3(self, push);
    store_word(self + 180, saved_x);
    store_word(self + 184, saved_y);
    store_half(self + 188, half);
    float speed = load_float(self + 756);
    if (0.0f < speed) {
        speed = speed - 2.0f;
        store_float(self + 756, speed);
    }
    if (speed < 0.0f) store_float(self + 756, 0.0f);
    if (load_signed_byte(self + 568) != 0) return;
    if ((moved & 3) != 0) return;
    for (int puff = 1; puff >= 0; --puff) {
        float base[3];
        const float r = load_float(self + 28);
        base[0] = load_float(self + 16) - r * 0.5f + static_cast<float>(static_cast<int>(eRand__10GameObject(self)) % static_cast<int>(r));
        const float r2 = load_float(self + 28);
        base[1] = load_float(self + 20) - r2 * 0.5f + static_cast<float>(static_cast<int>(eRand__10GameObject(self)) % static_cast<int>(r2));
        base[2] = load_float(self + 24) + static_cast<float>(eRand__10GameObject(self) % 10u);
        float velocity[3];
        float sum;
        do {
            velocity[0] = static_cast<float>(eRand__10GameObject(self)) * 0x1p-16f;
            velocity[0] = velocity[0] + velocity[0] - 1.0f;
            velocity[1] = static_cast<float>(eRand__10GameObject(self)) * 0x1p-16f;
            velocity[1] = velocity[1] + velocity[1] - 1.0f;
            float z = static_cast<float>(eRand__10GameObject(self)) * 0x1p-16f;
            z = z + z - 1.0f;
            velocity[2] = z;
            sum = velocity[0] * velocity[0] + velocity[1] * velocity[1] + z * z;
        } while (1.0f < sum);
        velocity[0] = velocity[0] * 10.45f;
        velocity[1] = velocity[1] * 10.45f;
        velocity[2] = velocity[2] * 10.45f;
        float position[3];
        position[0] = static_cast<float>(static_cast<int>(eRand__10GameObject(self) % 12u) - 6) + base[0];
        position[1] = static_cast<float>(static_cast<int>(eRand__10GameObject(self) % 12u) - 6) + base[1];
        position[2] = static_cast<float>(static_cast<int>(eRand__10GameObject(self) % 12u) - 6) + base[2];
        if ((levelFlags & 0x100) != 0) addEffect__FiG6Point3(17, position);
        else P_AddParticle__FP11ParticleDefP6Point3T1f(fxDust, position, velocity, 0.0f);
    }
}
