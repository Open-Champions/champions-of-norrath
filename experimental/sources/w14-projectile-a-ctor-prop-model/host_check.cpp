// Host contract for __4PropG6Point3P12_modelHeaderP8_textureli; not an EE input. 32-bit words are offsets from a host arena. Every
// callee is a stub that logs; the bounding-box stub fills the six integers from a table so the centre arithmetic, the rounding of
// shifted negative sums and the flag-4 branch can be checked against a recomputation. The authored function was also compared with
// the original in a private R5900 run (60 scenarios, every original word covered; mutants of the generator multiplier, the centre
// arithmetic, the flag bit, the cleared halfword and the resource words are caught there).
#define W14_PROJECTILE_A_HOST 1
#include "../w14-projectile-a-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w14_projectile_a_host_base;
unsigned w14_projectile_a_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
void w14_projectile_a_host_hurt(unsigned, void*, float, void*, int) {}
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static std::string calls; static int box[6];
static unsigned char* the_object;
extern "C" {
unsigned char vt_Prop[8] __asm__("_vt$4Prop");
void game_object_construct(void* self, Point3 p, void* map, char** names)
    __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls += "go(" + std::string(self == the_object ? "self" : "other") + "," + std::to_string(p.x) + "," + std::to_string(reinterpret_cast<unsigned long>(map)) + "," + std::to_string(reinterpret_cast<unsigned long>(names)) + ");";
}
void modelGetBoundingBox__FP12_modelHeaderlPiN52(void* h, long long l, int* a, int* b, int* c, int* d, int* e, int* f) {
    calls += "bbox(" + std::string(h == arena + 0x1000 ? "model" : "other") + "," + std::to_string(l) + ");";
    *a = box[0]; *b = box[1]; *c = box[2]; *d = box[3]; *e = box[4]; *f = box[5];
}
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
void init__4Propi(void*, int f) { calls += "init" + std::to_string(f) + ";"; }
}
#include "ctor.cpp"
int main() {
    w14_projectile_a_host_base = arena;
    int bad = 0;
    unsigned char* o = arena + 0x100;
    the_object = o;
    const int boxes[][6] = {{-10, -20, -30, 10, 20, 30}, {-7, -5, -3, 2, 4, 6}, {1, 2, 3, 4, 5, 6}, {-9, -9, -9, 0, 0, 0}};
    for (const auto& bx : boxes) for (int flags : {0, 4, 5, 3, 7}) {
        std::memcpy(box, bx, sizeof box);
        std::memset(o, 0x11, 0x200); calls.clear();
        float pos[3] = {1.0f, 2.0f, 3.0f}; std::memcpy(o + 16, pos, 12);
        const unsigned seed = 0x12345678u; std::memcpy(o + 68, &seed, 4);
        const long long big = -0x123456789abcdefLL;
        construct(o, Point3{9.0f, 8.0f, 7.0f}, arena + 0x1000, arena + 0x2000, big, flags);
        std::string want = "go(self,9.000000,0,0);bbox(model," + std::to_string(big) + ");";
        if (flags & 4) want += "grid;";
        want += "init" + std::to_string(flags) + ";";
        if (calls != want) { if (!bad) std::printf("calls %s\nwant  %s\n", calls.c_str(), want.c_str()); bad = 1; }
        using namespace w14pa;
        const unsigned w = seed * 0x19660du + 0x3c6ef35fu;
        if (word(o, 68) != w || static_cast<unsigned short>(half(o, 56)) != (w >> 16) || dword(o, 232) != static_cast<unsigned long long>(big)) bad = 2;
        if (real(o, 48) != 0 || real(o, 252) != 0 || real(o, 256) != 0 || half(o, 268) != 0 || word(o, 72) != to_word(vt_Prop)) bad = 3;
        if (word(o, 76) != 0x1000u || word(o, 80) != 0x2000u) bad = 4;
        const float cx = float((bx[0] + bx[3]) >> 1), cy = float((bx[1] + bx[4]) >> 1), cz = float((bx[2] + bx[5]) >> 1);
        if (real(o, 240) != cx || real(o, 244) != cy || real(o, 248) != cz || real(o, 264) != cz - float(bx[2])) bad = 5;
        if (real(o, 16) != 1.0f + ((flags & 4) ? cx : 0) || real(o, 20) != 2.0f + ((flags & 4) ? cy : 0) || real(o, 24) != 3.0f + ((flags & 4) ? cz : 0)) bad = 6;
        if (o[270] != 0x11) bad = 7;
        if (o[300] != 0x11) bad = 8;
        if (o[231] != 0x11) bad = 9;
    }
    return bad;
}
