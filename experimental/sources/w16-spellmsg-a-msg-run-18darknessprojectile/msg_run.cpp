// Observed msg_run__18DarknessProjectile at 0x002979b0, 1824 bytes. O is the object, P the point (three singles) at O+16, V the point at
// O+76, M the signed byte at O+160, frameNm the global frame counter and world the global world pointer. H is the object that is the word at
// O+168 and C = the word at H+12 (read each time it is used); W is the object that is the word at O+172 and s the signed word at O+152.
// "e(T, off)" is the entry at table offset off of the table whose address is the word at T+72 (for an object T, called on T adjusted by the
// entry's signed halfword); "w(off, ...)" is the entry at offset off of the table at (the word at W+96), called on W the same way. Pool
// constants with no symbols (bits): 6.0 (0x41c00000), 12.0 (0x41400000), 36.0 (0x42100000) and k = 0xbea22221 (about -0.3167), n = 0x3e4ccccc
// (about 0.2). A "unit-ball vector" is drawn by rejection: each component is eRand__10GameObject(O) * 2^-16 doubled minus 1.0 (x, y, z in
// that order, each call separate), repeated while 1.0 < (x*x + y*y) + z*z. The particle definition is an 80-byte bss object of the image
// (file-local symbol darkParticle 0x008d6b40), imported through an address-named view. "Candidates" are the objects returned by
// objectFindInBox__FG6Point3T0PP10GameObjecti(P - 6.0, P + 6.0 (each component), list, 128): the list is a null-terminated array that this
// call fills; nothing is searched when H is zero.
//  Mode M == 1: only when frameNm % 30 == 0 (signed remainder). For each candidate X, in order: skipped unless e(C, 160)(X) is nonzero, the
//    single at X+560 is above 0.0 and the halfword at X+796 is zero; then, with a = w(88)(0, s), a roll R % 100 (unsigned) + 1 must be below
//    a, and then b = w(88)(3, s) gives the damage d = 60 * b, halved (arithmetic shift) plus 1 when isBoss__FP10GameObject(X) is nonzero, and
//    e(X, 216)(d) is called on X.
//  Any other M: when the word at O+156 is nonzero, V is added to P component by component, and worldFindHeight__FP12_worldHeaderG6Point3f(world,
//    copy of P, 6.0) is asked; when the single at O+24 is below it the word at O+156 becomes 0. Then candidates are searched around the (updated)
//    position and for each X, in order: skipped unless e(C, 160)(X) is nonzero; when M is zero: m = w(88)(2, s), M becomes 1, the word at O+148
//    becomes 60 * m and P becomes X's point (at X+16) plus (0, 0, 36.0); then, whatever M was, e(C, 160)(X) is called again and, when it
//    is nonzero, w(72)(1, s, 0, &p, &q) is called with two out words and gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
//    X, the word at H+12, (float)p, (float)q, 100000, 0, 0, 64, null, false) runs.
//  Every mode ends with: P copied to O+136; lightUpdatePosition__FP5Light(O+88); eight times a unit-ball vector u gives the velocity V + u * k
//    and the position P + (u + u) * 12.0, and P_AddParticle__FP11ParticleDefP6Point3T1f(definition, position, velocity, 0.0) runs; then the
//    word at O+148 drops by 1 and, when it becomes -1, objectAddToDeleteList__FP10GameObject(O) and lightDeactivate__FP5Light(O+88) run.
// Inferred: a dark projectile that arms on the first creature it touches, then drains and damages creatures near it while a countdown
// runs down. Unknown: what the words at O+148..O+172 and the entries 72/88/160/216 mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char norrath_w16_spellmsg_a_view_8d6b40[80];
int eRand__10GameObject(void* object);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3* low, Point3* high, unsigned* list, int capacity);
int isBoss__FP10GameObject(void* object);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float radius);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
void lightUpdatePosition__FP5Light(void* light);
void lightDeactivate__FP5Light(void* light);
void objectAddToDeleteList__FP10GameObject(void* object);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float f);
}

namespace {
int roll(unsigned char* o) { return eRand__10GameObject(o); }

unsigned char* entry_of(unsigned char* t, unsigned offset) { return w12c::from_word(w12c::word(t, 72)) + offset; }

// Entry call on an object with (a1) and (a1, a2); the table is the word at +72.
unsigned call160(unsigned char* c, unsigned char* x) {
    unsigned char* const e = entry_of(c, 160);
    return reinterpret_cast<unsigned (*)(void*, unsigned)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(e + 4)))(
        c + *reinterpret_cast<short*>(e), w12c::to_word(x));
}
// Entry on W (table = the word at W+96) with (a1, a2).
int w_call(unsigned char* w, unsigned offset, int a1, int a2) {
    unsigned char* const e = w12c::from_word(w12c::word(w, 96)) + offset;
    return reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(e + 4)))(w + *reinterpret_cast<short*>(e), a1, a2);
}
}

