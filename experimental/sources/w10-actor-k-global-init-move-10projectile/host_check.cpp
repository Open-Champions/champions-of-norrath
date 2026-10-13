// Host contract for _GLOBAL_$I$move__10ProjectileG6Point3; not an EE input. The two constructors
// are stubs that record their arguments. The contract checks the call count and order by class,
// that the 19 targets are distinct, that the morph-effect objects come last and are the global
// symbols, and the full argument tuple of the first and last particle constructions.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
struct Call { bool particle; void* obj; int ints[8]; float f[3]; int tail[3]; std::string s1, s2; };
static std::vector<Call> calls;
extern "C" {
unsigned char goldAnm[16], goldModel[16], goldTex[16];
void __11ParticleDefiiiiiiiifffiiiPFPv_v(void* self, int a, int b, int c, int d, int e, int f, int g, int h,
    float x, float y, float z, int i, int j, int k, void (*callback)(void*)) {
    Call c0{}; c0.particle = true; c0.obj = self;
    const int v[8] = {a, b, c, d, e, f, g, h}; std::memcpy(c0.ints, v, sizeof v);
    c0.f[0] = x; c0.f[1] = y; c0.f[2] = z; c0.tail[0] = i; c0.tail[1] = j; c0.tail[2] = k;
    if (callback) c0.s1 = "callback";
    calls.push_back(c0);
}
void __11ResourceMapPcT1(void* self, const char* lump, const char* name) {
    Call c0{}; c0.particle = false; c0.obj = self; c0.s1 = lump; c0.s2 = name; calls.push_back(c0);
}
}
#include "static_init.cpp"
int main() {
    int bad = 0;
    _GLOBAL_$I$move__10ProjectileG6Point3();
    if (calls.size() != 19) { std::printf("count %zu\n", calls.size()); return 1; }
    int particles = 0, maps = 0;
    for (size_t i = 0; i < calls.size(); ++i) {
        if (calls[i].particle) ++particles; else ++maps;
        for (size_t j = 0; j < i; ++j) if (calls[i].obj == calls[j].obj) bad = 1;
    }
    if (particles != 15 || maps != 4) bad = 1;
    for (int i = 6; i < 10; ++i) if (calls[i].particle) bad = 1;
    if (calls[0].ints[0] != 238 || calls[0].ints[7] != -100 || calls[0].f[0] != bits(0x41700000u) || calls[0].f[1] != bits(0xc0200000u) ||
        calls[0].f[2] != bits(0x3f733333u) || calls[0].tail[0] != 1 || calls[0].tail[1] != 1 || calls[0].tail[2] != 0 || !calls[0].s1.empty()) bad = 1;
    if (calls[6].s1 != "fx.lmp" || calls[6].s2 != "goldpile_fall.anm" || calls[6].obj != goldAnm) bad = 1;
    if (calls[7].s2 != "goldpile.vif" || calls[7].obj != goldModel) bad = 1;
    if (calls[8].s2 != "goldpile.tex" || calls[8].obj != goldTex) bad = 1;
    if (calls[9].s2 != "goldpile_rest.vif") bad = 1;
    const Call& m1 = calls[17]; const Call& m2 = calls[18];
    if (m1.obj != morpheffectParticle1 || m2.obj != morpheffectParticle2) bad = 1;
    if (m1.ints[0] != 220 || m1.ints[1] != 20 || m1.ints[7] != 150 || m1.tail[0] != 2 || m1.tail[1] != 1 || m1.tail[2] != 8 || m1.f[0] != 15.0f || m1.f[1] != 15.0f || m1.f[2] != 0.0f) bad = 1;
    if (m2.ints[0] != 255 || m2.ints[7] != 255 || m2.tail[0] != 0 || m2.tail[1] != 1 || m2.tail[2] != 8 || m2.f[0] != 30.0f) bad = 1;
    return bad;
}
