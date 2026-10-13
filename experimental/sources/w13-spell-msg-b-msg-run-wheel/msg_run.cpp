// Observed msg_run__5Wheel at 0x00277180, 376 bytes. O is the object. Names: P = the point (O+16, O+20, O+24 - float(O+376) * 39.0);
// B = the block at word O+396; i = the signed word at O+384. Steps in order:
//  1. The word at O+348 becomes the word at B+384.
//  2. r = worldCheckForWater__FP12_worldHeaderG6Point3fPf(world, P, 24.0, null); the word at O+392 becomes r. Nothing more
//     happens when r is zero.
//  3. With the three floats V at B + 12i + 1836, m = (int)sqrt(Vx*Vx + Vy*Vy + Vz*Vz) (summed in that order).
//     When m is above zero: a roll eRand__10GameObject(O) with bit 0 clear ends it; otherwise a second roll gives
//     the remainder (signed) by m, and only a non-zero remainder calls
//     worldPerturbWater__FP12_worldHeaderG6Point3ff(world, P, float(100 * m), 24.0).
//     When m is zero or less: a roll with an unsigned remainder by 20 of zero calls worldPerturbWater(world, P, 400.0, 24.0).
// Inferred: a water wheel making ripples. Unknown: the meaning of the offsets (the block at +396 is likely a shared
// description) and of the two floats 39.0 and 24.0. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern void* world;
int eRand__10GameObject(void* self);
int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void* world, Point3 position, float radius, float* result);
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, Point3 position, float a, float b);

void msg_run__5Wheel(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    Point3 point;
    point.x = real(o, 16);
    point.y = real(o, 20);
    point.z = real(o, 24) - real(o, 376) * 39.0f;
    unsigned char* const block = from_word(word(o, 396));
    word(o, 348) = word(block, 384);
    const int water = worldCheckForWater__FP12_worldHeaderG6Point3fPf(world, point, 24.0f, nullptr);
    word(o, 392) = static_cast<unsigned>(water);
    if (water == 0) return;
    unsigned char* const v = from_word(word(o, 396)) + sword(o, 384) * 12 + 1836;
    const int m = static_cast<int>(__builtin_sqrtf(real(v, 0) * real(v, 0) + real(v, 4) * real(v, 4) + real(v, 8) * real(v, 8)));
    if (m > 0) {
        if ((eRand__10GameObject(o) & 1) == 0) return;
        const int roll = eRand__10GameObject(o);
        if (roll % m == 0) return;
        worldPerturbWater__FP12_worldHeaderG6Point3ff(world, point, static_cast<float>(m * 100), 24.0f);
    } else {
        const unsigned roll = static_cast<unsigned>(eRand__10GameObject(o));
        if (roll % 20u != 0u) return;
        worldPerturbWater__FP12_worldHeaderG6Point3ff(world, point, 400.0f, 24.0f);
    }
}
}
