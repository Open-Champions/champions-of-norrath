// Observed msg_run__6PhProp at 0x00149438, 4140 bytes. o is the object; pos the three floats at +16; body the
// structure at +400 (mass float at +400); bodyPos the three floats at +416; the three floats at +240 are a
// centre offset; the rotation rows used below are floats +468..+500. Steps, in order:
//   1. When the byte at +569 is zero and frameNm & 7 is zero and worldCheckForWater__FP12_worldHeaderG6Point3fPf(
//      world, pos, 12.0, null) is non-zero: byte +569 = 1, the generator word at +68 advances by one step
//      (x * 1664525 + 1013904223), sprintf(buffer, "foly_wtrsplash_sml%d.anm", 1), the sequence named by
//      lumpFindResource("fx.lmp", buffer) is played with soundPlaySequence(header, object, 0.2) and
//      worldPerturbWater__FP12_worldHeaderG6Point3ff(world, pos, 700.0, 24.0) runs.
//   2. The float +452 loses (levelFlags & 0x100 ? 0.054 : 0.18) / float +400. A threshold is the float +552
//      plus 2.0. force and torque (three floats each) are zero, the flag `touched` is zero, the word +560 is zero.
//   3. For each set bit i of the doubleword at +232 (lowest first): hull = modelGetCollision__FP12_modelHeaderi(
//      word +76, i), a list of eight signed-byte triples. For each triple: p = (x - float +240, y - float +244,
//      z - float +248); the plane height h = ((p.x * float +492 + p.y * float +496) + p.z * float +500) + float +424;
//      triples with h >= threshold are skipped. Otherwise p is passed through matUntransform__FP6Point3P8Matrix33T0
//      (matrix at +468) and bodyPos is added. If the word +564 is non-zero it is decremented and the triple is
//      skipped; triples with p.z >= threshold are skipped. Otherwise: touched = 1; v = the velocity from
//      bodyGetVelocity__FP5_bodyP6Point3T1(body, p); with v.z below -1.0 and the byte +569 zero, one of the
//      impact sounds listed below is played (lump "fx.lmp"); with v.z below zero bodyApplyImpulse(body, p, (0,0,1),
//      float +572) and the word +560 = 1; with s = min(threshold - p.z, 1.0) * 0.006 positive,
//      bodyAddForce__FP5_bodyP6Point3N31(body, p, (0, 0, s), force, torque).
//      Impact sounds. Byte +568 zero: halfword +320 bit 3 -> "inv_drop_mdwp_wd.anm", else bit 4 ->
//      "inv_drop_mdwp_mtl.anm", else none. Byte +568 non-zero: with e the word at +328 and f the doubleword at +336
//      (e's byte +29 zero: f & 0x800000 -> wd, else f & 0x40000 -> mdwp_mtl, else none; byte +29 non-zero and
//      byte +28 = 3: f & (0x800000 | 0x40000) -> "inv_drop_sm_shield.anm"; otherwise the first of f & 0x800000 wd,
//      0x200000 "inv_drop_chain.anm", 0x40000 "inv_drop_mwdp_mtl.anm", 0x100000 "inv_drop_leather.anm", 0x80000
//      "inv_drop_cloth.anm", 0x1000000 "inv_drop_potion.anm", any of 0x4000000 / 0x2000000 / 0x10000000
//      "inv_drop_jewelry.anm").
//   4. When halfword +320 bit 5 is set the floats +456, +460, +464 are limited so that each times the
//      float at +404, +408, +412 respectively stays within +-0.05. bodyApplyForce__FP5_bodyG6Point3T1f(body, force,
//      torque, 1.0). When touched, floats +444 and +448 are scaled by 0.95; floats +456, +460, +464 by 0.95.
//      pos = bodyPos. z of bodyPos gains float +452 * float +400. A motion vector (+444 * m, +448 * m, 0) is built
//      with m = float +400. h = worldFindStandHeight__FP12_worldHeaderG6Point3f(world, pos, float +28) (called on the copy
//      of pos made before the z update); float +552 = h; float +24 = h when float +24 - 0.01 <= h.
//   5. When motion.x^2 + motion.y^2 > 0.001: k = worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(world, pos,
//      motion, float +28, 1, &n); motion *= k; pos += motion; objectUpdateInGrid; when k < 1.0 and 0.001 - (n.x *
//      float +444 + n.y * float +448) > 0, floats +444 and +448 gain n * that value * 1.5; when the byte +570 is
//      non-zero: v = body velocity at pos; fxFlyingFire__FP6Point3T0ffb(bodyPos, v, (float)(byte +40) * 0.8, 1.0, false).
//   6. bodyPos x,y = pos x,y; objectUpdateInGrid. When byte +570 is 1, |float +24 - h| < 12.0 and every one of
//      floats +456, +460, +464, +444, +448, +452 is below 0.8 in magnitude: byte +570 = 2 and a 1192-byte StaticFire
//      is built at pos (240.0, 480 + 60 * ((state >> 16) % 40) after one generator step, 0.5) when allocation worked.
//   7. The halfword +270 is the life counter: below -220 the end part runs; otherwise it is decremented, a positive
//      old value returns, and the end part runs only when floats +456, +460, +464, +444, +448, +452 are all below 0.2
//      in magnitude.
//   8. End part: the 3x3 rotation at +468 is transposed into floats +272..+304 (row r column c <- column r row c)
//      and the translation +308..+316 is bodyPos; matScale(+272, float +576); matTrans(+272, -float +240, -float +244,
//      -float +248). A replacement object is built from the dying one when allocation works: byte +568 non-zero ->
//      a 400-byte ItemProp (position, the description at +328, the matrix); otherwise byte +84 non-zero -> a 328-byte
//      GameObject with string members +84, +100, +164 copied, word +76/+80 looked up by lumpFindResource(+84 text,
//      +100 / +164 text), flags |= 0x1809 and the 4Prop table; otherwise a 328-byte GameObject taking the words +76,
//      +80 directly, flags |= 0x5809. Both copy the matrix, the doubleword +232, set halfword +320 |= 1 and float +28 =
//      4.0, add to the run list when halfword +320 has bit 1 (flags |= 0x4000), and flags |= 0x1000 when it has bit 2.
//      Finally objectAddToDeleteList(object).
// Inferred from names only: this is the physics step of a dropped prop; +568 marks a carried item; +570 a fire
// state; +569 a flag set once the prop has touched water. Unknown: most field roles.
// Nothing here ran on the EE.
#include "../w11-actor-b-common/access.hpp"

