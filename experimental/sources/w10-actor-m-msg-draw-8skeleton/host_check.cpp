// Host contract for msg_draw__8Skeleton; not an EE input. Words that hold addresses are offsets from a host
// base; callees record their arguments.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
#include "../w10-actor-m-common/access.hpp"
unsigned char* w10_actor_m_host_base;
unsigned w10_actor_m_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
extern "C" unsigned char skeletonResMap[800];
unsigned char skeletonResMap[800];
struct Rec { std::string name; void *a, *b, *c, *d, *e; float fa, fb; long x, y, z; Point3 p; };
static Rec recs[8]; static int nrec;
extern "C" void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* d, void* m, void* s, Point3 p, short ang, void* m2, void* an, float a, int flag, int zero, float b, void* mat) {
    Rec& r = recs[nrec++]; r.name = "fx"; r.a = d; r.b = m; r.c = s; r.p = p; r.x = ang; r.d = m2; r.e = an; r.fa = a; r.fb = b; r.y = flag; r.z = zero * 1000 + (mat != nullptr);
}
static void* ret_record;
extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* m, void* t, void* x) {
    Rec& r = recs[nrec++]; r.name = "draw"; r.a = self; r.b = m; r.c = t; r.d = x; return ret_record;
}
extern "C" void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(void* m, void* t, int a, void* m2, void* rec, unsigned long b, int c, int d) {
    Rec& r = recs[nrec++]; r.name = "att"; r.a = m; r.b = t; r.c = rec; r.d = m2; r.x = a; r.y = static_cast<long>(b); r.z = c * 10 + d;
}
#include "msg_draw.cpp"
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x4000];
    w10_actor_m_host_base = arena;
    const unsigned O = 0x100, T = 0x2000;
    ret_record = arena + 0x3000;
    for (unsigned k : {0u, 1u, 5u, 12u}) for (unsigned j : {0u, 2u, 9u}) for (unsigned q : {0u, 1u}) for (short st : {6, 5}) {
        std::memset(arena, 0, sizeof arena);
        for (unsigned i = 0; i < 800; i += 4) { unsigned v = 0x3400 + i; std::memcpy(skeletonResMap + i, &v, 4); }
        std::memcpy(arena + O + 36, &T, 4);
        unsigned m = 0x3100, tx = 0x3200; std::memcpy(arena + T + 12, &m, 4); std::memcpy(arena + T + 20, &tx, 4);
        std::memcpy(arena + O + 976, &k, 4); std::memcpy(arena + O + 1120, &j, 4); std::memcpy(arena + O + 1124, &q, 4);
        Point3 pos{1.5f, -2.0f, 7.0f}; std::memcpy(arena + O + 16, &pos, 12);
        short ang = -77; std::memcpy(arena + O + 56, &ang, 2); std::memcpy(arena + O + 572, &st, 2);
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        nrec = 0;
        msg_draw__8Skeleton(arena + O);
        int n = 0;
        if (k) {
            Rec& r = recs[n++];
            unsigned mw; std::memcpy(&mw, skeletonResMap + 4 + 8 * k, 4);
            if (r.name != "fx" || r.a != arena + O + 1048 || r.b != arena + mw || r.c != arena + O + 980 || std::memcmp(&r.p, &pos, 12) || r.x != ang ||
                r.d != arena + m || r.e != arena + O + 288 || r.fa != 0.5f || r.fb != 1.0f || r.y != (st == 6) || r.z != 0) bad = 1;
        }
        { Rec& r = recs[n++]; if (r.name != "draw" || r.a != arena + O || r.b != arena + m || r.c != arena + tx || r.d != nullptr) bad = 1; }
        auto att = [&](unsigned ao, unsigned bo) {
            Rec& r = recs[n++]; unsigned x, y; std::memcpy(&x, skeletonResMap + ao, 4); std::memcpy(&y, skeletonResMap + bo, 4);
            if (r.name != "att" || r.a != arena + x || r.b != arena + y || r.c != ret_record || r.d != arena + m || r.x != 0 || r.y != -1 || r.z != -10) bad = 1;
        };
        if (k) att(4 + 8 * k, 12 + 8 * k);
        if (j) att(4 + 8 * j, 12 + 8 * j);
        if (q) att(652, 660);
        if (n != nrec) bad = 1;
        if (std::memcmp(arena, ref, sizeof arena)) bad = 1;
    }
    return bad;
}
