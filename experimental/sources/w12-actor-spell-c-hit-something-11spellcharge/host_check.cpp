// Host contract for hitSomething__11SpellChargeP10GameObject; not an EE input. 32-bit words are offsets from a host
// base and the callees log. The authored function was also compared with the original in a private R5900 run (87
// scenarios: the done flag, a null and a non-null hit, flag bits on the hit object and on listed objects, list shapes
// including the owner and the hit object, zero and negative radius values; all 399 original words reached).
#define W12_SPELL_C_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string calls;
static int slot88Values[3];
unsigned w12_spell_c_host_call(unsigned function_word, void* adjusted_object, int argument_count, unsigned long a1, unsigned long a2) {
    const long off = static_cast<unsigned char*>(adjusted_object) - w12_spell_c_host_base;
    if (function_word == 0x88) {
        calls += "s88(" + std::to_string(off) + "," + std::to_string(static_cast<long>(a1)) + "," + std::to_string(static_cast<long>(static_cast<int>(a2))) + ");";
        return static_cast<unsigned>(slot88Values[a1 % 3]);
    }
    calls += "s16(" + std::to_string(off) + "," + std::to_string(argument_count) + "," + std::to_string(a1) + "," + std::to_string(a2) + ");";
    return 0;
}
extern "C" { unsigned char spellcharge_hit_something_particle_8d6780[80]; }
#include "hit_something.cpp"

static int rollIndex, rollCount;
static int rolls[8];
static int listCount;
static void* listItems[8];
static int particles, effects;
static std::string num(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
int eRand__5Spell(void*) { calls += "r;"; return rolls[rollIndex++ % rollCount]; }
float icos__Fi(int) { return 0.5f; }
float isin__Fi(int) { return 0.25f; }
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return reinterpret_cast<void*>(0x77); }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float v) { calls += "sound(" + num(v) + ");"; }
void gameSetCameraShake__Ffb(float f, bool b) { calls += "shake(" + num(f) + "," + std::to_string(b) + ");"; }
void addEffect__FiG6Point3(int e, Point3 p) { ++effects; calls += "effect(" + std::to_string(e) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + ");"; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void*, Point3* p, Point3* v, float) {
    ++particles;
    if (particles <= 2) calls += "particle(" + num(p->x) + ":" + num(p->y) + ":" + num(p->z) + "," + num(v->x) + ":" + num(v->y) + ":" + num(v->z) + ");";
}
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 lo, Point3 hi, void** out, int cap) {
    calls += "box(" + num(lo.x) + ":" + num(lo.y) + ":" + num(lo.z) + "," + num(hi.x) + ":" + num(hi.y) + ":" + num(hi.z) + "," + std::to_string(cap) + ");";
    for (int i = 0; i < listCount; ++i) out[i] = listItems[i];
    out[listCount] = nullptr;
    return listCount;
}
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* t, void* a, float lo, float hi, int c1, int c2, int c3, int c4, void* d, bool f) {
    calls += "attack(" + std::to_string(static_cast<unsigned char*>(t) - w12_spell_c_host_base) + "," + std::to_string(static_cast<unsigned char*>(a) - w12_spell_c_host_base) +
             "," + num(lo) + "," + num(hi) + "," + std::to_string(c1) + "," + std::to_string(c2) + "," + std::to_string(c3) + "," + std::to_string(c4) + "," + std::to_string(d == nullptr) + "," + std::to_string(f) + ");";
}
}

static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const spell = arena + 0x100;
static unsigned char* const owner = arena + 0x400;
static unsigned char* const target = arena + 0x800;
static unsigned char* const hitObj = arena + 0xc00;
static unsigned char* const other = arena + 0x1000;
static unsigned char* const targetTable = arena + 0x1400;
static unsigned char* const objTable = arena + 0x1500;
static void put32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
static unsigned get32(unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static void reset() {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); rollIndex = 0; rolls[0] = 32768; rollCount = 1; listCount = 0; particles = 0; effects = 0;
    slot88Values[0] = 100; slot88Values[1] = 7; slot88Values[2] = 0;
    put32(spell + 12, static_cast<unsigned>(owner - arena)); put32(spell + 8, static_cast<unsigned>(target - arena)); spell[0] = 0xfe;
    putf(owner + 16, 10); putf(owner + 20, 20); putf(owner + 24, 30);
    put32(target + 96, static_cast<unsigned>(targetTable - arena));
    short a88 = 0; std::memcpy(targetTable + 88, &a88, 2); put32(targetTable + 92, 0x88);
    for (unsigned char* o : {hitObj, other}) { put32(o + 72, static_cast<unsigned>(objTable - arena)); }
    short a16 = 4; std::memcpy(objTable + 16, &a16, 2); put32(objTable + 20, 0x16);
}