extern "C" {
extern int frameNm;
extern int levelFlags;
extern void* world;
extern unsigned char vt_4Prop[] __asm__("_vt$4Prop");
int format_text(char* destination, const char* format, ...) __asm__("sprintf");
char* copy_text(char* destination, const char* source) __asm__("strcpy");
int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void* world, Point3 position, float radius, float* out);
char* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float rate);
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* world, Point3 position, float a, float b);
signed char* modelGetCollision__FP12_modelHeaderi(void* model, int index);
void matUntransform__FP6Point3P8Matrix33T0(Point3* out, void* matrix, Point3* in);
void bodyGetVelocity__FP5_bodyP6Point3T1(void* body, Point3* position, Point3* out);
void bodyApplyImpulse__FP5_bodyG6Point3T1f(void* body, Point3* position, Point3* direction, float amount);
void bodyAddForce__FP5_bodyP6Point3N31(void* body, Point3* position, Point3* a, Point3* b, Point3* c);
void bodyApplyForce__FP5_bodyG6Point3T1f(void* body, Point3* force, Point3* torque, float amount);
float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float radius);
float worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(void* world, Point3 position, Point3* motion, float radius, int flag, Point3* out);
void fxFlyingFire__FP6Point3T0ffb(Point3* position, Point3* velocity, float a, float b, bool flag);
void objectUpdateInGrid__FP10GameObject(void* object);
void objectAddToRunList__FP10GameObject(void* object);
void objectAddToDeleteList__FP10GameObject(void* object);
void* blockAlloc__Fi(int size);
void* ctor_StaticFire(void* self, Point3 position, float a, int b, float c) __asm__("__10StaticFireG6Point3fif");
void* ctor_ItemProp(void* self, Point3 position, void* description, void* matrix) __asm__("__8ItemPropG6Point3PQ24item11descriptionP8Matrix34");
void* ctor_GameObject(void* self, Point3 position, void* map, char** names) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void matScale__FP8Matrix34f(void* matrix, float scale);
void matTrans__FP8Matrix34fff(void* matrix, float x, float y, float z);
}

