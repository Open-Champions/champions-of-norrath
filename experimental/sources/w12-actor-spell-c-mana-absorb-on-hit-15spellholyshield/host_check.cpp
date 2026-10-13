// Host contract for manaAbsorbOnHit__15SpellHolyShield; not an EE input. 32-bit words are offsets from a host base
// and the callees log. The authored function was also compared with the original in a private R5900 run (13
// scenarios: shadow-point counts, the S+28 test around zero, allocation success and failure, signed bytes and
// returns; every one of its 216 words reached).
#define W12_SPELL_C_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string calls;
static int absorbed;
unsigned w12_spell_c_host_call(unsigned function_word, void* adjusted_object, int argument_count, unsigned long a1, unsigned long a2) {
    calls += "slot(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted_object) - w12_spell_c_host_base) + "," +
             std::to_string(argument_count) + "," + std::to_string(static_cast<long>(a1)) + "," + std::to_string(static_cast<long>(static_cast<int>(a2))) + ");";
    return function_word == 0x1111 ? 0x7000u : static_cast<unsigned>(absorbed);
}
extern "C" { unsigned char holyshield_particle_8d7720[80]; }
#include "mana_absorb.cpp"

static int rollIndex;
static int rolls[16], rollCount;
static int modelCount, particles, lights, allocFail;
static std::string num(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
int eRand__5Spell(void*) { return rolls[rollIndex++ % rollCount]; }
void P_OpenParticleSystem__Fi(int s) { calls += "open(" + std::to_string(s) + ");"; }
void P_CloseParticleSystem__Fi(int s) { calls += "close(" + std::to_string(s) + ");"; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float s) {
    ++particles;
    if (particles == 1 || particles == 241) calls += "particle(" + num(p->x) + ":" + num(p->y) + ":" + num(p->z) + "," + num(v->x) + ":" + num(v->y) + ":" + num(v->z) + "," + num(s) + "," + (d == holyshield_particle_8d7720 ? "def" : "?") + ");";
}
int modelGetNmShadowPoints__FP12_modelHeader(void* m) { calls += "points(" + std::to_string(reinterpret_cast<unsigned long>(m)) + ");"; return modelCount; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* m, void* anim, Point3 pos, short angle, int index, Point3* point, float* extra, void* matrix, float scale) {
    if (particles == 0) calls += "shadow(" + std::to_string(reinterpret_cast<unsigned long>(m)) + "," + std::to_string(static_cast<unsigned char*>(anim) - w12_spell_c_host_base) + "," + num(pos.x) + ":" + num(pos.y) + ":" + num(pos.z) +
                                  "," + std::to_string(angle) + "," + std::to_string(index) + "," + std::to_string(matrix == nullptr) + "," + num(scale) + ");";
    point->x = static_cast<float>(index); point->y = 2.0f; point->z = 3.0f; *extra = 0;
}
void* blockAlloc__Fi(int n) { calls += "alloc(" + std::to_string(n) + ");"; return allocFail ? nullptr : reinterpret_cast<void*>(0x4000); }
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(void*, const Point3* p, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10,
                                                       float p11, float p12, float one, float scaled, void* object, int flag) {
    ++lights;
    calls += "light(" + num(p->x) + ":" + num(p->y) + ":" + num(p->z) + "," + num(p3) + "," + num(p4) + "," + num(p5) + "," + num(p6) + "," + num(p7) + "," + num(p8) +
             "," + num(p9) + "," + num(p10) + "," + num(p11) + "," + num(p12) + "," + num(one) + "," + num(scaled) + "," + std::to_string(object == nullptr) + "," + std::to_string(flag) + ");";
}
}

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static unsigned char* const spell = arena + 0x100;
static unsigned char* const owner = arena + 0x400;
static unsigned char* const target = arena + 0x800;
static unsigned char* const ownerTable = arena + 0xc00;
static unsigned char* const targetTable = arena + 0xd00;
static void put32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
static float getf(unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static void reset(float f28) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); rollIndex = 0; particles = 0; lights = 0; allocFail = 0; modelCount = 4; absorbed = 250;
    put32(spell + 12, static_cast<unsigned>(owner - arena)); put32(spell + 8, static_cast<unsigned>(target - arena));
    put32(spell + 36, 9); putf(spell + 28, f28); spell[0] = 0xfe;
    putf(owner + 16, 1); putf(owner + 20, 2); putf(owner + 24, 3);
    short hw = 90; std::memcpy(owner + 56, &hw, 2);
    put32(owner + 72, static_cast<unsigned>(ownerTable - arena));
    short adjust = 16; std::memcpy(ownerTable + 128, &adjust, 2); put32(ownerTable + 132, 0x1111);
    put32(target + 96, static_cast<unsigned>(targetTable - arena));
    short tadjust = -8; std::memcpy(targetTable + 88, &tadjust, 2); put32(targetTable + 92, 0x2222);
    rolls[0] = 32768; rollCount = 1;
}