int main() {
    int bad = 0;
    reset(); put32(spell + 72, 1);
    hitSomething__11SpellChargeP10GameObject(spell, hitObj);
    if (!calls.empty()) { std::puts("fail: already done"); bad = 1; }
    reset();   // null hit: eight effects, all rolls 32768 (remainders 32768 % 12 = 8, % 72 = 8)
    hitSomething__11SpellChargeP10GameObject(spell, nullptr);
    if (calls.compare(0, std::strlen("lump(fx.lmp,spel_bw_bull_hit.anm);sound(1);shake(10,0);"), "lump(fx.lmp,spel_bw_bull_hit.anm);sound(1);shake(10,0);") != 0 || effects != 8 || get32(spell + 72) != 1 ||
        calls.find("effect(2,12:22:38);") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: null hit"); bad = 1; }
    reset();   // plain hit: A = 100 damage on a non-player object, B = 0 so no splash
    hitSomething__11SpellChargeP10GameObject(spell, hitObj);
    {
        const std::string want = "lump(fx.lmp,spel_bw_bull_hit.anm);sound(1);shake(10,0);s88(" + std::to_string(target - arena) + ",0,-2);s88(" + std::to_string(target - arena) +
                                 ",2,-2);r;attack(" + std::to_string(hitObj - arena) + "," + std::to_string(owner - arena) + ",100,110,100000,0,0,573504,1,0);";
        if (calls != want || get32(spell + 72) != 1) { std::puts(calls.c_str()); std::puts("fail: plain hit"); bad = 1; }
    }
    reset(); put32(hitObj + 12, 0x40);   // flagged object: slot 16 instead of damage
    hitSomething__11SpellChargeP10GameObject(spell, hitObj);
    if (calls.find("s16(" + std::to_string(hitObj - arena + 4) + ",2," + std::to_string(owner - arena) + ",262144);") == std::string::npos || calls.find("attack") != std::string::npos) {
        std::puts(calls.c_str()); std::puts("fail: flagged object"); bad = 1;
    }
    reset(); slot88Values[2] = 3; listCount = 3; listItems[0] = owner; listItems[1] = other; listItems[2] = hitObj;   // splash radius 36; the owner and the hit object are skipped
    hitSomething__11SpellChargeP10GameObject(spell, hitObj);
    if (calls.find("box(-26:-16:-6,46:56:66,128);") == std::string::npos || particles != 264 || calls.find("attack(" + std::to_string(other - arena)) == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: splash"); bad = 1; }
    if (calls.find("attack(" + std::to_string(owner - arena)) != std::string::npos) { std::puts("fail: owner hit"); bad = 1; }
    {   // the listed object is attacked with C = the mode-1 value (7), once; the hit object only with A
        size_t n = 0, pos = 0; while ((pos = calls.find("attack(" + std::to_string(other - arena), pos)) != std::string::npos) { ++n; ++pos; }
        if (n != 1 || calls.find("attack(" + std::to_string(other - arena) + "," + std::to_string(owner - arena) + ",7,") == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: listed attack"); bad = 1; }
    }
    reset(); slot88Values[2] = 3; listCount = 1; listItems[0] = other; put32(other + 12, 0x8);   // flag 0x8 objects are skipped
    hitSomething__11SpellChargeP10GameObject(spell, hitObj);
    if (calls.find("attack(" + std::to_string(other - arena)) != std::string::npos) { std::puts("fail: flag 8"); bad = 1; }
    return bad;
}