namespace {
using namespace w11b;

inline void limit_component(unsigned char* o, unsigned velocityOffset, unsigned massOffset) {
    const float limit = bits(0x3d4ccccc);
    const float mass = real(o, massOffset);
    float product = real(o, velocityOffset) * mass;
    if (limit < product) {
        const float v = limit / mass;
        real(o, velocityOffset) = v;
        product = v * mass;
    }
    if (product < -limit) real(o, velocityOffset) = -limit / mass;
}

inline bool below(unsigned char* o, unsigned offset, float limit) { return __builtin_fabsf(real(o, offset)) < limit; }

inline const char* impact_sound(unsigned char* o) {
    if (*reinterpret_cast<signed char*>(o + 568) == 0) {
        const unsigned flags = static_cast<unsigned short>(half(o, 320));
        if ((flags & 8u) != 0) return "inv_drop_mdwp_wd.anm";
        if ((flags & 0x10u) != 0) return "inv_drop_mdwp_mtl.anm";
        return nullptr;
    }
    unsigned char* e = from_word(word(o, 328));
    const unsigned long long f = dword(o, 336);
    if (*reinterpret_cast<signed char*>(e + 29) == 0) {
        if ((f & 0x800000ull) != 0) return "inv_drop_mdwp_wd.anm";
        if ((f & 0x40000ull) != 0) return "inv_drop_mdwp_mtl.anm";
        return nullptr;
    }
    if (*reinterpret_cast<signed char*>(e + 28) == 3) {
        if ((f & 0x800000ull) != 0 || (f & 0x40000ull) != 0) return "inv_drop_sm_shield.anm";
        return nullptr;
    }
    if ((f & 0x800000ull) != 0) return "inv_drop_mdwp_wd.anm";
    if ((f & 0x200000ull) != 0) return "inv_drop_chain.anm";
    if ((f & 0x40000ull) != 0) return "inv_drop_mwdp_mtl.anm";
    if ((f & 0x100000ull) != 0) return "inv_drop_leather.anm";
    if ((f & 0x80000ull) != 0) return "inv_drop_cloth.anm";
    if ((f & 0x1000000ull) != 0) return "inv_drop_potion.anm";
    if ((f & 0x4000000ull) != 0 || (f & 0x2000000ull) != 0 || (f & 0x10000000ull) != 0) return "inv_drop_jewelry.anm";
    return nullptr;
}

inline void finish_copy(unsigned char* n, unsigned char* o, unsigned flagsToAdd, short flags320, unsigned long long mask232) {
    word(n, 12) |= flagsToAdd;
    word(n, 72) = to_word(vt_4Prop);
    objectUpdateInGrid__FP10GameObject(n);
    half(n, 320) = static_cast<short>(flags320 | 1);
    real(n, 28) = 4.0f;
    for (int i = 0; i < 12; ++i) real(n, 272 + 4 * i) = real(o, 272 + 4 * i);
    (void)mask232;
}

inline void register_copy(unsigned char* n) {
    if ((half(n, 320) & 2) != 0) {
        objectAddToRunList__FP10GameObject(n);
        word(n, 12) |= 0x4000;
    }
    if ((half(n, 320) & 4) != 0) word(n, 12) |= 0x1000;
}
}  // namespace

