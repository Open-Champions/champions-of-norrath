// Host contract for msg_run__13MissileWeapon; not an EE input. 32-bit words are offsets from a host base; every
// callee logs its arguments (strings by content; the kind-5 snow loop reads an indeterminate local, which the host
// check pins to a known value through the stale-word hook below). Scenarios give the exact call log for the short
// paths and independently recomputed particle counts and random states for the loops. The authored function was
// also compared with the original in a private R5900 run (900 scenarios). Build with -ffp-contract=off. Returns
// non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_a_host_base;
#include "msg_run.cpp"

extern "C" int host_sprintf(char* b, const char* f, ...) __asm__("sprintf");
extern "C" int host_sprintf(char* b, const char* f, ...) { va_list ap; va_start(ap, f); const int n = std::vsprintf(b, f, ap); va_end(ap); return n; }
int levelFlags, frameNm;
unsigned char ballSparkParticle[80], trailParticleFire[80], trailParticleSmoke[80], snowParticle1[80];
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static int activeResult, moveResult, moveSetsState, featRank, allocFail, allocCount;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%.9g", v); return b; }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
static unsigned off(const void* p) { return p ? static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena) : 0u; }
static int particles[4];
unsigned w11_actor_a_host_call(unsigned fn, void* adjusted, int argc, unsigned long a1, unsigned long) {
    calls += "table(" + num(fn) + "," + num(off(adjusted)) + "," + num(argc) + "," + num(a1) + ");"; return 0;
}
extern "C" {
int objectActiveProcessing__FP10GameObjectii(void* o, int a, int b) { calls += "active(" + num(off(o)) + "," + num(a) + "," + num(b) + ");"; return activeResult; }
void objectAddToDeleteList__FP10GameObject(void* o) { calls += "queue(" + num(off(o)) + ");"; }
int move__10ProjectileG6Point3(void* o, Point3 v) { calls += "move(" + pt(v) + ");"; if (moveSetsState >= 0) obj[204] = static_cast<unsigned char>(moveSetsState); return moveResult; }
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return arena + 0x7000; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* a, void* o, float v) { calls += "sequence(" + num(off(a)) + "," + num(off(o)) + "," + num(v) + ");"; }
void addEffect__FiG6Point3(int e, Point3 p) { calls += "effect(" + num(e) + "," + pt(p) + ");"; }
void soundPlaySound__FP10_vagHeaderP6Point3fff(void* h, Point3* p, float a, float b, float c) { calls += "sound(" + num(off(h)) + "," + pt(*p) + "," + num(a) + "," + num(b) + "," + num(c) + ");"; }
void __12ParticlePropG6Point3i(void* s, Point3 p, int n) { calls += "pprop(" + num(off(s)) + "," + pt(p) + "," + num(n) + ");"; }
void* blockAlloc__Fi(int n) { ++allocCount; calls += "alloc" + num(n) + ";"; return (allocFail && allocCount % allocFail == 0) ? nullptr : arena + 0x6000 + 0x200 * allocCount; }
void makeExplosion__FG6Point3f(Point3 p, float s) { calls += "explode(" + pt(p) + "," + num(s) + ");"; }
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* o, Point3 p, int a, int b, float c, float d, int e, int f) {
    calls += "radial(" + num(off(o)) + "," + pt(p) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + ");";
}
int playerGetFeatRank__Fix(int p, long long f) { calls += "feat(" + num(p) + "," + num(static_cast<double>(f)) + ");"; return featRank; }
void fxRunBallLightning__FP20fxBallLightningStateP10GameObjectT1G6Point3fii(void* s, void* o, void* owner, Point3 p, float f, int a, int b) {
    calls += "ball(" + num(off(s)) + "," + num(off(o)) + "," + num(off(owner)) + "," + pt(p) + "," + num(f) + "," + num(a) + "," + num(b) + ");";
}
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3*, Point3*, float) {
    ++particles[d == ballSparkParticle ? 0 : d == trailParticleFire ? 1 : d == trailParticleSmoke ? 2 : 3];
}
void P_ExtendTrail__FiG6Point3(int t, Point3 p) { calls += "trail(" + num(t) + "," + pt(p) + ");"; }
void lightUpdatePosition__FP5Light(void* l) { calls += "lightpos(" + num(off(l)) + ");"; }
void LightEffect_ctor(void*, Point3, float, float, float, float, float, float, float, float, float, float, float, float, void*, int);
}
extern "C" void LightEffect_ctor(void* s, Point3 p, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, void* o, int n)
    __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