int main() {
    int bad = 0;
    reset(0.0f);   // all rolls 32768: the direction is (0, 0, 0)
    const float r = manaAbsorbOnHit__15SpellHolyShield(spell);
    const std::string head = "slot(4369," + std::to_string(owner - arena + 16) + ",0,0,0);points(28672);slot(4369," + std::to_string(owner - arena + 16) + ",0,0,0);open(9);";
    if (calls.compare(0, head.size(), head) != 0) { std::puts(calls.c_str()); std::puts("fail: opening calls"); bad = 1; }
    if (calls.find("shadow(28672," + std::to_string(owner + 288 - arena) + ",1:2:3,90,0,1,1);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: shadow call"); bad = 1; }
    if (particles != 241) { std::printf("fail: %d particles\n", particles); bad = 1; }
    if (calls.find("close(9);alloc(188);light(1:2:63,64,64,125,0,0,0,0,0,120,60,0.5,0.263002,1,0);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: light"); bad = 1; }
    const std::string tail = "slot(8738," + std::to_string(target - arena - 8) + ",2,0,-2);";
    if (calls.size() < tail.size() || calls.compare(calls.size() - tail.size(), tail.size(), tail) != 0) { std::puts(calls.c_str()); std::puts("fail: absorb slot"); bad = 1; }
    if (getf(spell + 28) != 0.263002f && std::memcmp(spell + 28, "\x32\xa8\x86\x3e", 4) != 0) { std::puts("fail: S+28 after"); bad = 1; }
    if (r != 250.0f * 0.0099999998f) { std::printf("fail: return %g\n", r); bad = 1; }
    // already positive: no light, no change
    reset(0.5f);
    manaAbsorbOnHit__15SpellHolyShield(spell);
    if (calls.find("alloc") != std::string::npos || getf(spell + 28) != 0.5f) { std::puts("fail: positive S+28"); bad = 1; }
    // allocation failure: no light call, but the float is still set
    reset(-1.0f); allocFail = 1;
    manaAbsorbOnHit__15SpellHolyShield(spell);
    if (calls.find("alloc(188);") == std::string::npos || lights != 0 || getf(spell + 28) <= 0.0f) { std::puts("fail: alloc failure"); bad = 1; }
    // rejection and scaling: first draw (65535, 65535, 65535) is outside the ball, then rolls of 49152 give 0.5 each (0.75 sum of squares)
    reset(1.0f);
    {
        static const int seq[] = {0, 65535, 65535, 65535, 49152, 49152, 49152};
        for (int i = 0; i < 7; ++i) rolls[i] = seq[i];
        rollCount = 7;
        manaAbsorbOnHit__15SpellHolyShield(spell);
        if (calls.find("particle(0:2:3,18.5:18.5:18.5,0,def);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: rejection"); bad = 1; }
    }
    return bad;
}
