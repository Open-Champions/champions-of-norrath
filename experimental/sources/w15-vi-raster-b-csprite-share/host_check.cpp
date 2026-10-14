// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 17];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
extern "C" unsigned norrath_host_address(const unsigned char* p) { return arena_base + static_cast<unsigned>(p - arena); }
static int kind[512], ident[512]; static const void* target[512]; static int n, fail_kind, fail_id, fail_nth, hits;
static int rec(int k, const void* t, int id) {
    kind[n] = k; ident[n] = id; target[n++] = t;
    if (k == fail_kind && id == fail_id && ++hits == fail_nth) return -1;
    return 0;
}
extern "C" int ShareAnimation__7VIScenei(unsigned char* s, int id) { return rec(1, s, id); }
extern "C" int ShareSound__13VISoundDevicei(unsigned char* d, int id) { return rec(2, d, id); }
extern "C" int ShareSprite__7VIScenei(unsigned char* s, int id) { return rec(3, s, id); }
extern "C" int ShareMaterialPal__8VIRasteri(unsigned char* r, int id) { return rec(4, r, id); }
extern "C" int ShareRefMap__7VIScenei(unsigned char* s, int id) { return rec(5, s, id); }
extern "C" int ShareSurface__8VIRasteri(unsigned char* r, int id) { return rec(6, r, id); }
extern "C" int ShareParticleDefinition__16VIParticleSystemi(unsigned char* p, int id) { return rec(7, p, id); }
#include "share.cpp"
#include <cstdio>
static int& I(unsigned off) { return *reinterpret_cast<int*>(arena + off); }
static const unsigned SCENE = 0x10000, RASTER = 0x14000, SOUND = 0x15000, ARRAY = 0x16000;
static void reset(bool sound) {
    std::memset(arena, 0xff, 0x6000);                       // every id is -1 (skipped)
    n = 0; fail_kind = 0; fail_id = 0; fail_nth = 0; hits = 0;
    I(SCENE + 19360) = sound ? (int)(arena_base + SOUND) : 0;
    I(4616) = 0; I(4620) = (int)(arena_base + ARRAY);
}
int main() {
    bool ok = true;
    unsigned char* self = arena; unsigned char* scene = arena + SCENE; unsigned char* raster = arena + RASTER;
    // nothing to share: only the unconditional ref map for word +384 (value -1) is called
    reset(true);
    ok = ok && Share__9VICSpriteR7VISceneP8VIRaster(self, scene, nullptr) == 0 && n == 1 && kind[0] == 5 && ident[0] == -1 && target[0] == scene;
    // fill: record 0 ids, record 65 ids, odd ones negative-but-not -1 (skipped by the >= 0 test)
    reset(true);
    I(424) = 11; I(428) = -7; I(448) = 21; I(452) = 22;
    I(424 + 52 * 65) = 31; I(428 + 52 * 65) = 32; I(448 + 52 * 65) = -2; I(452 + 52 * 65) = 33;
    I(3856) = 40; I(416) = 50; I(420) = 51; I(388) = 60; I(384) = 61;
    I(3908 + 24 * 0) = 70; I(3908 + 24 * 18) = 71; I(4592) = 80;
    I(4616) = 2; I(ARRAY) = 90; I(ARRAY + 1056) = 91;
    int want[][2] = {{1, 11}, {2, 21}, {2, 22}, {1, 31}, {1, 32}, {2, 33}, {3, 40}, {4, 50}, {5, 51}, {1, 60}, {5, 61}, {6, 70}, {6, 71}, {2, 80}, {7, 90}, {7, 91}};
    ok = ok && Share__9VICSpriteR7VISceneP8VIRaster(self, scene, raster) == 0 && n == 16;
    for (int i = 0; i < 16 && i < n; ++i) ok = ok && kind[i] == want[i][0] && ident[i] == want[i][1];
    ok = ok && target[0] == scene && target[1] == arena + SOUND && target[7] == raster && target[8] == scene && target[11] == raster && target[14] == scene + 1352;
    // no sound device: sound ids never go out, not even the +4592 one
    reset(false);
    I(448) = 21; I(4592) = 80; I(424) = 11;
    ok = ok && Share__9VICSpriteR7VISceneP8VIRaster(self, scene, nullptr) == 0 && n == 2 && kind[0] == 1 && kind[1] == 5;
    // no raster: no material pal, ref map +420, or surfaces
    reset(true); I(416) = 50; I(420) = 51; I(3908) = 70;
    ok = ok && Share__9VICSpriteR7VISceneP8VIRaster(self, scene, nullptr) == 0 && n == 1;
    // each checked call can abort; the unchecked ones cannot
    struct F { int k, id; int expect_n; } fails[] = {{1, 11, 1}, {2, 21, 2}, {3, 40, 7}, {4, 50, 8}, {5, 51, 9}, {1, 60, 10}, {5, 61, 11}};
    for (const F& f : fails) {
        reset(true);
        I(424) = 11; I(448) = 21; I(3856) = 40; I(416) = 50; I(420) = 51; I(388) = 60; I(384) = 61; I(3908) = 70; I(4592) = 80; I(4616) = 1; I(ARRAY) = 90;
        fail_kind = f.k; fail_id = f.id; fail_nth = 1;
        int r = Share__9VICSpriteR7VISceneP8VIRaster(self, scene, raster);
        ok = ok && r == -1;
        if (f.k == 1 && f.id == 11) ok = ok && n == 1;
        if (f.k == 2) ok = ok && n == 2;
    }
    reset(true); I(3908) = 70; I(4592) = 80; I(4616) = 1; I(ARRAY) = 90; fail_kind = 6; fail_id = 70; fail_nth = 1;
    ok = ok && Share__9VICSpriteR7VISceneP8VIRaster(self, scene, raster) == 0;
    // an array count of zero or below skips the particle loop
    reset(true); I(4616) = 0; I(ARRAY) = 90;
    ok = ok && Share__9VICSpriteR7VISceneP8VIRaster(self, scene, raster) == 0 && n == 3;   // two ref maps and the material pal
    reset(true); I(4616) = -4; I(ARRAY) = 90;
    ok = ok && Share__9VICSpriteR7VISceneP8VIRaster(self, scene, raster) == 0 && n == 3;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
