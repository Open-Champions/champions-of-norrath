// Observed msg_run__11FireBomblet at 0x00289fa0, 2368 bytes. Argument: the object o. Nothing is returned.
// Names: pos = floats at o+16, o+20, o+24 (z is at +24); vel = floats at +76, +80, +84; r = float at +28;
// L = the Light member at o+88 (its floats at +28, +32, +36, +40 are o+116..o+128); the signed bytes at +152,
// +153, +154 and +156 are called A, B, C and D; id = the signed word at +148. Float constants read through the
// global pointer, none with a symbol: G = -0.1097 (0xbde0bb27 at 0x00482594), V = -7.034 (0xc0e1163b at
// 0x00482598), S = 0.1422 (0x3e119451 at 0x0048259c) and F = 1.736e-05 (0x3791a2b3 at 0x004825a0). "bounced",
// "remove" and "stopped" are locals that start false.
//  1. When D is zero:
//     a. When id is -1, gamePositionVisible(pos, 960.0) runs and id is read again.
//     b. pos += vel; the unsigned halfwords at +168 and +170 are added and stored back at +168.
//     c. vz = vz + G is stored; when it is below V, V is stored instead and stopped = (C != 0).
//     d. When id is not -1: soundChangeSequenceVolume(id, min(1.0, |vel| * S)).
//     e. objectUpdateInGrid(o); pos is copied to o+136; lightUpdatePosition(L).
//     f. When A is zero it becomes 1 if vz < 0; when it is still zero nothing else of this part runs.
//     g. Unless stopped (and A is nonzero): h = worldFindHeight(world, pos, 4.0); P = h + r. When z <= P:
//        a speed |vel| below 1.0 sets remove; otherwise, when P - z is above 18.0, vx and vy are negated;
//        then z = P, bounced = true, the word at +160 gets P - r, and bounce(o) runs.
//        Then, when (frameNm & 7) == 0: the objects in the box pos +- r (objectFindInBox, capacity 64) each
//        receive gameAttackResolver(target, o, 2.0, 2.0, 100000, 0, 0, 0x100c040, &description, false) unless the
//        target is o; description is a stack item description with its halfwords at +16..+22 and +24..+30
//        zeroed, its bytes at +32 and +33 set to -1, and four 8-byte slots at +36 whose first halfword is -1
//        except the first free one (or one equal to 21), which becomes {21, 1, 1, 0}.
//  2. When A is nonzero and z < the float at +164, remove is set.
//  3. When remove is set: objectAddToDeleteList(o); when id >= 0, soundStopSound(id); return.
//  4. fxFlyingFire(pos, vel, (float)(unsigned byte at +40), 1.0, false). When bounced:
//     a. n = 309 and size = 14.0 when C < 2, else n = 77 and size = 3.5. When cutMode is nonzero the word at +68
//        is set to 0x246553.
//     b. n times: a unit-ball vector u (rejection from three eRand draws, the shared unit_ball) times size gives c,
//        with c.z replaced by -c.z when negative; a second unit-ball vector w gives the point pos + w * 12.0;
//        P_AddParticle(sparkParticle, point, c, 0.0).
//     c. When C is 1: ss1 = syncRandState * 0x19660d + 0x3c6ef35f. When bits 16..17 of ss1 are zero and cutMode
//        is zero, ss1 is stored and nothing is spawned. Otherwise ss2 = ss1 * 0x19660d + 0x3c6ef35f is
//        stored, m = 5 when cutMode is nonzero else ((ss2 >> 16) mod 3) + 2, and m times a 112-byte block is
//        allocated and, if it succeeded, the FireBombletJr constructor runs on it with o; then a third step
//        ss3 is stored and the 64-bit word at +176 becomes 1 << ((ss3 >> 16) & 3). The halfword at +170
//        becomes 2615 in both cases.
//     d. When C is not 1, the halfword at +170 is halved (rounded toward zero).
//  5. Whether or not bounced is set: when D is zero and B is zero and bounced, B becomes 1 and L's floats at
//     +28 and +36 become 240.0 and those at +32 and +40 become F. When B is nonzero and the float at +116 is
//     above 24.0, t = it - 12.0 is stored at +116 and +124 and 1.0/(t*t) at +120 and +128.
// Inferred: this is the per-frame handler of a thrown fire bomb, from its symbol; the balls spawned are
// FireBombletJr objects. Unknown: the meaning of A, B, C, D, id, the fields at +164 and +176, and the constants.
// Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
extern void* world;
extern int frameNm;
extern signed char cutMode;
extern unsigned syncRandState;
extern unsigned char sparkParticle[];
unsigned eRand__10GameObject(void* self);
void gamePositionVisible__FP6Point3f(Point3* position, float radius);
void soundChangeSequenceVolume__Fif(int id, float volume);
void soundStopSound__Fi(int id);
void objectUpdateInGrid__FP10GameObject(void* object);
void lightUpdatePosition__FP5Light(void* light);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float f);
void bounce__11FireBomblet(void* self);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description,
                                                                      bool flag);