extern "C" void LightEffect_ctor(void* s, Point3 p, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, void* o, int n) {
    calls += "light(" + num(off(s)) + "," + pt(p) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + num(f) + num(g) + num(h) + "," + num(i) + "," + num(j) + "," + num(k) + "," + num(l) + "," + num(off(o)) + "," + num(n) + ");";
}
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static void putf(unsigned o, float v) { std::memcpy(obj + o, &v, 4); }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
static int bad = 0;
struct Cfg {
    int kind = 0; int state = 0; unsigned x = 0; int result = 0; unsigned frame = 5; unsigned end236 = 100; unsigned w364 = 0, w440 = 0, w76 = 0; int rank = 0; int flags = 0; unsigned w68 = 12345;
    int moveState = -1; int fail = 0; unsigned flagsT = 0x20; unsigned long long bits160 = 1ull << 41;
};
static void setup(const Cfg& c) {
    std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear(); allocCount = 0; std::memset(particles, 0, sizeof particles);
    put32(72, 0x4000); *reinterpret_cast<short*>(arena + 0x4000 + 8) = 16; put32(0, 0); std::memcpy(arena + 0x4000 + 12, "\x01\x01\x00\x00", 4);
    obj[204] = c.state; put32(208, c.kind); put32(292, c.x); put32(236, c.end236); put32(364, c.w364); put32(440, c.w440); put32(76, c.w76); put32(68, c.w68); put32(216, 33); obj[205] = 0xfe;
    float p[3] = {1, 2, 3}, v[3] = {4, 5, 6}; std::memcpy(obj + 16, p, 12); std::memcpy(obj + 428, v, 12);
    levelFlags = c.flags; frameNm = static_cast<int>(c.frame); activeResult = 9; moveResult = c.result; moveSetsState = c.moveState; featRank = c.rank; allocFail = c.fail;
    if (c.x) { put32(292, 0x2000); std::memcpy(arena + 0x2000 + 12, "\x00\x28\x00\x00", 4); std::memcpy(arena + 0x2800 + 12, &c.flagsT, 4); std::memcpy(arena + 0x2800 + 160, &c.bits160, 8); }
    if (c.w76) { put32(76, 0x3000); putA(0x3000 + 12, 0x3800); putA(0x3800 + 12, 0x40); putA(0x3800 + 960, 2); }
}
static std::string run(const Cfg& c) { setup(c); msg_run__13MissileWeapon(obj); return calls; }
static void expect(const char* name, const Cfg& c, const std::string& want) { const std::string got = run(c); if (got != want) { std::printf("fail: %s\n  want %s\n  got  %s\n", name, want.c_str(), got.c_str()); bad = 1; } }
static unsigned lcg(unsigned s) { return s * 0x19660du + 0x3c6ef35fu; }
// A reference sampler: draws (s >> 16) three at a time until the squared length is not above 1.
static void sampleRef(unsigned& s, Point3& p) {
    do { float c[3]; for (int i = 0; i < 3; ++i) { s = lcg(s); const float v = static_cast<float>(static_cast<int>(s >> 16)) * w11a::bits(0x37800000u); c[i] = v + v - 1.0f; } p = Point3{c[0], c[1], c[2]}; } while (1.0f < p.x * p.x + p.y * p.y + p.z * p.z);
}
int main() {
    const std::string del = "table(257,4112,1,3);";   // the table entry at 8 has the adjustment 16
    const std::string P = "1:2:3";
    const std::string seq = "sequence(28672,4096,1);";
    auto lightOrange = [](int block) { return "alloc188;light(" + num(0x6000 + 0x200 * block) + ",1:2:3,255,155,0,-32,0000,216,120,1,1,0,0);"; };
    auto lightBlue = [](int block) { return "alloc188;light(" + num(0x6000 + 0x200 * block) + ",1:2:3,0,155,255,-32,0000,216,120,1,1,0,0);"; };
    { Cfg c; c.x = 1; expect("attached, deletes", c, "active(4096,-2,480);queue(4096);"); }
    { Cfg c; c.x = 1; c.flagsT = 0; expect("attached, no flag", c, "active(4096,-2,480);"); }
    { Cfg c; c.x = 1; c.bits160 = 0; expect("attached, no bit 41", c, "active(4096,-2,480);"); }
    { Cfg c; c.state = 2; expect("state set deletes", c, del); }
    { Cfg c; c.kind = 0; expect("quiet", c, "move(4:5:6);"); }
    { Cfg c; c.kind = 0; c.end236 = 5; expect("end frame", c, "move(4:5:6);" + del); }
    { Cfg c; c.kind = 3; c.w364 = 7; expect("trail extend", c, "move(4:5:6);trail(7,1:2:3);"); }
    // Hits that name an impact sound (kinds 0 and 3).
    for (int variant = 0; variant < 3; ++variant) {
        Cfg c; c.kind = variant == 2 ? 0 : 3; c.result = 1; c.w440 = 0x500; c.w68 = 7; c.flags = variant == 0 ? 0x40000 : variant == 1 ? 0x100 : 0;
        const unsigned s = lcg(7); const std::string n = num(((s >> 16) & 3) + 1);
        std::string want = "move(4:5:6);";
        if (variant == 0) want += "effect(8," + P + ");lump(fx.lmp,impct_mtl_ice" + n + ".anm);";
        else want += std::string("effect(") + (variant == 1 ? "17" : "2") + "," + P + ");lump(fx.lmp,impct_mtl_stone" + n + ".anm);";
        want += seq + "sound(1280,1:2:3,1,1,0);" + del;
        expect("impact sound", c, want);
        if (get32(68) != s) { std::puts("fail: impact random state"); bad = 1; }
    }
    // Result 2 (a state set by move) for kinds 0 and 3.
    { Cfg c; c.kind = 0; c.result = 0; c.moveState = 2; c.flags = 0x100; expect("result 2, state 2, bit 0x100", c, "move(4:5:6);effect(17,1:2:3);" + del); }
    { Cfg c; c.kind = 3; c.result = 0; c.moveState = 2; c.flags = 0; expect("result 2, state 2", c, "move(4:5:6);effect(2,1:2:3);" + del); }
    { Cfg c; c.kind = 0; c.result = 0; c.moveState = 3; c.frame = 40; c.end236 = 100; run(c); if (get32(236) != 40) { std::puts("fail: state 3 end frame"); bad = 1; } }
    { Cfg c; c.kind = 0; c.result = 0; c.moveState = 1; c.frame = 40; c.end236 = 100; run(c); if (get32(236) != 41) { std::puts("fail: other state end frame"); bad = 1; } }
    { Cfg c; c.kind = 1; c.result = 0; c.moveState = 2; c.frame = 40; run(c); if (get32(236) != 0) { std::puts("fail: a hit clears +236"); bad = 1; } }
    // Kind 1 hit: sound, effect 5, vag sound, 3 + n props, the orange light, the 24-step trail and the saved position.
    for (int fail = 0; fail < 2; ++fail) {
        Cfg c; c.kind = 1; c.result = 1; c.w440 = 0x500; c.w68 = 99; c.fail = fail ? 2 : 0; c.end236 = 100;
        unsigned s = lcg(99); const int n = static_cast<int>((s >> 16) & 3) + 3;
        std::string want = "move(4:5:6);lump(fx.lmp,spel_hit_explarrow.anm);" + seq + "effect(5," + P + ");sound(1280,1:2:3,1,1,0);";
        int block = 0;
        for (int i = 0; i < n; ++i) { ++block; want += "alloc328;"; if (!(fail && block % 2 == 0)) want += "pprop(" + num(0x6000 + 0x200 * block) + "," + P + ",8);"; }
        ++block; if (fail && block % 2 == 0) want += "alloc188;"; else want += lightOrange(block);
        want += "lightpos(4464);" + del;
        expect("kind 1 hit", c, want);
        if (particles[1] != 24 || particles[2] != 24) { std::printf("fail: trail particle counts %d %d\n", particles[1], particles[2]); bad = 1; }
        // Reference random state after the 24 trail steps.
        for (int i = 0; i < 24; ++i) { Point3 a, b; sampleRef(s, a); s = lcg(lcg(lcg(s))); sampleRef(s, b); }
        float saved[3]; std::memcpy(saved, obj + 416, 12);
        if (get32(68) != s || saved[0] != 1 || saved[1] != 2 || saved[2] != 3) { std::puts("fail: kind 1 state or saved position"); bad = 1; }
    }
    // Kind 2 hit: explosion, orange light, radial damage; the position is saved.
    { Cfg c; c.kind = 2; c.result = 1; c.w440 = 0;
      expect("kind 2 hit", c, "move(4:5:6);lump(fx.lmp,spel_hit_explarrow.anm);" + seq + "explode(1:2:3,2);" + lightOrange(1) + "radial(4096,1:2:3,33,0,24,72,0,0);lightpos(4464);" + del); }
    // Kind 5 hit: cold sound, effect 10, blue light, then 32 snow particles.
    { Cfg c; c.kind = 5; c.result = 1;
      expect("kind 5 hit", c, "move(4:5:6);lump(fx.lmp,spel_cold_afteraffct.anm);" + seq + "effect(10," + P + ");" + lightBlue(1) + "lightpos(4464);" + del);
      if (particles[3] != 32) { std::puts("fail: snow count"); bad = 1; } }
    // Kind 6 hit: electric sound, 100 sparks, blue light; the fly step asks for the owner's feat rank.
    { Cfg c; c.kind = 6; c.result = 1; c.w76 = 1; c.rank = 3; c.w68 = 5;
      const float bx = 1 + 4 * w11a::bits(0x3da3d70au), by = 2 + 5 * w11a::bits(0x3da3d70au), bz = 3 + 6 * w11a::bits(0x3da3d70au);
      expect("kind 6 hit", c, "move(4:5:6);lump(fx.lmp,spel_elctric_afteraffct.anm);" + seq + lightBlue(1) + "feat(2,4096);ball(4180,4096,14336," + pt(Point3{bx, by, bz}) + ",1,0,3);lightpos(4464);" + del);
      if (particles[0] != 100) { std::puts("fail: spark count"); bad = 1; }
      unsigned s = 5; for (int i = 0; i < 100; ++i) { Point3 u; sampleRef(s, u); }
      if (get32(68) != s) { std::puts("fail: kind 6 random state"); bad = 1; } }
    { Cfg c; c.kind = 6; c.w76 = 1; c.rank = 1; expect("kind 6 low rank", c, "move(4:5:6);feat(2,4096);lightpos(4464);"); }
    { Cfg c; c.kind = 6; c.w76 = 0; expect("kind 6 no owner", c, "move(4:5:6);lightpos(4464);"); }
    // Kind 0 with bit 0x100: a draw mod 3 decides between the extra effect and the trail.
    for (unsigned seed = 1; seed < 40; ++seed) {
        Cfg c; c.kind = 0; c.flags = 0x100; c.w68 = seed; c.w364 = 9;
        const unsigned s1 = lcg(seed);
        std::string want = "move(4:5:6);";
        if ((s1 >> 16) % 3 == 0) {
            const unsigned s2 = lcg(s1);
            const float z = 3.0f + static_cast<float>(static_cast<int>(s2 >> 16)) * w11a::bits(0x37800000u) * w11a::bits(0x41900000u);
            want += "effect(17," + pt(Point3{1, 2, z}) + ");";
        } else want += "trail(9,1:2:3);";
        expect("kind 0 fly", c, want);
    }
    return bad;
}
