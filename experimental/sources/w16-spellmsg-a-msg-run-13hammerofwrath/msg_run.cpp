// Observed msg_run__13HammerOfWrath at 0x002b3968, 4744 bytes. O is the object, P the point (three singles) at O+16, V the point at O+204 and U the
// point at O+216. The word at O+200 is a state number 0..4 (a jump table of five labels; any other value skips the state code), the word at O+160 a
// counter, H the object that is the word at O+76 (an owner holder; its word at +12, called C below, is a creature), N the node that is the word at
// O+80 (a list whose first node's word at +12, called X below, is the current target) and W the object that is the word at O+196. "e(T, off)" is the
// entry at table offset off of the table at (the word at T+72), called on T adjusted by the entry's signed halfword; "w(a1, a2)" is the entry at offset
// 88 of the table at (the word at W+96), called on W the same way with (a1, a2). frameNm is the global frame counter and world the world pointer.
// The six dynamics singles are: yaw = O+172, pitch = O+176, roll = O+180 and their rates at O+184, +188 and +192. Pool constants have no symbols; their
// bits are written where they are used. A halfword angle A starts as the signed halfword at O+56 (an int) and is replaced by -iatan2__Fff(...) in some
// states. Each state also sets the targets and gains the common tail uses: pitch target pt (default -10000.0), roll target rt (16384.0), yaw gain yg
// (default 0.0003), yaw damp yd (0.01), pull k (0.003) and drag g (0.05).
//  Entry: when H is nonzero and the squared distance from P to (C.x + 0.0, C.y, C.z + 60.0) exceeds 360000.0 the state becomes 0; when the signed byte
//    cutMode (0x00483e... gp-14040) is nonzero the state becomes 0; when H is nonzero, C's flags (word at +12) have 0x40 and the word at C+1400 equals 8
//    the state becomes 0 and a flag j is set (otherwise j is 0).
//  State 0 (search): when V's squared length exceeds 1.0, A = -iatan2__Fff(V.x, V.y). Q is P; when H is nonzero Q becomes (C.x + 0.0, C.y, C.z + 60.0) and U
//    is pulled toward Q until it is within 60.0 of it (U += (Q - U) * (1 - 60/dist) when dist > 60). A box Q +- (180, 180, 1200) is searched with
//    objectFindInBox__FG6Point3T0PP10GameObjecti(list, 128). Unless cutMode or j is set, each candidate X is skipped when worldHitscan__FP12_worldHeaderG6Point3T1fi
//    (world, P, X.pos - P, 1.0, 0) is above 0.0, when H is zero, or when e(C, 160)(X) is zero; otherwise its squared distance d2 from Q counts toward a
//    total when d2 < 14400.0 and, when below the best so far (starting at 32400.0), makes X the best. A total of 4 or more sets the counter to 0 and the
//    state to 4; otherwise a best X (if any) is linked with listOnewayLink__FP8ListHeadT0Pv(O+80, X, X) and the counter becomes 0; the state becomes 3
//    when cutMode is nonzero, otherwise 4 when ((float)eRand__10GameObject(O) * 2^-16) * 100.0 < (float)w(2, word at O+164) and 3 when not.
//  State 1 (turn): no N, or X flags with 8: state 0. Otherwise A = -iatan2(X.x - P.x, X.y - P.y); yd = 0.1, yg = 0.05, pt = 0.0; the single at O+188 loses
//    a tenth of itself; the counter is incremented and, once the old value was 61 or more, the state becomes 2 and
//    lumpFindResource__FPcT0("fx.lmp", "spel_charge_mord.anm") is played at O (volume 1.0).
//  State 2 (charge): no N or X flags 8: state 0. A = -iatan2(X.x - P.x, X.y - P.y); yg = 0.05, yd = 0.1, pt = 0.0; U becomes (X.x + 0.0, X.y, X.z + 48.0);
//    V becomes its projection on the unit direction from P to U; when the squared distance from P to U is below 576.0, w(0, word at O+164) is passed as the
//    first float to gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(X, O, (float)n, 40000.0, 0, 0, 0, 260, O+88, false), the sound
//    "spel_he_hammer_hit.anm" is played, the state becomes 3 and the counter 0.
//  State 3 (strike): as state 2 for N and X; A = -iatan2(...); yg = 0.1, yd = 0.6, k = 0.006, g = 0.15; U is pulled toward Q = (X.x + 0.0, X.y, X.z + 60.0)
//    until within 24.0. The counter is incremented; pt becomes -20000.0 from 31 on and 0.0 from 42 on; at exactly 47 a hit with amount n = w(0, word at
//    O+164) and second amount n + ((n * (roll * 2^-16)) * 0.2) is dealt (a2 = 300000, flags 257) and the sound played; the counter resets to 0 from 61 on.
//  State 4 (sweep): no H: state 0. The counter is incremented, the single at O+184 becomes -3500.0, pt = rt = 0.0, k = 0.01, g = 0.2. From 61 on, every 20th
//    count, X is the nearest creature (flags 0x20 set, 0x48 clear) within squared distance 5184.0 of P in the box P +- (72, 72, 1200); a hit with amount
//    w(0, word at O+164) and second amount n + ((n * roll) * 0.2) (a2 = 3000, flags 257) is dealt on it with the sound. Then with N and X (flags 8 clear):
//    A = -iatan2(...), yg = 0.1, yd = 0.6 and U pulled toward (X.x + 0.0, X.y, X.z + 60.0) until within 24.0; without N: U orbits (C.x + 0.0, C.y, C.z + 36.0) at
//    36.0 with the angle counter << 9. The state becomes 0 when counter << 9 exceeds 65536.
//  Common tail: yaw delta = (float)(short)(A - (int)yaw) * yg; the three rates gain their spring terms ((pt - pitch) * 0.05, (rt - roll) * 0.05, delta),
//    lose 20% (0.2) of themselves (the yaw rate loses yd of itself) and are integrated; rates at +188 and +192 gain isin__Fi(frameNm << 10) * 20.0 and
//    isin__Fi(frameNm * 800) * 20.0; V gains isin(frameNm * 403/358/332) * 0.01 per component; V += (U - P) * k; P += V; V loses g of itself;
//    objectUpdateInGrid__FP10GameObject(O); the word at O+168 drops by 1 and, when not above 0 and O is nonzero, the entry at table offset 8 of the table at
//    (the word at O+72) is called on O with 3.
// Inferred: a flying hammer that homes on a target, winds up, charges and strikes with several phases. Unknown: the meaning of most O fields. Nothing here
// ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern signed char cutMode;
int eRand__10GameObject(void* object);
float isin__Fi(int angle);
float icos__Fi(int angle);
int iatan2__Fff(float y, float x);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3* low, Point3* high, unsigned* list, int capacity);
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void* world, Point3 from, Point3 direction, float radius, int mode);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data);
void objectUpdateInGrid__FP10GameObject(void* object);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* attacker, void* target, float a, float b, int c, int d, int e, int f,
                                                                    void* description, bool g);
}