void objectAddToDeleteList__FP10GameObject(void* object);
void fxFlyingFire__FP6Point3T0ffb(Point3* position, Point3* velocity, float a, float b, bool flag);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float age);
void* blockAlloc__Fi(int size);
void FireBombletJr_construct(void* self, void* parent) __asm__("__13FireBombletJrP11FireBomblet");

void msg_run__11FireBomblet(void* self) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    auto rand = [](unsigned char* object) { return eRand__10GameObject(object); };
    bool bounced = false, remove = false, stopped = false;
    int id = 0;
    if (*reinterpret_cast<signed char*>(o + 156) == 0) {
        id = sword(o, 148);
        if (id == -1) {
            gamePositionVisible__FP6Point3f(reinterpret_cast<Point3*>(o + 16), 960.0f);
            id = sword(o, 148);
        }
        real(o, 16) = real(o, 16) + real(o, 76);
        real(o, 20) = real(o, 20) + real(o, 80);
        half(o, 168) = static_cast<short>(static_cast<unsigned short>(half(o, 168)) + static_cast<unsigned short>(half(o, 170)));
        real(o, 24) = real(o, 24) + real(o, 84);
        real(o, 84) = real(o, 84) + bits(0xbde0bb27u);
        if (real(o, 84) < bits(0xc0e1163bu)) {
            real(o, 84) = bits(0xc0e1163bu);
            stopped = *reinterpret_cast<signed char*>(o + 154) != 0;
        }
        if (id != -1) {
            const float speed = __builtin_sqrtf(real(o, 76) * real(o, 76) + real(o, 80) * real(o, 80) + real(o, 84) * real(o, 84));
            float volume = speed * bits(0x3e119451u);
            if (1.0f < volume) volume = 1.0f;
            soundChangeSequenceVolume__Fif(id, volume);
        }
        objectUpdateInGrid__FP10GameObject(o);
        *reinterpret_cast<Point3*>(o + 136) = *reinterpret_cast<Point3*>(o + 16);
        lightUpdatePosition__FP5Light(o + 88);
        if (*reinterpret_cast<signed char*>(o + 152) == 0 && real(o, 84) < 0.0f) o[152] = 1;
        if (o[152] != 0 && !stopped) {
            const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, *reinterpret_cast<Point3*>(o + 16), 4.0f);
            const float top = height + real(o, 28);
            if (real(o, 24) <= top) {
                const float speed = __builtin_sqrtf(real(o, 76) * real(o, 76) + real(o, 80) * real(o, 80) + real(o, 84) * real(o, 84));
                if (speed < 1.0f) {
                    remove = true;
                } else {
                    if (bits(0x41900000u) < top - real(o, 24)) {
                        real(o, 76) = -real(o, 76);
                        real(o, 80) = -real(o, 80);
                    }
                    real(o, 24) = top;
                    bounced = true;
                    real(o, 160) = top - real(o, 28);
                    bounce__11FireBomblet(o);
                }
            }
            if ((frameNm & 7) == 0) {
                const float r = real(o, 28);
                const Point3 low = {real(o, 16) - r, real(o, 20) - r, real(o, 24) - r};
                const Point3 high = {real(o, 16) + r, real(o, 20) + r, real(o, 24) + r};
                void* found[64];
                objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 64);
                unsigned char description[80];
                for (int k = 0; k < 4; ++k) half(description, 60 - 8 * k) = -1;
                description[33] = 0xff;
                description[32] = 0xff;
                for (int k = 0; k < 4; ++k) {
                    half(description, 16 + 2 * k) = 0;
                    half(description, 24 + 2 * k) = 0;
                }
                for (int k = 0; k < 4; ++k) {
                    unsigned char* const slot = description + 36 + 8 * k;
                    if (half(slot, 0) == -1 || half(slot, 0) == 21) {
                        half(slot, 0) = 21;
                        half(slot, 6) = 0;
                        half(slot, 2) = 1;
                        half(slot, 4) = 1;
                        break;
                    }
                }
                for (void** target = found; *target != nullptr; ++target)
                    if (*target != self)
                        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(*target, self, 2.0f, 2.0f, 100000, 0, 0, 0x100c040, description, false);
            }
        }
    }
    if (o[152] != 0 && real(o, 24) < real(o, 164)) remove = true;
    if (remove) {
        objectAddToDeleteList__FP10GameObject(o);
        id = sword(o, 148);
        if (id >= 0) soundStopSound__Fi(id);
        return;
    }
    fxFlyingFire__FP6Point3T0ffb(reinterpret_cast<Point3*>(o + 16), reinterpret_cast<Point3*>(o + 76), static_cast<float>(o[40]), 1.0f, false);
    if (bounced) {
        int count = 309;
        float size = bits(0x41600000u);
        if (*reinterpret_cast<signed char*>(o + 154) >= 2) {
            count = 77;
            size = bits(0x40600000u);
        }
        if (cutMode != 0) word(o, 68) = 0x246553u;
        for (int i = 0; i < count; ++i) {
            Point3 unit;
            unit_ball(o, unit, rand);
            Point3 velocity = unit;
            velocity.x = velocity.x * size;
            velocity.y = velocity.y * size;
            velocity.z = velocity.z * size;
            if (velocity.z < 0.0f) velocity.z = -velocity.z;
            Point3 offset;
            unit_ball(o, offset, rand);
            offset.x = offset.x * bits(0x41600000u);
            offset.y = offset.y * bits(0x41600000u);
            offset.z = offset.z * bits(0x41600000u);
            Point3 point = {real(o, 16) + offset.x, real(o, 20) + offset.y, real(o, 24) + offset.z};
            P_AddParticle__FP11ParticleDefP6Point3T1f(sparkParticle, &point, &velocity, 0.0f);
        }
        if (*reinterpret_cast<signed char*>(o + 154) == 1) {
            const unsigned first = syncRandState * 0x19660du + 0x3c6ef35fu;
            if (((first >> 16) & 3u) == 0 && cutMode == 0) {
                syncRandState = first;
            } else {
                const unsigned second = first * 0x19660du + 0x3c6ef35fu;
                unsigned m = (cutMode != 0) ? 5u : ((second >> 16) % 3u) + 2u;
                syncRandState = second;
                if (m != 0) {
                    do {
                        void* const block = blockAlloc__Fi(112);
                        if (block != nullptr) FireBombletJr_construct(block, self);
                        --m;
                    } while (m != 0);
                }
                const unsigned third = syncRandState * 0x19660du + 0x3c6ef35fu;
                syncRandState = third;
                dword(o, 176) = static_cast<unsigned long long>(static_cast<long long>(static_cast<int>(1u << ((third >> 16) & 3u))));
            }
            half(o, 170) = 2615;
        } else {
            half(o, 170) = static_cast<short>(half(o, 170) / 2);
        }
    }
    if (*reinterpret_cast<signed char*>(o + 156) == 0) {
        if (*reinterpret_cast<signed char*>(o + 153) == 0) {
            if (bounced) {
                real(o, 124) = bits(0x43700000u);
                real(o, 128) = bits(0x3791a2b3u);
                real(o, 116) = bits(0x43700000u);
                real(o, 120) = bits(0x3791a2b3u);
                o[153] = 1;
            }
        } else if (bits(0x41c00000u) < real(o, 116)) {
            const float t = real(o, 116) - bits(0x41400000u);
            const float square = t * t;
            real(o, 116) = t;
            real(o, 124) = t;
            real(o, 128) = 1.0f / square;
            real(o, 120) = 1.0f / square;
        }
    }
}
}
