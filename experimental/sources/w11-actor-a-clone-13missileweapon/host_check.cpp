// Host contract for clone__13MissileWeapon; not an EE input. Includes the source.
#include "clone.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char vt_10GameObject[8], vt_10Projectile[8], vt_13MissileWeapon[8];
static unsigned char arena[516] __attribute__((aligned(16)));
static unsigned char src[516] __attribute__((aligned(16)));
static bool failNext;
static std::string calls;
static unsigned lo(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
static unsigned word72() { unsigned w; std::memcpy(&w, arena + 72, 4); return w; }
static unsigned tableAtCall[32];
static int callCount;
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return failNext ? nullptr : arena; }
void ListHead_copy(void* d, const void* s) {
    calls += "lh" + std::to_string(static_cast<unsigned char*>(d) - arena) + "<" + std::to_string(static_cast<const unsigned char*>(s) - src) + ";";
    tableAtCall[callCount++] = word72(); std::memset(d, 0, 4);
}
static unsigned lightTable;
void lightAdd__FP5Light(void* l) {
    unsigned w[2]; std::memcpy(w, l, 8);
    calls += "light" + std::to_string(static_cast<unsigned char*>(l) - arena) + (w[0] || w[1] ? "dirty" : "clear") + ";";
    lightTable = word72();
}
int main() {
    int bad = 0;
    for (int i = 0; i < 516; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
    failNext = true;
    std::memset(arena, 0xa5, sizeof arena);
    if (clone__13MissileWeapon(src) != nullptr || calls != "alloc452;") { std::puts("fail: null allocation"); bad = 1; }
    failNext = false;
    const unsigned heads[][2] = {{0,0}, {4,0}, {8,0}, {76,0}, {92,0}, {116,0}, {140,0}, {164,0}, {188,0}, {292,0}};
    const unsigned char* tables[] = {vt_10GameObject, vt_10GameObject, vt_10GameObject, vt_10Projectile, vt_13MissileWeapon, vt_13MissileWeapon, vt_13MissileWeapon, vt_13MissileWeapon, vt_13MissileWeapon, vt_13MissileWeapon};
    const unsigned ranges[][2] = {{12,41},{44,57},{60,71},{80,80},{84,91},{96,107},{108,115},{120,131},{132,139},{144,155},{156,163},{168,179},{180,187},{192,203},{204,205},{208,248},{250,254},{256,291},{296,367},{368,427},{428,451}};
    for (int mode = 0; mode < 4; ++mode) {
        for (int i = 0; i < 516; ++i) src[i] = static_cast<unsigned char>(i * 31 + 7);
        if (!(mode & 1)) std::memset(src + 368, 0, 4);
        if (!(mode & 2)) std::memset(src + 372, 0, 4);
        std::memset(arena, 0xa5, sizeof arena); calls.clear(); callCount = 0; lightTable = 0;
        void* r = clone__13MissileWeapon(src);
        std::string want = "alloc452;";
        for (auto& h : heads) want += "lh" + std::to_string(h[0]) + "<" + std::to_string(h[0]) + ";";
        if (mode) want += "light368clear;";
        if (r != arena || calls != want) { std::printf("fail: mode %d calls %s\n", mode, calls.c_str()); bad = 1; }
        for (unsigned k = 0; k < sizeof tables / sizeof tables[0]; ++k)
            if (tableAtCall[k] != lo(tables[k])) { std::printf("fail: table word at call %u\n", k); bad = 1; }
        if (mode && lightTable != lo(vt_13MissileWeapon)) { std::puts("fail: table word at lightAdd"); bad = 1; }
        if (word72() != lo(vt_13MissileWeapon)) { std::puts("fail: final table pointer"); bad = 1; }
        static bool copied[516];
        for (auto& rg : ranges) for (unsigned i = rg[0]; i <= rg[1]; ++i) copied[i] = true;
        for (unsigned i = 0; i < 516; ++i) {
            bool head = (i >= 0 && i < 4) || (i >= 4 && i < 8) || (i >= 8 && i < 12) || (i >= 76 && i < 80) || (i >= 92 && i < 96) || (i >= 116 && i < 120) || (i >= 140 && i < 144) || (i >= 164 && i < 168) || (i >= 188 && i < 192) || (i >= 292 && i < 296);
            bool cleared = mode && i >= 368 && i < 376;
            if (i >= 72 && i < 76) continue;
            if (head || cleared) { if (arena[i] != 0) { std::printf("fail: byte %u not cleared\n", i); bad = 1; break; } continue; }
            if (copied[i] && arena[i] != src[i]) { std::printf("fail: byte %u not copied\n", i); bad = 1; break; }
            if (!copied[i] && arena[i] != 0xa5) { std::printf("fail: byte %u written outside the ranges\n", i); bad = 1; break; }
        }
    }
    return bad;
}