namespace {
using namespace w12c;

int w_call(unsigned char* o, int a1, int a2) {
    unsigned char* const w = from_word(word(o, 196));
    unsigned char* const e = from_word(word(w, 96)) + 88;
    return reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(e + 4)))(w + *reinterpret_cast<short*>(e), a1, a2);
}

unsigned call160(unsigned char* c, unsigned char* x) {
    unsigned char* const e = from_word(word(c, 72)) + 160;
    return reinterpret_cast<unsigned (*)(void*, unsigned)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(e + 4)))(c + *reinterpret_cast<short*>(e), to_word(x));
}

void play(unsigned char* o, const char* name) {
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>(name)), o, 1.0f);
}

// Q = (C.x + 0.0, C.y, C.z + z) for the creature-like object at c (the "+ 0.0" is an addition in the image).
Point3 above(const unsigned char* c, float z, float zero) {
    return Point3{*reinterpret_cast<const float*>(c + 16) + zero, *reinterpret_cast<const float*>(c + 20), *reinterpret_cast<const float*>(c + 24) + z};
}

// Pulls U toward q so that it is at most `radius` away.
void pull(unsigned char* o, const Point3& q, float radius) {
    unsigned char* const u = o + 216;
    const float ux = real(u, 0);
    const float uy = real(u, 4);
    const float uz = real(u, 8);
    const float dx = ux - q.x;
    const float dy = uy - q.y;
    const float dz = uz - q.z;
    const float dist = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    if (radius < dist) {
        const float rest = 1.0f - radius / dist;
        const float ex = (q.x - ux) * rest;
        const float ey = (q.y - uy) * rest;
        const float ez = (q.z - uz) * rest;
        real(u, 0) = ux + ex;
        real(u, 8) = uz + ez;
        real(u, 4) = uy + ey;
    }
}

