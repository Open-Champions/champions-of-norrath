// Host contract for msg_collision__13MissileWeaponP10GameObject; not an EE input. 32-bit words are offsets from a
// host base; table calls (run-time type, owner and shadow-point entries) and the other callees are stubs that
// log. Each scenario lists the exact call log and final state the rule in msg_collision.cpp gives. The authored
// function was also compared with the original in a private R5900 run (700 scenarios). Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_a_host_base;
#include "msg_collision.cpp"

unsigned char skills[256];
unsigned syncRandState;
float worldDifficulty;
static unsigned char arena[0x8000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static unsigned char* const tgt = arena + 0x2000;
static std::string calls;
static unsigned typeResult, ownerResult, entry136Result, entry96Result, modelResult, allocResult, attackResult, rankResult;
static float deflection;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned off(const void* p) { return p ? static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena) : 0u; }
unsigned w11_actor_a_host_call(unsigned fn, void* adjusted, int argc, unsigned long a1, unsigned long a2) {
    switch (fn) {
    case 0x111: calls += "type;"; return typeResult;
    case 0x116: calls += "c16(" + num(off(adjusted)) + "," + num(argc) + ");"; return 0;
    case 0x180: calls += "c128(" + num(off(adjusted)) + ");"; return modelResult;
    case 0x1a0: calls += "c160(" + num(off(adjusted)) + ");"; return ownerResult;
    case 0x188: calls += "c136(" + num(off(adjusted)) + "," + num(a1 ? off(reinterpret_cast<void*>(a1)) : 0) + ");"; return entry136Result;
    case 0x160: calls += "c96(" + num(off(adjusted)) + "," + num(a1) + "," + num(static_cast<int>(a2)) + ");"; return entry96Result;
    }
    calls += "?;"; return 0;
}
extern "C" {
void* __tf13AutoContainer() { calls += "tfac;"; return reinterpret_cast<void*>(0xac); }
void* __tf8Skeleton() { calls += "tfsk;"; return reinterpret_cast<void*>(0x5c); }
void* __tf15PushPhysicsProp() { calls += "tfpp;"; return reinterpret_cast<void*>(0x99); }
int __eq__C9type_infoRC9type_info(const void* a, const void* b) { calls += "eq;"; return a == b; }
void __throw_bad_typeid() { calls += "badtypeid;"; }
void* listOnewayLink__FP8ListHeadT0Pv(void* a, void* b, void* v) { calls += "link(" + num(off(a)) + "," + num(off(b)) + "," + num(off(v)) + ");"; return nullptr; }
void listDestroy__FP8ListHead(void* h) { calls += "destroy(" + num(off(h)) + ");"; }
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(void* m, void* a, Point3 p, short h, int i, Point3* out, float* v, void* mat, float s) {
    calls += "shadow(" + num(reinterpret_cast<__UINTPTR_TYPE__>(m)) + "," + num(off(a)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(h) + "," + num(i) + "," + (v ? "val" : "null") + "," + num(s) + ");";
    float* r = static_cast<float*>(mat); for (int k = 0; k < 9; ++k) r[k] = static_cast<float>(k + 1);
}
void matTranspose__FP8Matrix33T0(void* a, void* b) { float* r = static_cast<float*>(a); calls += std::string("transpose(") + (a == b ? "same" : "diff") + "," + num(r[0]) + ");"; r[0] = 100; }
void matIdent__FP8Matrix33(void* m) { float* r = static_cast<float*>(m); for (int k = 0; k < 9; ++k) r[k] = (k % 4 == 0); calls += "ident;"; }
void matRot__FP8Matrix33is(void* m, int axis, short a) { calls += "rot(" + num(axis) + "," + num(a) + ");"; static_cast<float*>(m)[1] = 7; }
void matMul__FP8Matrix33N20(void* d, void* a, void* b) { calls += "mul(" + num(off(d)) + "," + num(static_cast<float*>(a)[1]) + "," + num(static_cast<float*>(b)[0]) + ");"; }
int playerGetSkillRank__Fii(int a, int b) { calls += "rank(" + num(a) + "," + num(b) + ");"; return static_cast<int>(rankResult); }
float getDeflectionChance__10SkillDodgei(void* d, int rank) { calls += "defl(" + num(off(d)) + "," + num(rank) + ");"; return deflection; }
void playerBlockSparks__6Playeri(void* p, int a) { calls += "sparks(" + num(off(p)) + "," + num(a) + ");"; }
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; return allocResult ? arena + allocResult : nullptr; }
void __6PhPropG6Point3P12_modelHeaderP8_textureliPQ24item11descriptionf(void* s, Point3 p, void* m, void* t, long n, int f, void* d, float sc) {
    calls += "prop(" + num(off(s)) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(off(m)) + "," + num(off(t)) + "," + num(n) + "," + num(f) + "," + (d ? "desc" : "null") + "," + num(sc) + ");";
}
void objectAddToDeleteList__FP10GameObject(void* o) { calls += "delete(" + num(off(o)) + ");"; }
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* d, bool f) {
    calls += "attack(" + num(off(a)) + "," + num(off(b)) + "," + num(x) + "," + num(y) + "," + num(p) + "," + num(q) + "," + num(r) + "," + num(s) + "," + num(off(d)) + "," + num(f) + ");";
    return static_cast<int>(attackResult);
}
}
static void put32(unsigned char* o, unsigned at, unsigned v) { std::memcpy(o + at, &v, 4); }
static void putA(unsigned a, unsigned v) { std::memcpy(arena + a, &v, 4); }
static unsigned rollOf(unsigned s) { return (((s * 0x19660du + 0x3c6ef35fu) >> 16) % 100u) + 1; }
static unsigned seedWithRoll(bool below, unsigned limit) {
    for (unsigned s = 1;; ++s) if ((rollOf(s) < limit) == below) return s;
}
struct Cfg {
    signed char state = 0; unsigned tflags = 0x20, wflags = 0x20, kind = 0, t216 = 0, type = 0, owner160 = 1, r136 = 0, model = 0, r96 = 0, rank = 0, alloc = 0, attack = 1;
    bool owner = true; float defl = 0; unsigned seed = 1; unsigned w444 = 0, w448 = 0; float difficulty = 0; bool target = true;
};
struct Result { std::string calls; signed char state; unsigned flags, w228, w232, w244, sync; };
static Result run(const Cfg& c) {
    std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear();
    put32(obj, 72, 0x4800); putA(0x4800 + 136 + 4, 0x188);
    obj[204] = static_cast<unsigned char>(c.state);
    if (c.owner) { put32(obj, 76, 0x1800); putA(0x1800 + 12, 0x3000); }
    put32(tgt, 72, 0x4000); putA(0x4000 + 4, 0x111); putA(0x4000 + 16 + 4, 0x116); *reinterpret_cast<short*>(arena + 0x4000 + 16) = 4;
    *reinterpret_cast<short*>(arena + 0x4000 + 128) = 8; putA(0x4000 + 128 + 4, 0x180);
    put32(tgt, 12, c.tflags); putA(0x3000 + 12, c.wflags); putA(0x3000 + 72, 0x4400); putA(0x4400 + 160 + 4, 0x1a0);
    put32(obj, 208, c.kind); put32(tgt, 216, c.t216); put32(tgt, 960, 7);
    put32(obj, 444, c.w444); put32(obj, 448, c.w448); put32(obj, 212, 11); put32(obj, 216, 12); put32(obj, 220, 13); put32(obj, 224, 14); put32(obj, 296, 0x6000);
    put32(skills, 8, 0x5000); putA(0x5000 + 96, 0x5400); putA(0x5400 + 96 + 4, 0x160);
    float pt[3] = {1, 2, 3}; std::memcpy(obj + 16, pt, 12);
    float tp[3] = {4, 5, 6}; std::memcpy(tgt + 16, tp, 12); *reinterpret_cast<short*>(tgt + 56) = 33; float sc = 0.5f; std::memcpy(tgt + 224, &sc, 4);
    *reinterpret_cast<short*>(obj + 56) = -9;
    typeResult = c.type; ownerResult = c.owner160; entry136Result = c.r136; modelResult = c.model; entry96Result = c.r96; rankResult = c.rank; deflection = c.defl;
    allocResult = c.alloc; attackResult = c.attack; syncRandState = c.seed; worldDifficulty = c.difficulty;
    msg_collision__13MissileWeaponP10GameObject(obj, c.target ? tgt : nullptr);
    Result r; r.calls = calls; r.state = static_cast<signed char>(obj[204]);
    std::memcpy(&r.flags, obj + 12, 4); std::memcpy(&r.w228, obj + 228, 4); std::memcpy(&r.w232, obj + 232, 4); std::memcpy(&r.w244, obj + 244, 4); r.sync = syncRandState;
    return r;
}
static int bad = 0;
static void expect(const char* name, const Cfg& c, const std::string& want, int state, unsigned flagBit = 0, unsigned w228 = 0, unsigned w232 = 0, unsigned w244 = 0, bool syncChanged = false) {
    const Result r = run(c);
    const bool syncOk = syncChanged ? r.sync == c.seed * 0x19660du + 0x3c6ef35fu : r.sync == c.seed;
    if (r.calls != want || r.state != state || (r.flags & 1u) != flagBit || r.w228 != w228 || r.w232 != w232 || r.w244 != w244 || !syncOk) {
        std::printf("fail: %s\n  want %s state %d flag %u 228=%u 232=%u 244=%u sync=%d\n  got  %s state %d flag %u 228=%u 232=%u 244=%u sync=%d\n", name, want.c_str(), state, flagBit, w228, w232, w244, syncChanged,
                    r.calls.c_str(), r.state, r.flags & 1u, r.w228, r.w232, r.w244, !syncOk);
        bad = 1;
    }
}
int main() {
    const std::string atk = "attack(8192,12288,11,12,0,13,14,0,24576,0);";
    // A nonzero state byte ends it at once.
    { Cfg c; c.state = 1; expect("state set", c, "", 1); }
    // Not a creature: the type decides whether the container entry runs; the state becomes 2 either way.
    { Cfg c; c.tflags = 0; c.type = 7; expect("not a creature", c, "type;tfac;eq;", 2); }
    { Cfg c; c.tflags = 0; c.type = 0xac; expect("auto container", c, "type;tfac;eq;c16(8196,2);", 2); }
    { Cfg c; c.owner160 = 0; expect("owner refuses", c, "c160(12288);", 0); }
    // The owner has 0x20 and 0x40; kind 0 links, asks the entry at 136 and releases the link on a zero answer.
    { Cfg c; c.wflags = 0x60; c.r136 = 0; expect("kind 0, no hit list", c, "c160(12288);link(4388,8192,8192);c136(4096,8192);destroy(4388);" + atk, 1, 1, 0, 0, 0, true); }
    { Cfg c; c.wflags = 0x60; c.r136 = 9; c.type = 0x5c; expect("skeleton", c, "c160(12288);link(4388,8192,8192);c136(4096,8192);type;tfsk;eq;destroy(4388);" + atk, 1, 1, 0, 0, 9, true); }
    { Cfg c; c.wflags = 0x60; c.r136 = 9; c.type = 0x7; expect("no model", c, "c160(12288);link(4388,8192,8192);c136(4096,8192);type;tfsk;eq;c128(8200);" + atk, 1, 1, 0, 0, 9, true); }
    { Cfg c; c.wflags = 0x60; c.r136 = 9; c.type = 0x7; c.model = 0x77;
      expect("model", c, "c160(12288);link(4388,8192,8192);c136(4096,8192);type;tfsk;eq;c128(8200);shadow(119,8480,4:5:6,33,9,null,0.5);transpose(same,1);ident;rot(2,-9);mul(4352,7,100);" + atk, 1, 1, 0, 0, 9, true); }
    // Kind 1, 5 and 6 store a damage word pair and skip the link.
    { Cfg c; c.wflags = 0x60; c.kind = 1; expect("kind 1", c, "c160(12288);attack(8192,12288,11,12,10000,13,14,32,24576,0);", 1, 0, 10000, 32, 0, true); }
    { Cfg c; c.wflags = 0x60; c.kind = 5; expect("kind 5", c, "c160(12288);attack(8192,12288,11,12,10000,13,14,1024,24576,0);", 1, 0, 10000, 1024, 0, true); }
    { Cfg c; c.wflags = 0x60; c.kind = 6; expect("kind 6", c, "c160(12288);attack(8192,12288,11,12,10000,13,14,128,24576,0);", 1, 0, 10000, 128, 0, true); }
    { Cfg c; c.wflags = 0x60; c.kind = 2; expect("kind 2 stores nothing", c, "c160(12288);" + atk, 1, 0, 0, 0, 0, true); }
    // A player target at 0x40 returns from the owner branch; here the owner lacks 0x40, so the block chance runs.
    { Cfg c; c.tflags = 0x60; c.wflags = 0x20; c.rank = 3; c.defl = 0.5f; c.seed = seedWithRoll(false, 50);
      expect("player, not blocked", c, "c160(12288);rank(7,2);defl(20480,3);" + atk, 3, 0, 0, 0, 0, true); }
    { Cfg c; c.tflags = 0x60; c.wflags = 0x20; c.rank = 3; c.defl = 0.5f; c.seed = seedWithRoll(true, 50); c.alloc = 0x7000; c.w444 = 0x111; c.w448 = 0x222;
      expect("player, blocked with a spark prop", c, "c160(12288);rank(7,2);defl(20480,3);sparks(8192,0);alloc584;prop(28672,1:2:3,273,546,1,42,null,1);delete(4096);", 0, 0, 0, 0, 0, true); }
    { Cfg c; c.tflags = 0x60; c.wflags = 0x20; c.rank = 3; c.defl = 0.5f; c.seed = seedWithRoll(true, 50);
      expect("player, blocked, no model words", c, "c160(12288);rank(7,2);defl(20480,3);sparks(8192,0);delete(4096);", 0, 0, 0, 0, 0, true); }
    { Cfg c; c.tflags = 0x60; c.wflags = 0x20; c.rank = 3; c.defl = 0.5f; c.seed = seedWithRoll(true, 50); c.w444 = 0x111; c.w448 = 0x222; c.alloc = 0;
      expect("player, blocked, allocation fails", c, "c160(12288);rank(7,2);defl(20480,3);sparks(8192,0);alloc584;delete(4096);", 0, 0, 0, 0, 0, true); }
    // A creature with the word at +216 uses the entry at 96 of the skill object with the difficulty.
    { Cfg c; c.wflags = 0x20; c.t216 = 5; c.r96 = 100; c.difficulty = 2.5f; c.seed = seedWithRoll(true, 100);
      expect("creature, entry 96 blocks", c, "c160(12288);c96(20480,0,2);delete(4096);", 0, 0, 0, 0, 0, true); }
    { Cfg c; c.wflags = 0x20; c.t216 = 0; c.r96 = 100; c.seed = 3; expect("creature without +216 never blocks", c, "c160(12288);" + atk, 1, 0, 0, 0, 0, true); }
    // The attack result 1 gives 1 for a creature and 3 for a player; any other result gives 2.
    { Cfg c; c.wflags = 0x20; c.attack = 4; expect("attack not 1", c, "c160(12288);" + atk, 2, 0, 0, 0, 0, true); }
    // A null owner: the attacker argument is the missile itself.
    { Cfg c; c.owner = false; expect("no owner", c, "attack(8192,4096,11,12,0,13,14,0,24576,0);", 1, 0, 0, 0, 0, true); }
    // A null target does nothing.
    { Cfg c; c.target = false; expect("null target", c, "", 0); }
    return bad;
}