extern "C" void msg_run__18DarknessProjectile(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const float six = bits(0x41c00000u);
    const int s = sword(o, 152);
    unsigned list[128];
    if (static_cast<signed char>(o[160]) == 1) {
        if (frameNm % 30 == 0) {
            const float x = real(o, 16);
            const float y = real(o, 20);
            const float z = real(o, 24);
            Point3 low = {x - six, y - six, z - six};
            Point3 high = {x + six, y + six, z + six};
            if (word(o, 168) != 0u) {
                objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 128);
                if (list[0] != 0u) {
                    for (unsigned* p = list; *p != 0u; ++p) {
                        unsigned char* const holder = from_word(word(o, 168));
                        unsigned char* const c = from_word(word(holder, 12));
                        unsigned char* const target = from_word(*p);
                        if (call160(c, target) == 0u) continue;
                        if (!(0.0f < real(from_word(*p), 560))) continue;
                        if (half(from_word(*p), 796) != 0) continue;
                        unsigned char* const w = from_word(word(o, 172));
                        const int a = w_call(w, 88, 0, s);
                        if (!(static_cast<int>(static_cast<unsigned>(eRand__10GameObject(o)) % 100u) + 1 < a)) continue;
                        const int b = w_call(from_word(word(o, 172)), 88, 3, s);
                        int damage = b * 60;
                        if (isBoss__FP10GameObject(from_word(*p)) != 0) damage = (damage >> 1) + 1;
                        unsigned char* const victim = from_word(*p);
                        unsigned char* const e = entry_of(victim, 216);
                        reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(e + 4)))(victim + *reinterpret_cast<short*>(e), damage);
                    }
                }
            }
        }
    } else {
        if (word(o, 156) != 0u) {
            real(o, 16) = real(o, 16) + real(o, 76);
            real(o, 20) = real(o, 20) + real(o, 80);
            real(o, 24) = real(o, 24) + real(o, 84);
            const Point3 at = *reinterpret_cast<Point3*>(o + 16);
            const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, at, six);
            if (real(o, 24) < height) word(o, 156) = 0;
        }
        const float x = real(o, 16);
        const float y = real(o, 20);
        const float z = real(o, 24);
        Point3 low = {x - six, y - six, z - six};
        Point3 high = {x + six, y + six, z + six};
        if (word(o, 168) != 0u) {
            objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 128);
            if (list[0] != 0u) {
                for (unsigned* p = list; *p != 0u; ++p) {
                    unsigned char* const c = from_word(word(from_word(word(o, 168)), 12));
                    if (call160(c, from_word(*p)) == 0u) continue;
                    if (static_cast<signed char>(o[160]) == 0) {
                        const int m = w_call(from_word(word(o, 172)), 88, 2, s);
                        o[160] = 1;
                        const unsigned char* const q = from_word(*p) + 16;
                        const Point3 offset = {0.0f, 0.0f, 36.0f};
                        word(o, 148) = static_cast<unsigned>(m * 60);
                        const Point3 moved = {*reinterpret_cast<const float*>(q) + offset.x, *reinterpret_cast<const float*>(q + 4), *reinterpret_cast<const float*>(q + 8) + offset.z};
                        *reinterpret_cast<Point3*>(o + 16) = moved;
                    }
                    unsigned char* const c2 = from_word(word(from_word(word(o, 168)), 12));
                    if (call160(c2, from_word(*p)) != 0u) {
                        unsigned char* const w = from_word(word(o, 172));
                        unsigned char* const e = from_word(word(w, 96)) + 72;
                        int outA;
                        int outB;
                        reinterpret_cast<void (*)(void*, int, int, int, int*, int*)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(e + 4)))(
                            w + *reinterpret_cast<short*>(e), 1, s, 0, &outA, &outB);
                        gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(from_word(*p), from_word(word(from_word(word(o, 168)), 12)),
                                                                                          static_cast<float>(outA), static_cast<float>(outB), 100000, 0, 0, 64, nullptr, false);
                    }
                }
            }
        }
    }
    *reinterpret_cast<Point3*>(o + 136) = *reinterpret_cast<Point3*>(o + 16);
    lightUpdatePosition__FP5Light(o + 88);
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    for (int i = 0; i < 8; ++i) {
        Point3 u;
        unit_ball(o, u, roll);
        Point3 v = {u.x * bits(0xbea22221u), u.y * bits(0xbea22221u), u.z * bits(0xbea22221u)};
        Point3 velocity = {real(o, 76) + v.x, real(o, 80) + v.y, real(o, 84) + v.z};
        Point3 w = {u.x + u.x, u.y + u.y, u.z + u.z};
        w.x = w.x * 12.0f;
        w.y = w.y * 12.0f;
        w.z = w.z * 12.0f;
        Point3 position = {w.x + real(o, 16), w.y + real(o, 20), w.z + real(o, 24)};
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w16_spellmsg_a_view_8d6b40, &position, &velocity, zero);
    }
    sword(o, 148) = sword(o, 148) - 1;
    if (sword(o, 148) == -1) {
        objectAddToDeleteList__FP10GameObject(o);
        lightDeactivate__FP5Light(o + 88);
    }
}