float d2(const Point3& a, const unsigned char* b) {
    const float dx = a.x - *reinterpret_cast<const float*>(b + 16);
    const float dy = a.y - *reinterpret_cast<const float*>(b + 20);
    const float dz = a.z - *reinterpret_cast<const float*>(b + 24);
    return dx * dx + dy * dy + dz * dz;
}
}

extern "C" void msg_run__13HammerOfWrath(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    float pt = bits(0xc61c4000u), rt = bits(0x46800000u), yg = bits(0x399d4951u), yd = bits(0x3c23d70au), kp = bits(0x3b449ba5u), dg = bits(0x3d4ccccc);
    int angle = half(o, 56);
    unsigned list[128];
    unsigned char* const holder = from_word(word(o, 76));
    unsigned char* const p = o + 16;
    if (holder != nullptr) {
        const Point3 q = above(from_word(word(holder, 12)), 60.0f, zero);
        const float dx = q.x - real(p, 0);
        const float dy = q.y - real(p, 4);
        const float dz = q.z - real(p, 8);
        if (bits(0x48afc800u) < dx * dx + dy * dy + dz * dz) word(o, 200) = 0;
    }
    if (cutMode != 0) word(o, 200) = 0;
    bool j = false;
    if (holder != nullptr) {
        unsigned char* const c = from_word(word(holder, 12));
        if ((word(c, 12) & 0x40u) != 0u && word(c, 1400) == 8u) {
            word(o, 200) = 0;
            j = true;
        }
    }
    switch (word(o, 200)) {
    case 0: {
        const float vx = real(o, 204), vy = real(o, 208), vz = real(o, 212);
        if (1.0f < vx * vx + vy * vy + vz * vz) angle = -iatan2__Fff(real(o, 204), real(o, 208));
        Point3 q = *reinterpret_cast<Point3*>(p);
        unsigned char* h = from_word(word(o, 76));
        if (h != nullptr) {
            q = above(from_word(word(h, 12)), 60.0f, zero);
            pull(o, q, 60.0f);
        }
        Point3 low = {q.x - 180.0f, q.y - 180.0f, q.z - 1200.0f};
        Point3 high = {q.x + 180.0f, q.y + 180.0f, q.z + 1200.0f};
        unsigned char* best = nullptr;
        int count = 0;
        float bestD2 = bits(0x46fd2000u);
        objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 128);
        if (cutMode == 0 && !j && list[0] != 0u) {
            for (unsigned* it = list; *it != 0u; ++it) {
                unsigned char* const x = from_word(*it);
                const Point3 dir = {real(x, 16) - real(p, 0), real(x, 20) - real(p, 4), real(x, 24) - real(p, 8)};
                const Point3 from = *reinterpret_cast<Point3*>(p);
                if (0.0f < worldHitscan__FP12_worldHeaderG6Point3T1fi(world, from, dir, 1.0f, 0)) continue;
                unsigned char* const hh = from_word(word(o, 76));
                if (hh == nullptr) continue;
                if (call160(from_word(word(hh, 12)), x) == 0u) continue;
                const float dd = d2(q, x);
                if (dd < bits(0x46610000u)) ++count;
                if (dd < bestD2) {
                    bestD2 = dd;
                    best = x;
                }
            }
        }
        if (count >= 4) {
            word(o, 160) = 0;
            word(o, 200) = 4;
            break;
        }
        if (best == nullptr) break;
        listOnewayLink__FP8ListHeadT0Pv(o + 80, best, best);
        unsigned next = 3;
        if (cutMode == 0) {
            const float roll = static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u) * 100.0f;
            if (roll < static_cast<float>(w_call(o, 2, static_cast<int>(word(o, 164))))) next = 4;
        }
        word(o, 160) = 0;
        word(o, 200) = next;
        break;
    }
    case 1: {
        unsigned char* const node = from_word(word(o, 80));
        if (node == nullptr) { word(o, 200) = 0; break; }
        unsigned char* const x = from_word(word(node, 12));
        if ((word(x, 12) & 8u) != 0u) { word(o, 200) = 0; break; }
        yd = bits(0x3dcccccc);
        yg = bits(0x3d4ccccc);
        pt = zero;
        angle = -iatan2__Fff(real(x, 16) - real(p, 0), real(x, 20) - real(p, 4));
        real(o, 188) = real(o, 188) - real(o, 188) * yd;
        const int counter = sword(o, 160);
        word(o, 160) = static_cast<unsigned>(counter + 1);
        if (counter >= 61) {
            word(o, 200) = 2;
            play(o, "spel_charge_mord.anm");
        }
        break;
    }
    case 2: {
        unsigned char* const node = from_word(word(o, 80));
        if (node == nullptr) { word(o, 200) = 0; break; }
        unsigned char* const x = from_word(word(node, 12));
        if ((word(x, 12) & 8u) != 0u) { word(o, 200) = 0; break; }
        yg = bits(0x3d4ccccc);
        yd = bits(0x3dcccccc);
        angle = -iatan2__Fff(real(x, 16) - real(p, 0), real(x, 20) - real(p, 4));
        pt = zero;
        const Point3 q = above(x, 48.0f, zero);
        *reinterpret_cast<Point3*>(o + 216) = q;
        float dx = real(o + 216, 0) - real(p, 0);
        float dy = real(o + 216, 4) - real(p, 4);
        float dz = real(o + 216, 8) - real(p, 8);
        float inv;
        const float one = 1.0f;
        __asm__("rsqrt.s %0, %1, %2" : "=f"(inv) : "f"(one), "f"(dx * dx + dy * dy + dz * dz));
        dx = dx * inv;
        dy = dy * inv;
        dz = dz * inv;
        const float dot = dx * real(o, 204) + dy * real(o, 208) + dz * real(o, 212);
        const Point3 projected = {dx * dot, dy * dot, dz * dot};
        *reinterpret_cast<Point3*>(o + 204) = projected;
        const float ex = real(o + 216, 0) - real(p, 0);
        const float ey = real(o + 216, 4) - real(p, 4);
        const float ez = real(o + 216, 8) - real(p, 8);
        if (ex * ex + ey * ey + ez * ez < 576.0f) {
            const int n = w_call(o, 0, static_cast<int>(word(o, 164)));
            gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(x, o, static_cast<float>(n), bits(0x471c4000u), 0, 0, 0, 260, o + 88, false);
            play(o, "spel_he_hammer_hit.anm");
            word(o, 200) = 3;
            word(o, 160) = 0;
        }
        break;
    }
    case 3: {
        unsigned char* const node = from_word(word(o, 80));
        if (node == nullptr) { word(o, 200) = 0; break; }
        unsigned char* const x = from_word(word(node, 12));
        if ((word(x, 12) & 8u) != 0u) { word(o, 200) = 0; break; }
        yg = bits(0x3dcccccc);
        yd = bits(0x3f199999);
        angle = -iatan2__Fff(real(x, 16) - real(p, 0), real(x, 20) - real(p, 4));
        pull(o, above(x, 60.0f, zero), 24.0f);
        kp = bits(0x3bc49ba5u);
        dg = bits(0x3e199999u);
        const int counter = sword(o, 160) + 1;
        word(o, 160) = static_cast<unsigned>(counter);
        if (counter >= 31) pt = bits(0xc69c4000u);
        if (counter >= 42) pt = zero;
        if (counter == 47) {
            const float n = static_cast<float>(w_call(o, 0, static_cast<int>(word(o, 164))));
            const float r = static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u);
            gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(x, o, n, n + (n * r) * bits(0x3e4ccccc), 0x493e0, 0, 0, 257, o + 88, false);
            play(o, "spel_he_hammer_hit.anm");
        }
        if (sword(o, 160) >= 61) word(o, 160) = 0;
        break;
    }
    case 4: {
        if (holder == nullptr) { word(o, 200) = 0; break; }
        pt = zero;
        const int counter = sword(o, 160) + 1;
        word(o, 160) = static_cast<unsigned>(counter);
        real(o, 184) = bits(0xc55ac000u);
        kp = bits(0x3c23d70au);
        dg = bits(0x3e4ccccc);
        rt = pt;
        if (counter >= 61 && counter % 20 == 0) {
            Point3 low = {real(p, 0) - 72.0f, real(p, 4) - 72.0f, real(p, 8) - 1200.0f};
            Point3 high = {real(p, 0) + 72.0f, real(p, 4) + 72.0f, real(p, 8) + 1200.0f};
            objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 128);
            unsigned char* best = nullptr;
            float bestD2 = bits(0x45a20000u);
            const Point3 at = *reinterpret_cast<Point3*>(p);
            if (list[0] != 0u) {
                for (unsigned* it = list; *it != 0u; ++it) {
                    unsigned char* const x = from_word(*it);
                    const unsigned flags = word(x, 12);
                    if ((flags & 0x20u) == 0u || (flags & 0x48u) != 0u) continue;
                    const float dd = d2(at, x);
                    if (dd < bestD2) {
                        bestD2 = dd;
                        best = x;
                    }
                }
            }
            if (best != nullptr) {
                const float n = static_cast<float>(w_call(o, 0, static_cast<int>(word(o, 164))));
                const float r = static_cast<float>(eRand__10GameObject(o)) * bits(0x37800000u);
                gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(best, o, n, n + (n * r) * bits(0x3e4ccccc), 3000, 0, 0, 257, o + 88, false);
                play(o, "spel_he_hammer_hit.anm");
            }
        }
        unsigned char* const node = from_word(word(o, 80));
        if (node != nullptr) {
            unsigned char* const x = from_word(word(node, 12));
            if ((word(x, 12) & 8u) != 0u) { word(o, 200) = 0; break; }
            yg = bits(0x3dcccccc);
            yd = bits(0x3f199999);
            angle = -iatan2__Fff(real(x, 16) - real(p, 0), real(x, 20) - real(p, 4));
            pull(o, above(x, 60.0f, zero), 24.0f);
        } else {
            unsigned char* const c = from_word(word(from_word(word(o, 76)), 12));
            const Point3 q = above(c, 36.0f, zero);
            const int a = sword(o, 160) << 9;
            const float cs = icos__Fi(a);
            const float sn = isin__Fi(sword(o, 160) << 9);
            const float ox = cs * 36.0f;
            const float oy = sn * 36.0f;
            const Point3 u = {q.x + ox, q.y + oy, q.z};
            *reinterpret_cast<Point3*>(o + 216) = u;
        }
        if (sword(o, 160) * 512 > 0x10000) word(o, 200) = 0;
        break;
    }
    default:
        break;
    }
    const float yawDelta = static_cast<float>(static_cast<short>(angle - static_cast<int>(real(o, 172)))) * yg;
    const float pitchSpring = (pt - real(o, 176)) * bits(0x3d4ccccc);
    const float rollSpring = (rt - real(o, 180)) * bits(0x3d4ccccc);
    float rollRate = real(o, 192) + rollSpring;
    float pitchRate = real(o, 188) + pitchSpring;
    float yawRate = real(o, 184) + yawDelta;
    const float rollLoss = rollRate * bits(0x3e4ccccc);
    const float pitchLoss = pitchRate * bits(0x3e4ccccc);
    const float yawLoss = yawRate * yd;
    rollRate = rollRate - rollLoss;
    pitchRate = pitchRate - pitchLoss;
    yawRate = yawRate - yawLoss;
    const float yaw0 = real(o, 172), pitch0 = real(o, 176), roll0 = real(o, 180);
    real(o, 180) = roll0 + rollRate;
    real(o, 192) = rollRate;
    real(o, 176) = pitch0 + pitchRate;
    real(o, 188) = pitchRate;
    real(o, 172) = yaw0 + yawRate;
    real(o, 184) = yawRate;
    real(o, 188) = real(o, 188) + isin__Fi(frameNm << 10) * 20.0f;
    real(o, 192) = real(o, 192) + isin__Fi(frameNm * 800) * 20.0f;
    real(o, 204) = real(o, 204) + isin__Fi(frameNm * 403) * bits(0x3c23d70au);
    real(o, 208) = real(o, 208) + isin__Fi(frameNm * 358) * bits(0x3c23d70au);
    real(o, 212) = real(o, 212) + isin__Fi(frameNm * 332) * bits(0x3c23d70au);
    {
        const float ex = (real(o, 216) - real(p, 0)) * kp;
        const float ey = (real(o, 220) - real(p, 4)) * kp;
        const float ez = (real(o, 224) - real(p, 8)) * kp;
        real(o, 204) = real(o, 204) + ex;
        real(o, 208) = real(o, 208) + ey;
        real(o, 212) = real(o, 212) + ez;
        real(p, 0) = real(p, 0) + real(o, 204);
        real(p, 4) = real(p, 4) + real(o, 208);
        real(p, 8) = real(p, 8) + real(o, 212);
        const float vx = real(o, 204), vy = real(o, 208), vz = real(o, 212);
        const float lx = vx * dg, ly = vy * dg, lz = vz * dg;
        real(o, 208) = vy - ly;
        real(o, 212) = vz - lz;
        real(o, 204) = vx - lx;
    }
    objectUpdateInGrid__FP10GameObject(o);
    sword(o, 168) = sword(o, 168) - 1;
    if (sword(o, 168) > 0) return;
    __asm__("" : "+r"(o));
    if (o != nullptr) {
        unsigned char* const entry = from_word(word(o, 72)) + 8;
        reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(entry + 4)))(o + *reinterpret_cast<short*>(entry), 3);
    }
}