extern "C" void msg_run__6PhProp(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    Point3* pos = reinterpret_cast<Point3*>(o + 16);
    unsigned char* body = o + 400;
    Point3* bodyPos = reinterpret_cast<Point3*>(o + 416);
    if (*reinterpret_cast<signed char*>(o + 569) == 0 && (frameNm & 7) == 0) {
        if (worldCheckForWater__FP12_worldHeaderG6Point3fPf(world, *pos, bits(0x41400000), nullptr) != 0) {
            char buffer[128];
            *(o + 569) = 1;
            word(o, 68) = word(o, 68) * 0x19660du + 0x3c6ef35fu;
            format_text(buffer, "foly_wtrsplash_sml%d.anm", 1);
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), buffer), o, bits(0x3e4ccccc));
            worldPerturbWater__FP12_worldHeaderG6Point3ff(world, *pos, bits(0x442f0000), bits(0x41c00000));
        }
    }
    real(o, 452) = real(o, 452) - ((levelFlags & 0x100) != 0 ? bits(0x3d5d2f18) : bits(0x3e3851e9)) / real(o, 400);
    const float threshold = real(o, 552) + 2.0f;
    Point3 force = {0.0f, 0.0f, 0.0f};
    Point3 torque = {0.0f, 0.0f, 0.0f};
    int touched = 0;
    word(o, 560) = 0;
    unsigned long long mask = dword(o, 232);
    int index = 0;
    if (mask != 0) {
        do {
            if ((static_cast<int>(mask) & 1) != 0) {
                signed char* hull = modelGetCollision__FP12_modelHeaderi(from_word(word(o, 76)), index);
                for (int corner = 7; corner >= 0; --corner, hull += 3) {
                    Point3 p;
                    p.x = static_cast<float>(hull[0]) - real(o, 240);
                    p.y = static_cast<float>(hull[1]) - real(o, 244);
                    p.z = static_cast<float>(hull[2]) - real(o, 248);
                    const float plane = p.x * real(o, 492) + p.y * real(o, 496) + p.z * real(o, 500) + real(o, 424);
                    if (threshold <= plane) continue;
                    matUntransform__FP6Point3P8Matrix33T0(&p, o + 468, &p);
                    p.x = p.x + bodyPos->x;
                    p.y = p.y + bodyPos->y;
                    p.z = p.z + bodyPos->z;
                    if (word(o, 564) != 0) {
                        word(o, 564) = word(o, 564) - 1;
                        continue;
                    }
                    if (!(p.z < threshold)) continue;
                    Point3 normal;
                    normal.x = 0.0f;
                    normal.y = 0.0f;
                    touched = 1;
                    normal.z = 1.0f;
                    Point3 velocity;
                    bodyGetVelocity__FP5_bodyP6Point3T1(body, &p, &velocity);
                    const float vz = velocity.z;
                    if (vz < -1.0f && *reinterpret_cast<signed char*>(o + 569) == 0) {
                        const char* name = impact_sound(o);
                        if (name != nullptr)
                            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>(name)), o, 1.0f);
                    }
                    if (vz < 0.0f) {
                        bodyApplyImpulse__FP5_bodyG6Point3T1f(body, &p, &normal, real(o, 572));
                        word(o, 560) = 1;
                    }
                    float depth = threshold - p.z;
                    if (1.0f < depth) depth = 1.0f;
                    const float push = depth * bits(0x3bc49ba5);
                    if (0.0f < push) {
                        Point3 lift;
                        lift.x = 0.0f;
                        lift.y = 0.0f;
                        lift.z = push;
                        bodyAddForce__FP5_bodyP6Point3N31(body, &p, &lift, &force, &torque);
                    }
                }
            }
            mask >>= 1;
            ++index;
        } while (mask != 0);
    }
    if ((static_cast<unsigned short>(half(o, 320)) & 0x20u) != 0) {
        limit_component(o, 456, 404);
        limit_component(o, 460, 408);
        limit_component(o, 464, 412);
    }
    bodyApplyForce__FP5_bodyG6Point3T1f(body, &force, &torque, 1.0f);
    if (touched != 0) {
        real(o, 444) = real(o, 444) * bits(0x3f733333);
        real(o, 448) = real(o, 448) * bits(0x3f733333);
    }
    real(o, 456) = real(o, 456) * bits(0x3f733333);
    real(o, 460) = real(o, 460) * bits(0x3f733333);
    real(o, 464) = real(o, 464) * bits(0x3f733333);
    *pos = *bodyPos;
    const float mass = real(o, 400);
    Point3 motion;
    motion.x = real(o, 444) * mass;
    motion.y = real(o, 448) * mass;
    bodyPos->z = bodyPos->z + real(o, 452) * mass;
    motion.z = 0.0f;
    const float height = worldFindStandHeight__FP12_worldHeaderG6Point3f(world, *pos, real(o, 28));
    real(o, 552) = height;
    if (real(o, 24) - bits(0x3c23d70a) <= height) real(o, 24) = height;
    if (bits(0x3a83126e) < motion.x * motion.x + motion.y * motion.y) {
        Point3 hit;
        const float fraction = worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3(world, *pos, &motion, real(o, 28), 1, &hit);
        motion.x = motion.x * fraction;
        motion.y = motion.y * fraction;
        motion.z = motion.z * fraction;
        pos->x = pos->x + motion.x;
        pos->y = pos->y + motion.y;
        pos->z = pos->z + motion.z;
        objectUpdateInGrid__FP10GameObject(o);
        if (fraction < 1.0f) {
            const float along = bits(0x3a83126e) - (hit.x * real(o, 444) + hit.y * real(o, 448));
            if (0.0f < along) {
                real(o, 448) = real(o, 448) + hit.y * along * 1.5f;
                real(o, 444) = real(o, 444) + hit.x * along * 1.5f;
            }
        }
        if (*reinterpret_cast<signed char*>(o + 570) != 0) {
            Point3 velocity;
            bodyGetVelocity__FP5_bodyP6Point3T1(body, pos, &velocity);
            fxFlyingFire__FP6Point3T0ffb(bodyPos, &velocity, static_cast<float>(o[40]) * bits(0x3f4ccccc), 1.0f, false);
        }
    }
    bodyPos->x = pos->x;
    bodyPos->y = pos->y;
    objectUpdateInGrid__FP10GameObject(o);
    if (*reinterpret_cast<signed char*>(o + 570) == 1) {
        if (__builtin_fabsf(real(o, 24) - height) < 12.0f && below(o, 456, bits(0x3f4ccccc)) && below(o, 460, bits(0x3f4ccccc)) &&
            below(o, 464, bits(0x3f4ccccc)) && below(o, 444, bits(0x3f4ccccc)) && below(o, 448, bits(0x3f4ccccc)) &&
            below(o, 452, bits(0x3f4ccccc))) {
            *(o + 570) = 2;
            void* block = blockAlloc__Fi(1192);
            if (block != nullptr) {
                word(o, 68) = word(o, 68) * 0x19660du + 0x3c6ef35fu;
                ctor_StaticFire(block, *pos, 240.0f, 480 + 60 * static_cast<int>((word(o, 68) >> 16) % 40u), 0.5f);
            }
        }
    }
    const int life = half(o, 270);
    if (life >= -220) {
        half(o, 270) = static_cast<short>(life - 1);
        if (life > 0) return;
        const float quiet = bits(0x3e4ccccc);
        if (!below(o, 456, quiet) || !below(o, 460, quiet) || !below(o, 464, quiet) || !below(o, 444, quiet) || !below(o, 448, quiet) ||
            !below(o, 452, quiet))
            return;
    }
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c) real(o, 272 + 4 * (3 * r + c)) = real(o, 468 + 4 * (r + 3 * c));
    real(o, 308) = real(o, 416);
    real(o, 312) = real(o, 420);
    real(o, 316) = real(o, 424);
    unsigned char* matrix = o + 272;
    matScale__FP8Matrix34f(matrix, real(o, 576));
    matTrans__FP8Matrix34fff(matrix, -real(o, 240), -real(o, 244), -real(o, 248));
    if (*reinterpret_cast<signed char*>(o + 568) != 0) {
        void* block = blockAlloc__Fi(400);
        if (block != nullptr) ctor_ItemProp(block, *pos, o + 328, matrix);
    } else if (*reinterpret_cast<signed char*>(o + 84) != 0) {
        unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(328));
        if (n != nullptr) {
            const unsigned long long mask232 = dword(o, 232);
            const short flags320 = half(o, 320);
            ctor_GameObject(n, *pos, nullptr, nullptr);
            finish_copy(n, o, 0x1809, flags320, mask232);
            copy_text(reinterpret_cast<char*>(n + 84), reinterpret_cast<char*>(o + 84));
            copy_text(reinterpret_cast<char*>(n + 100), reinterpret_cast<char*>(o + 100));
            copy_text(reinterpret_cast<char*>(n + 164), reinterpret_cast<char*>(o + 164));
            word(n, 76) = to_word(lumpFindResource__FPcT0(reinterpret_cast<char*>(n + 84), reinterpret_cast<char*>(n + 100)));
            word(n, 80) = to_word(lumpFindResource__FPcT0(reinterpret_cast<char*>(n + 84), reinterpret_cast<char*>(n + 164)));
            dword(n, 232) = mask232;
            register_copy(n);
        }
    } else {
        unsigned char* n = static_cast<unsigned char*>(blockAlloc__Fi(328));
        if (n != nullptr) {
            const unsigned first = word(o, 76);
            const unsigned second = word(o, 80);
            const unsigned long long mask232 = dword(o, 232);
            const short flags320 = half(o, 320);
            ctor_GameObject(n, *pos, nullptr, nullptr);
            finish_copy(n, o, 0x5809, flags320, mask232);
            word(n, 76) = first;
            word(n, 80) = second;
            dword(n, 232) = mask232;
            register_copy(n);
        }
    }
    objectAddToDeleteList__FP10GameObject(o);
}
