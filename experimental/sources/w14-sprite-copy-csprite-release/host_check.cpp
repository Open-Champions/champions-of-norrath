// Host check only; not a build input.
#define NORRATH_HOST_ARENA 1
#include "release.cpp"
#include <cstdint>
#include <cstring>
alignas(16) static unsigned char arena[0x20000];
static unsigned word_of(const void* p) { return static_cast<unsigned>(reinterpret_cast<uintptr_t>(p)); }
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return reinterpret_cast<unsigned char*>((reinterpret_cast<uintptr_t>(arena) & ~uintptr_t{0xffffffffu}) | a); }
typedef unsigned char* P;
struct Call { int id; intptr_t a, b, c; };
static Call log_[800]; static int n_; static Call expect_[800]; static int expect_n;
static void rec(int id, intptr_t a, intptr_t b = 0, intptr_t c = 0) { if (n_ < 800) log_[n_] = Call{id, a, b, c}; ++n_; }
static void ex(int id, intptr_t a, intptr_t b = 0, intptr_t c = 0) { if (expect_n < 800) expect_[expect_n] = Call{id, a, b, c}; ++expect_n; }
static bool same() { if (n_ != expect_n || n_ > 800) return false; for (int i = 0; i < n_; ++i) if (log_[i].id != expect_[i].id || log_[i].a != expect_[i].a || log_[i].b != expect_[i].b || log_[i].c != expect_[i].c) return false; return true; }
static int baseRet, failAt, calls;
static P scene_, dict_, rast_, snd_, coll_;
extern "C" int Release__9VIHSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(P s, P sc, P d, P r, P sd, P c) { rec(1, (intptr_t)s, (intptr_t)sc, (intptr_t)d); (void)r; (void)sd; (void)c; return baseRet; }
static int tick(int id, intptr_t a, int b, intptr_t c = 0) { rec(id, a, b, c); return (++calls == failAt) ? -4 : 3; }
extern "C" int ReleaseAnimation__7VIScenei(P, int a) { return tick(2, (intptr_t)scene_, a); }
extern "C" int ReleaseSound__13VISoundDeviceiR12VIDictionary(P d, int s, P di) { return tick(3, (intptr_t)d, s, (intptr_t)di); }
extern "C" int ReleaseSprite__7VIScenei(P, int s) { return tick(4, (intptr_t)scene_, s); }
extern "C" int ReleaseMaterialPal__8VIRasteriR12VIDictionary(P r, int p, P d) { return tick(5, (intptr_t)r, p, (intptr_t)d); }
extern "C" int ReleaseRefMap__7VIScenei(P, int m) { return tick(6, (intptr_t)scene_, m); }
extern "C" void ReleaseSurface__8VIRasteriR12VIDictionary(P r, int s, P d) { tick(7, (intptr_t)r, s, (intptr_t)d); }
extern "C" void VITrailFxEnd__FP9VITrailFx(unsigned t) { rec(8, t); }
extern "C" void Stop__13VISoundDevicei(P d, int v) { rec(9, (intptr_t)d, v); }
extern "C" void Destroy__17VIParticleEmitter(P e) { rec(10, (intptr_t)e); }
extern "C" void ReleaseParticleDefinition__16VIParticleSystemiR12VIDictionary(P s, int d, P di) { rec(11, (intptr_t)s, d, (intptr_t)di); }
static unsigned lcg = 1;
static int rnd() { lcg = lcg * 1664525u + 1013904223u; return (int)(lcg >> 8); }
int main() {
    P self = arena + 0x100; scene_ = arena + 0x8000; dict_ = arena + 0x8010; rast_ = arena + 0x8020; snd_ = arena + 0x8030; coll_ = arena + 0x8040;
    P entries = arena + 0x9000;
    for (int iter = 0; iter < 160; ++iter) {
        memset(arena, 0, 0x8000);
        int mode = iter % 3;
        for (int i = 0; i < 66; ++i) for (int j = 0; j < 4; ++j) {
            int v = rnd() & 0x3ff;
            if (mode == 1 && (rnd() & 3) == 0) v = -1; if (mode == 2 && (rnd() & 1)) v = -1 - (rnd() & 7);
            *reinterpret_cast<int*>(self + 424 + 52 * i + (j < 2 ? 4 * j : 24 + 4 * (j - 2))) = v;
        }
        int sp3856 = (iter & 4) ? -1 : 0x55;
        *reinterpret_cast<int*>(self + 3856) = sp3856;
        *reinterpret_cast<int*>(self + 416) = 0x41; *reinterpret_cast<int*>(self + 420) = 0x42;
        for (int k = 0; k < 19; ++k) *reinterpret_cast<int*>(self + 3908 + 24 * k) = (k % 3 == (iter & 3)) ? -1 : 0x300 + k;
        for (int k = 0; k < 3; ++k) *reinterpret_cast<int*>(self + 4372 + 12 * k) = ((k + iter) % 3 == 1) ? -1 : 0x400 + k;
        unsigned trail = (iter & 8) ? 0x1234 : 0; *reinterpret_cast<unsigned*>(self + 4576) = trail;
        int v92 = (iter & 16) ? -1 : 0x66; int v00 = (iter & 32) ? -1 : 0x67;
        *reinterpret_cast<int*>(self + 4592) = v92; *reinterpret_cast<int*>(self + 4600) = v00;
        int cnt = (iter >> 2) % 4; *reinterpret_cast<int*>(self + 4616) = cnt == 3 ? 0 : cnt; cnt = cnt == 3 ? 0 : cnt;
        *reinterpret_cast<unsigned*>(self + 4620) = word_of(entries);
        for (int i = 0; i < 4; ++i) { *reinterpret_cast<int*>(entries + 1056 * i) = (i == 1 && (iter & 64)) ? -1 : 0x500 + i; }
        P rs = (iter & 1) ? rast_ : 0; P sd = (iter & 2) ? snd_ : 0;
        baseRet = (iter % 11 == 5) ? -1 : 0;
        int budget = 400;
        failAt = (iter % 4 == 0) ? 1 + (iter * 7) % budget : 0;
        calls = 0; n_ = 0; expect_n = 0;
        int want = 0;
        ex(1, (intptr_t)self, (intptr_t)scene_, (intptr_t)dict_);
        if (baseRet < 0) want = -1;
        else {
            int c = 0; bool stop = false;
            auto T = [&](int id, intptr_t a, int b, intptr_t cc, bool used) { ex(id, a, b, cc); if (++c == failAt && used) stop = true; };
            // a call that returns -4 only matters where the image tests the result; the counter still advances
            for (int i = 0; i < 66 && !stop; ++i) {
                for (int j = 0; j < 2 && !stop; ++j) { int v = *reinterpret_cast<int*>(self + 424 + 52 * i + 4 * j); if (v >= 0) T(2, (intptr_t)scene_, v, 0, true); }
                for (int j = 0; j < 2 && !stop; ++j) if (sd) { int v = *reinterpret_cast<int*>(self + 448 + 52 * i + 4 * j); if (v >= 0) T(3, (intptr_t)snd_, v, (intptr_t)dict_, true); }
            }
            if (!stop && sp3856 != -1) T(4, (intptr_t)scene_, sp3856, 0, true);
            if (!stop && rs) {
                T(5, (intptr_t)rast_, 0x41, (intptr_t)dict_, true);
                if (!stop) T(6, (intptr_t)scene_, 0x42, 0, true);
                for (int k = 0; k < 19 && !stop; ++k) { int s = *reinterpret_cast<int*>(self + 3908 + 24 * k); if (s != -1) T(7, (intptr_t)rast_, s, (intptr_t)dict_, false); }
            }
            if (!stop) {
                for (int k = 0; k < 3; ++k) { int s = *reinterpret_cast<int*>(self + 4372 + 12 * k); if (s != -1) T(4, (intptr_t)scene_, s, 0, false); }
                if (trail) ex(8, trail);
                if (sd && v92 != -1) { if (v00 != -1) ex(9, (intptr_t)snd_, v00); T(3, (intptr_t)snd_, v92, (intptr_t)dict_, false); }
                for (int i = 0; i < cnt; ++i) { int id = *reinterpret_cast<int*>(entries + 1056 * i); if (id != -1) { ex(10, (intptr_t)(entries + 1056 * i + 16)); ex(11, (intptr_t)(scene_ + 1352), id, (intptr_t)dict_); } }
            }
            want = stop ? -1 : 0;
        }
        int got = Release__9VICSpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide(self, scene_, dict_, rs, sd, coll_);
        if (got != want || !same()) return iter + 1;
        if (want == 0 && *reinterpret_cast<unsigned*>(self + 4576) != 0) return iter + 1;
        if (want == -1 && baseRet == 0 && trail != 0 && *reinterpret_cast<unsigned*>(self + 4576) != trail) return iter + 1;
    }
    return 0;
}
