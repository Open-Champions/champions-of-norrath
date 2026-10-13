// Host contract for doWavelet__9WaveMakerP6Point3P8Matrix33i; not an EE input. Every callee is a recording stub.
// The control flow (which draws are taken, which particle definition is chosen, which objects are marked) is
// compared with a model that follows the generator in 64-bit arithmetic; values of positions and velocities are
// checked for the always-wet and always-dry cases with the stub transform being the identity.
#define W11_ACTOR_C_HOST 1
#include "do_wavelet.cpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w11_actor_c_host_base;
W11ActorCHostSlot w11_actor_c_host_slots[1];
int frameNm;
unsigned char* world = reinterpret_cast<unsigned char*>(0x1000);
unsigned char waveTrail[80], waveMainParticle[80], waveMainAlternate[80], waveRandomFoam[80];
static std::string calls;
static int rot_calls, transform_calls, box_calls, perturb_calls;
static std::vector<short> rot_angles;
static void* box_list[8];
extern "C" {
void matTransform__FP6Point3P8Matrix33T0(Point3* out, Matrix33* m, Point3* in) { ++transform_calls; *out = *in; }
void matRot__FP8Matrix33is(Matrix33*, int axis, short angle) { ++rot_calls; rot_angles.push_back(angle); if (axis != 0) calls += "badaxis;"; }
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity) {
    ++box_calls;
    char b[200]; std::snprintf(b, sizeof b, "box(%g,%g,%g;%g,%g,%g;%d);", low.x, low.y, low.z, high.x, high.y, high.z, capacity); calls += b;
    int i = 0; for (; box_list[i]; ++i) found[i] = box_list[i]; found[i] = nullptr; return i;
}
int iatan2__Fff(float a, float b) { char s[64]; std::snprintf(s, sizeof s, "atan(%g,%g);", a, b); calls += s; return 0x12345; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float e) {
    const char* n = d == waveTrail ? "trail" : d == waveMainParticle ? "main" : d == waveMainAlternate ? "alt" : d == waveRandomFoam ? "foam" : "?";
    char s[200]; std::snprintf(s, sizeof s, "%s(%.4g,%.4g,%.4g|%.4g,%.4g,%.4g|%g);", n, p->x, p->y, p->z, v->x, v->y, v->z, e); calls += s;
}
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void* w, Point3 p, float a, float b) {
    ++perturb_calls; char s[120]; std::snprintf(s, sizeof s, "perturb(%d,%g,%g,%g,%g,%g);", w == world, p.x, p.y, p.z, a, b); calls += s;
}
}
struct Gen {
    unsigned long long s;
    unsigned next() { s = (s * 1664525ull + 1013904223ull) & 0xffffffffull; return static_cast<unsigned>(s >> 16); }
};
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n  calls: %s\n", what, calls.c_str()); ++failures; } }
static unsigned char selfMem[2048];
static void reset(unsigned state, float water) {
    std::memset(selfMem, 0, sizeof selfMem);
    std::memcpy(selfMem + 68, &state, 4);
    std::memcpy(selfMem + 1888, &water, 4);
    const short r = 1000, l = 77; std::memcpy(selfMem + 1876, &r, 2); std::memcpy(selfMem + 1878, &l, 2);
    calls.clear(); rot_calls = transform_calls = box_calls = perturb_calls = 0; rot_angles.clear();
    std::memset(box_list, 0, sizeof box_list);
}
int main() {
    Matrix33 m = {{1, 0, 0, 0, 1, 0, 0, 0, 1}};
    const Point3 p = {10.0f, 20.0f, 30.0f};
    // Gating by (frameNm + n): box when & 15 == 0, perturb when & 31 == 0
    for (int f = 0; f < 70; ++f) for (int n : {0, 5, -3}) {
        frameNm = f; reset(0x1234u, 1e9f);
        doWavelet__9WaveMakerP6Point3P8Matrix33i(selfMem, &p, &m, n);
        expect(box_calls == (((f + n) & 15) == 0), "box gating");
        expect(perturb_calls == (((f + n) & 31) == 0), "perturb gating");
        expect(rot_calls == 7 && rot_angles[0] == 1000, "first rotation uses the halfword at +1876");
        for (int i = 1; i < 7; ++i) expect(rot_angles[i] == 11186, "pass rotation");
        expect(transform_calls == 7, "seven transforms with every pass wet");
        if (((f + n) & 31) == 0) expect(calls.find("perturb(1,10,20,30,435,24);") != std::string::npos, "perturb arguments");
    }
    // Always wet (water level far above every point): six trail particles; with identity transforms the first one
    // is position (Q.x - W.x*34 + jitter, Q.y - W.y*34 + jitter, water level) and velocity ((u-0.5)*5, (u'-0.5)*5, 0).
    {
        frameNm = 1; reset(777u, 100.0f);
        doWavelet__9WaveMakerP6Point3P8Matrix33i(selfMem, &p, &m, 0);
        int trails = 0; for (size_t at = 0; (at = calls.find("trail(", at)) != std::string::npos; ++at) ++trails;
        expect(trails == 6, "six wet particles");
        expect(calls.find("main(") == std::string::npos && calls.find("alt(") == std::string::npos, "no dry particles when wet");
        float px, py, pz, vx, vy, vz, e; char name[8];
        std::sscanf(calls.c_str(), "%5[a-z](%f,%f,%f|%f,%f,%f|%f)", name, &px, &py, &pz, &vx, &vy, &vz, &e);
        Gen h{777};
        const double w = (h.next() / 65536.0 * 0.372996985912323 + 0.8135014772415161) * -15.0;
        const double u1 = h.next() / 65536.0, u2 = h.next() / 65536.0, u3 = h.next() / 65536.0, u4 = h.next() / 65536.0;
        const double wy = -1.7572853565216064;
        auto close = [](double a, double b) { return std::fabs(a - b) < 2e-3; };
        expect(close(vx, (u1 - 0.5) * 5.0) && close(vy, (u2 - 0.5) * 5.0) && vz == 0.0f && pz == 100.0f, "wet velocity and level");
        expect(close(px, 10.0 - 0.0 * 34.0 + (u3 - 0.5) * 36.0) && close(py, (20.0 + w) - wy * 34.0 + (u4 - 0.5) * 36.0), "wet position");
    }
    // Always dry: the control flow of the draws follows the model
    for (unsigned seed = 1; seed < 3000; seed += 7) {
        frameNm = 1; reset(seed, -1e9f);
        doWavelet__9WaveMakerP6Point3P8Matrix33i(selfMem, &p, &m, 0);
        Gen g{seed}; g.next();
        std::string want;
        for (int pass = 0; pass < 6; ++pass) {
            if (g.next() & 1u) {
                g.next(); g.next(); g.next();
                want += (g.next() & 0xffu) ? "main;" : "alt;";
                if ((g.next() & 0x1ffu) == 0) { g.next(); want += "foam;"; }
            }
        }
        std::string got; size_t at = 0;
        while (at < calls.size()) {
            size_t e = calls.find('(', at); std::string n = calls.substr(at, e - at);
            if (n == "main" || n == "alt" || n == "foam") got += n + ";";
            at = calls.find(");", at) + 2;
        }
        expect(got == want, "dry control flow");
        expect(transform_calls == 7 + [&] { int c = 0; Gen h{seed}; h.next(); for (int pass = 0; pass < 6; ++pass) { if (h.next() & 1u) { ++c; h.next(); h.next(); h.next(); h.next(); if ((h.next() & 0x1ffu) == 0) h.next(); } } return c; }(), "dry transform count");
    }
    // Objects: marking rules and the direction fed to iatan2
    {
        static unsigned char objs[6][800];
        const unsigned flagsList[6] = {0x20, 0x21, 0x00, 0x30, 0x20, 0x20};
        const float zs[6] = {5.0f, 5.0f, 5.0f, 5.0f, 18.0f, 18.0f};   // p.z = 30: differences 25, 25, 25, 25, 12 (not above), 12
        for (int i = 0; i < 6; ++i) { std::memset(objs[i], 0, sizeof objs[i]); std::memcpy(objs[i] + 12, &flagsList[i], 4); std::memcpy(objs[i] + 24, &zs[i], 4); box_list[i] = objs[i]; }
        frameNm = 0; reset(99u, 1e9f);
        for (int i = 0; i < 6; ++i) box_list[i] = objs[i];
        doWavelet__9WaveMakerP6Point3P8Matrix33i(selfMem, &p, &m, 0);
        const bool marked[6] = {true, false, false, true, false, false};
        for (int i = 0; i < 6; ++i) {
            float v; unsigned short h; std::memcpy(&v, objs[i] + 756, 4); std::memcpy(&h, objs[i] + 748, 2);
            expect(marked[i] ? (v == 82.0f && h == 0x2345) : (v == 0.0f && h == 0), "object marking");
        }
        expect(calls.find("box(-2,8,-30;-2,32,40;64);") == 0, "box arguments (the high x equals the low x)");
    }
    if (!failures) std::puts("doWavelet ok");
    return failures != 0;
}
