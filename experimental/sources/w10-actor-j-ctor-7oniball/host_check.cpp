// Host contract for the OniBall constructor; not an EE input. Callees are stubs that log; random
// values come from a list. The authored function was also compared with the original in a private
// R5900 run (120 scenarios, all 111 words covered).
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls;
static unsigned rolls[8]; static int rollIndex;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
#include "oniball.cpp"
extern "C" {
int frameNm;
unsigned char vt_OniBall[8];
void projectile_construct(void* self, Point3 p, void* owner, void* map) {
    calls += "proj(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + (owner ? "o" : "null") + "," + (map ? "m" : "null") + ");";
    float f[3] = {100, 200, 300}; unsigned w = 0x100;
    std::memcpy(static_cast<unsigned char*>(self) + 16, f, 12); std::memcpy(static_cast<unsigned char*>(self) + 12, &w, 4);
}
unsigned eRand__10GameObject(void*) { calls += "rand;"; return rolls[rollIndex++]; }
void objectAddToRunList__FP10GameObject(void*) { calls += "runlist;"; }
}
static float getf(unsigned char* o, unsigned off) { float v; std::memcpy(&v, o + off, 4); return v; }
static unsigned get32(unsigned char* o, unsigned off) { unsigned v; std::memcpy(&v, o + off, 4); return v; }
int main() {
    alignas(16) static unsigned char o[256], owner[1024];
    int bad = 0;
    for (int which = 0; which < 2; ++which) {
        std::memset(o, 0x11, sizeof o); std::memset(owner, 0, sizeof owner);
        calls.clear(); rollIndex = 0; frameNm = 1000;
        rolls[0] = 0x12345678; rolls[1] = which ? 29491u : 0x20000u + 29490u; rolls[2] = 205; rolls[3] = 317; rolls[4] = 4321;
        float hp = 400.0f; std::memcpy(owner + 564, &hp, 4);
        oniball_construct(o, Point3{1, 2, 3}, 7, owner, nullptr);
        const std::string want = "proj(1:2:3,null,null);rand;rand;runlist;rand;rand;rand;";
        const bool ok = calls == want && get32(o, 72) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_OniBall)) &&
            o[128] == 0 && o[129] == 0 && get32(o, 28) == 0 && get32(o, 12) == (0x100u | 0x4000u) && o[56] == 0x78 && o[57] == 0x56 &&
            get32(o, 84) == 1600 && o[116] == (which ? 1 : 0) && get32(owner, which ? 936 : 928) == 30 && get32(owner, which ? 928 : 936) == 0 &&
            get32(owner, 252) == 1000 && static_cast<int>(get32(o, 88)) == 19 && getf(o, 24) == 336.0f &&
            getf(o, 104) == 100.0f - 50.0f + 5.0f && getf(o, 108) == 200.0f - 50.0f + 17.0f && getf(o, 112) == 336.0f + 321.0f + 36.0f;
        if (!ok) { std::printf("fail %d: %s\n", which, calls.c_str()); bad = 1; }
    }
    return bad;
}
