// Observed msg_run__11BoltGoRound at 0x001e9528, 1000 bytes. O is the object, frameNm the global frame counter, E the
// signed word at O+80 (an end frame), R the single at O+84 and n the signed halfword at O+76. Three pool constants are read
// from the image and have no symbols: K1 = bits 0x3f8ccccc (1.1), K2 = bits 0x3e999999 (0.3) and the constants 500.0,
// 15.0 and 0.0 are immediates.
//  1. R changes: while frameNm < E, R becomes R + 15.0 when R < 500.0 (unchanged otherwise); otherwise R becomes R - 15.0,
//     and 0.0 if that is negative.
//  2. With i starting at (frameNm & 1) and rising by 2 while i < n, a bolt i is processed. P is the point (three floats) at
//     O+16, T the point at O+104+12*i, active = (frameNm < E) read again for each bolt:
//       d = T - P (x, y, z); lim = min(R, 500.0); unit = d * rsqrt(1.0, (d.x*d.x + d.y*d.y) + d.z*d.z) (the rsqrt.s
//       instruction, no zero test); B = unit * lim; C = P + B.
//       When active: hit = null; r = worldHitscanWithObjects(world, &P-copy, &B, K1, O, &hit, 0, 0). When hit is non-null, its
//       word at +12 has bit 0x40 and r < 1.0: C becomes the point at hit+16; an item description is built on the stack
//       (see spell.hpp) and its first entry whose first halfword is -1 or 24 gets 24, k, k and 0 where
//       k = (int)(gameDifficultyScale() * K2); then gameAttackResolver(hit, null, 0.0, 0.0, 100000, 0, 0, 64, description,
//       false) runs.
//       Whether or not active: P_MoveLightning__FiG6Point3T1ii(the word at O+88+4*i, &P-copy, &C, 94, -1) runs.
//  3. After the loop (also when it did not run): when E < frameNm and R == 0.0: soundStopSound__Fi(the word at O+152) and the
//     entry at offset 8 of O's table is called on O (adjusted by the entry's signed halfword) with the argument 3.
// Inferred: a set of lightning bolts from the object to n target points that fades in and out, hurting players it touches
// (the 0x40 flag test is a guess at "player"). Unknown: what E, R, the target array and the words at O+88+4*i and +152 mean.
// Nothing here ran on the EE.
#include "../w13-spell-msg-a-common/spell.hpp"

extern "C" {
extern int frameNm;
extern void* world;
float worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(
    void* world, const Point3* from, const Point3* delta, float range, void* ignore, void** hit, int a, int b);
void P_MoveLightning__FiG6Point3T1ii(int handle, const Point3* from, const Point3* to, int a, int b);
float gameDifficultyScale__Fv();
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
void soundStopSound__Fi(int channel);

static inline float rsqrt_s(float numerator, float value) {
#if defined(__mips__)
    float out;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(numerator), "f"(value));
    return out;
#else
    return numerator / __builtin_sqrtf(value);
#endif
}

void msg_run__11BoltGoRound(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (frameNm < sword(o, 80)) {
        if (real(o, 84) < 500.0f) real(o, 84) = real(o, 84) + 15.0f;
    } else {
        const float lowered = real(o, 84) - 15.0f;
        real(o, 84) = lowered;
        if (lowered < 0.0f) real(o, 84) = 0.0f;
    }
    for (int i = frameNm & 1; i < half(o, 76); i += 2) {
        const bool active = frameNm < sword(o, 80);
        Point3 position = *reinterpret_cast<Point3*>(o + 16);
        const Point3 target = *reinterpret_cast<Point3*>(o + 104 + 12 * i);
        Point3 d;
        d.x = target.x - position.x;
        d.y = target.y - position.y;
        d.z = target.z - position.z;
        const float reach = real(o, 84);
        const float lim = reach < 500.0f ? reach : 500.0f;
        const float inverse = rsqrt_s(1.0f, d.x * d.x + d.y * d.y + d.z * d.z);
        d.x = d.x * inverse;
        d.y = d.y * inverse;
        d.z = d.z * inverse;
        Point3 b;
        b.x = d.x * lim;
        b.y = d.y * lim;
        b.z = d.z * lim;
        Point3 c;
        c.x = position.x + b.x;
        c.y = position.y + b.y;
        c.z = position.z + b.z;
        if (active) {
            void* hit = nullptr;
            const float r = worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii(
                world, &position, &b, bits(0x3f8ccccc), o, &hit, 0, 0);
            if (hit != nullptr && (word(static_cast<unsigned char*>(hit), 12) & 0x40u) && r < 1.0f) {
                c = *reinterpret_cast<Point3*>(static_cast<unsigned char*>(hit) + 16);
                unsigned char description[80];
                w13a::init_description(description);
                const int k = static_cast<int>(gameDifficultyScale__Fv() * bits(0x3e999999));
                w13a::fill_description(description, 24, static_cast<short>(k), static_cast<short>(k), 0);
                gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(hit, nullptr, 0.0f, 0.0f, 100000, 0, 0, 64, description, false);
            }
        }
        P_MoveLightning__FiG6Point3T1ii(sword(o, 88 + 4 * i), &position, &c, 94, -1);
    }
    if (sword(o, 80) < frameNm && real(o, 84) == 0.0f) {
        soundStopSound__Fi(sword(o, 152));
        call_entry(o, 8, 3);
    }
}
}
