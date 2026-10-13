// Observed msg_run__13FireBombletJr at 0x0028b0b8, 944 bytes. Argument: the object o. Nothing is returned.
// Names: pos = floats at o+16, o+20, o+24 (z at +24); vel = floats at +76, +80, +84; r = float at +28; C = signed byte at +88;
// the unsigned halfwords at +90 and +92 are called A and B; M = float at +96; world and frameNm are the image globals. Float
// constants read through the global pointer, none with a symbol: G = -0.1097 (0xbde0bb27 at 0x004825b0) and V = -7.034
// (0xc0e1163b at 0x004825b4); the others are immediates. "bounced", "remove" and "stopped" are locals that start false.
//  1. pos += vel. vz = vz + G is stored; when that is below V, V is stored instead and stopped = (C != 0).
//  2. objectUpdateInGrid__FP10GameObject(o) runs; A = A + B (16 bits).
//  3. Unless stopped: h = worldFindHeight__FP12_worldHeaderG6Point3f(world, copy of pos, 4.0); P = h + r. When z <= P: a speed
//     sqrt((vx*vx + vy*vy) + vz*vz) below 1.0 sets remove; otherwise, when P - z is above 18.0, vx and vy are negated; then z = P,
//     bounced = true and bounce__13FireBombletJr(o) runs. Then, whether or not z <= P, when (frameNm & 7) == 0 the objects in the
//     box pos +- r (the z used is the one after any bounce; objectFindInBox, capacity 64) each receive
//     gameAttackResolver(target, o, 1.0, 1.0, 100000, 0, 0, 0x100c040, &description, false) unless the target is o; description
//     is a stack item description with its halfwords at +16..+22 and +24..+30 zeroed, its bytes at +32 and +33 set to -1 and four
//     8-byte slots at +36 whose first halfword is -1 except the first free one (or one equal to 21), which becomes {21, 1, 1, 0}.
//  4. When z < M, remove is set. When remove is set, objectAddToDeleteList__FP10GameObject(o) runs and the function returns.
//  5. fxFlyingFire__FP6Point3T0ffb(pos, vel, (float)(unsigned byte at +40), 0.25, false) runs. When bounced, B is halved
//     (signed, rounded toward zero).
// Inferred: the per-frame handler of the small fire balls a FireBomblet throws off, from its symbol. Unknown: the meaning of C, A,
// B and M and of the constants. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern void* world;
extern int frameNm;
void objectUpdateInGrid__FP10GameObject(void* object);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float f);
void bounce__13FireBombletJr(void* self);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description,
                                                                      bool flag);
void objectAddToDeleteList__FP10GameObject(void* object);
void fxFlyingFire__FP6Point3T0ffb(Point3* position, Point3* velocity, float a, float b, bool flag);

void msg_run__13FireBombletJr(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    bool bounced = false, remove = false, stopped = false;
    real(o, 16) = real(o, 16) + real(o, 76);
    real(o, 20) = real(o, 20) + real(o, 80);
    real(o, 24) = real(o, 24) + real(o, 84);
    real(o, 84) = real(o, 84) + bits(0xbde0bb27u);
    if (real(o, 84) < bits(0xc0e1163bu)) {
        real(o, 84) = bits(0xc0e1163bu);
        stopped = *reinterpret_cast<signed char*>(o + 88) != 0;
    }
    objectUpdateInGrid__FP10GameObject(o);
    half(o, 90) = static_cast<short>(static_cast<unsigned short>(half(o, 90)) + static_cast<unsigned short>(half(o, 92)));
    if (!stopped) {
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
                bounce__13FireBombletJr(o);
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
                    gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(*target, self, 1.0f, 1.0f, 100000, 0, 0, 0x100c040, description, false);
        }
    }
    if (real(o, 24) < real(o, 96)) remove = true;
    if (remove) {
        objectAddToDeleteList__FP10GameObject(o);
        return;
    }
    fxFlyingFire__FP6Point3T0ffb(reinterpret_cast<Point3*>(o + 16), reinterpret_cast<Point3*>(o + 76), static_cast<float>(o[40]), 0.25f, false);
    if (bounced) half(o, 92) = static_cast<short>(half(o, 92) / 2);
}
}
