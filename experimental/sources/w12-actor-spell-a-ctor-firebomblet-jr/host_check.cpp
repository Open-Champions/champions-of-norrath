// Host contract for the FireBombletJr constructor; not an EE input. Stubs record the call order; the expected object bytes
// are recomputed here with a separate straight-line version of the generator and arithmetic.
#define W10_ACTOR_M_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include "ctor.cpp"
unsigned char* w10_actor_m_host_base;
unsigned w10_actor_m_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
unsigned char vt_FireBombletJr[8];
unsigned char bombResMap[32];
unsigned syncRandState;
static std::string order;
static unsigned char* obj;
static float engine_start[3];
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    char b[160];
    std::snprintf(b, sizeof b, "go(%d,%.6f,%.6f,%.6f,%d,%d,%u);", self == obj, p.x, p.y, p.z, map == bombResMap, names == nullptr, syncRandState);
    order += b;
    std::memset(self, 0x21, 0x200);
    const unsigned flags = 0x100;
    std::memcpy(static_cast<unsigned char*>(self) + 12, &flags, 4);
}
extern "C" void matIdent__FP8Matrix33(void* m) { order += "ident;"; float* f = static_cast<float*>(m); for (int i = 0; i < 9; ++i) f[i] = (i % 4 == 0) ? 1.0f : 0.0f; }
extern "C" void matRot__FP8Matrix33is(void* m, int axis, short angle) {
    char b[64]; std::snprintf(b, sizeof b, "rot(%d,%d);", axis, angle); order += b;
    static_cast<float*>(m)[1] = static_cast<float>(angle) * 0.001f;
}
extern "C" void matTransform__FP6Point3P8Matrix33T0(Point3* out, void* m, Point3* in) {
    const float a = static_cast<float*>(m)[1];
    char b[96]; std::snprintf(b, sizeof b, "xf(%.6f,%.6f,%.6f);", in->x, in->y, in->z); order += b;
    out->x = in->x + a; out->y = in->y * 2 + a * 0.5f - 0.25f; out->z = in->z + 1.0f;
}
extern "C" void objectAddToRunList__FP10GameObject(void* o) { order += std::string("run") + std::to_string(o == obj) + ";"; }

struct Gen {
    unsigned s;
    unsigned next() { s = s * 1664525u + 1013904223u; return s >> 16; }
    float unit() { return static_cast<float>(next()) / 65536.0f; }
};
int main() {
    alignas(16) static unsigned char o[0x300], p[0x300];
    obj = o;
    w10_actor_m_host_base = o;
    int bad = 0;
    for (int k = 0; k < 30; ++k) {
        std::memset(o, 0, sizeof o);
        std::memset(p, 0, sizeof p);
        Gen g{static_cast<unsigned>(k * 2654435761u + 12345u)};
        const float pp[3] = {10.0f + k, -20.0f + 3 * k, 5.0f - k};
        const float pv[3] = {1.5f - 0.3f * k, (k % 3) - 1.0f, 0.75f};
        const float pf84 = pv[2], pf164 = 3.25f * k;
        const unsigned short heading = static_cast<unsigned short>(40000 + 997 * k);
        std::memcpy(p + 16, pp, 12); std::memcpy(p + 76, pv, 12); std::memcpy(p + 164, &pf164, 4); std::memcpy(p + 56, &heading, 2); p[157] = static_cast<unsigned char>(0xa0 + k);
        syncRandState = g.s;
        order.clear();
        __13FireBombletJrP11FireBomblet(o, p);
        // independent model
        Gen m{g.s};
        float v[3];
        int tries = 0;
        do { for (int i = 0; i < 3; ++i) { const float r = m.unit(); v[i] = r + r - 1.0f; } ++tries; } while (1.0f < v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        float start[3];
        for (int i = 0; i < 3; ++i) start[i] = pp[i] + v[i] * 5.0f;
        char want[512];
        int n = std::snprintf(want, sizeof want, "go(1,%.6f,%.6f,%.6f,1,1,%u);", start[0], start[1], start[2], m.s);
        const unsigned roll = m.next() % 32000u;
        const unsigned short h = static_cast<unsigned short>(heading + roll - 16384u);
        const float len = std::sqrt(pv[0] * pv[0] + pv[1] * pv[1] + 0.0f);
        n += std::snprintf(want + n, sizeof want - n, "ident;rot(2,%d);xf(0.000000,-1.000000,0.000000);", static_cast<short>(h));
        const float a = static_cast<float>(static_cast<short>(h)) * 0.001f;
        float d[3] = {0.0f + a, -2.0f + a * 0.5f - 0.25f, 0.0f + 1.0f};
        const float speed = len * 0.5f + m.unit() * 0.5f;
        for (float& c : d) c *= speed;
        if (d[0] * pv[0] + d[1] * pv[1] + d[2] * pv[2] < 0.70699996f) { d[0] = -d[0]; d[1] = -d[1]; }
        const float f84 = pf84 * (0.5f + 0.5f * m.unit());
        const short f92 = static_cast<short>(static_cast<int>((m.unit() + 0.5f) * 2615.0f));
        const unsigned long long q104 = 1ull << (m.next() & 3);
        std::snprintf(want + n, sizeof want - n, "run1;");
        if (order != want) { std::fprintf(stderr, "calls\n%s\n%s\n", order.c_str(), want); bad = 1; }
        float g76[3], g84, g96; short g92, g90; unsigned long long g104; unsigned g72, g12; float g28;
        std::memcpy(g76, o + 76, 12); std::memcpy(&g84, o + 84, 4); std::memcpy(&g92, o + 92, 2); std::memcpy(&g90, o + 90, 2); std::memcpy(&g96, o + 96, 4);
        std::memcpy(&g104, o + 104, 8); std::memcpy(&g72, o + 72, 4); std::memcpy(&g12, o + 12, 4); std::memcpy(&g28, o + 28, 4);
        unsigned short g56; std::memcpy(&g56, o + 56, 2);
        const bool ok = g76[0] == d[0] && g76[1] == d[1] && g76[2] == f84 && g84 == f84 && g92 == f92 && g90 == 0 && g96 == pf164 && g104 == q104 &&
                        g72 == w10m::to_word(vt_FireBombletJr) && g12 == 16385u && g28 == 6.0f && g56 == h && o[40] == 6 && o[88] == 0 && o[89] == p[157] && syncRandState == m.s;
        if (!ok) {
            std::fprintf(stderr, "fields wrong (case %d): d %g %g %g got %g %g %g; f84 %g/%g f92 %d/%d f96 %g/%g q %llu/%llu v72 %d v12 %u g28 %g h %u/%u syncs %u/%u\n", k, d[0], d[1], d[2], g76[0], g76[1], g76[2], f84, g84, f92, g92, pf164, g96,
                         q104, g104, g72 == w10m::to_word(vt_FireBombletJr), g12, g28, h, g56, syncRandState, m.s);
            bad = 1;
        }
    }
    if (!bad) std::puts("firebomblet jr ctor ok");
    return bad;
}
