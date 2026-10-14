// Host contract for msg_run__11FireBomblet; not an EE input. 32-bit words are offsets from a host base (the
// object is the only block that uses them); every callee logs its arguments, eRand returns scripted 16-bit rolls
// and worldFindHeight a scripted height. Each scenario lists the exact call log and the final fields the rule in
// msg_run.cpp gives. The authored function was also compared with the original in a private R5900 run (800
// scenarios). Build with -ffp-contract=off. Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w11_actor_a_host_base;
#include "msg_run.cpp"

void* world;
int frameNm;
signed char cutMode;
unsigned syncRandState;
unsigned char sparkParticle[80];
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x1000;
static std::string calls;
static std::vector<unsigned> rolls;
static size_t nextRoll;
static float heightResult;
static std::vector<void*> foundList;
static int allocCount;
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%.9g", v); return b; }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
static unsigned off(const void* p) { return p ? static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena) : 0u; }
extern "C" {
unsigned eRand__10GameObject(void*) { return rolls[nextRoll++ % rolls.size()]; }
void gamePositionVisible__FP6Point3f(Point3* p, float r) { calls += "visible(" + pt(*p) + "," + num(r) + ");"; std::memcpy(obj + 148, "\x03\x00\x00\x00", 4); }
void soundChangeSequenceVolume__Fif(int id, float v) { calls += "volume(" + num(id) + "," + num(v) + ");"; }
void soundStopSound__Fi(int id) { calls += "stop(" + num(id) + ");"; }
void objectUpdateInGrid__FP10GameObject(void* o) { calls += "grid(" + num(off(o)) + ");"; }
void lightUpdatePosition__FP5Light(void* l) { calls += "lightpos(" + num(off(l)) + ");"; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void* w, Point3 p, float f) { calls += std::string("height(") + (w == world ? "w" : "?") + "," + pt(p) + "," + num(f) + ");"; return heightResult; }
void bounce__11FireBomblet(void* o) { calls += "bounce(" + num(off(o)) + ");"; }
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 lo, Point3 hi, void** found, int cap) {
    calls += "box(" + pt(lo) + "," + pt(hi) + "," + num(cap) + ");";
    size_t n = 0; for (void* p : foundList) found[n++] = p; found[n] = nullptr;
}
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* d, bool f) {
    const unsigned char* dd = static_cast<const unsigned char*>(d);
    short slot0[4]; std::memcpy(slot0, dd + 36, 8); short slot1; std::memcpy(&slot1, dd + 44, 2);
    calls += "attack(" + num(off(a)) + "," + num(off(b)) + "," + num(x) + "," + num(y) + "," + num(p) + "," + num(q) + "," + num(r) + "," + num(s) + ",desc[" + num(static_cast<signed char>(dd[32])) + "," +
             num(static_cast<signed char>(dd[33])) + "," + num(slot0[0]) + ":" + num(slot0[1]) + ":" + num(slot0[2]) + ":" + num(slot0[3]) + "," + num(slot1) + "]," + num(f) + ");";
    return 0;
}
void objectAddToDeleteList__FP10GameObject(void* o) { calls += "delete(" + num(off(o)) + ");"; }
void fxFlyingFire__FP6Point3T0ffb(Point3* p, Point3* v, float a, float b, bool f) { calls += "fire(" + pt(*p) + "," + pt(*v) + "," + num(a) + "," + num(b) + "," + num(f) + ");"; }
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* d, Point3* p, Point3* v, float age) { calls += std::string("spark(") + (d == sparkParticle ? "s" : "?") + "," + pt(*p) + "," + pt(*v) + "," + num(age) + ");"; }
void* blockAlloc__Fi(int n) { calls += "alloc" + num(n) + ";"; ++allocCount; return (allocCount % 2) ? arena + 0x2000 : nullptr; }
void FireBombletJr_ctor(void*, void*);
}
extern "C" void FireBombletJr_ctor(void* s, void* p) __asm__("__13FireBombletJrP11FireBomblet");
extern "C" void FireBombletJr_ctor(void* s, void* p) { calls += "child(" + num(off(s)) + "," + num(off(p)) + ");"; }
static void putf(unsigned o, float v) { std::memcpy(obj + o, &v, 4); }
static void put32(unsigned o, unsigned v) { std::memcpy(obj + o, &v, 4); }
static float getf(unsigned o) { float v; std::memcpy(&v, obj + o, 4); return v; }
static unsigned get32(unsigned o) { unsigned v; std::memcpy(&v, obj + o, 4); return v; }
struct Cfg {
    int D = 0, A = 0, B = 0, C = 0; int id = 5; float v[3] = {0, 0, 0}; float pos[3] = {1, 2, 3}; float r = 2; float f164 = -1000; float f116 = 0; float height = -100;
    int frame = 3; int cut = 0; unsigned sync = 0x1234; std::vector<void*> found; std::vector<unsigned> rolls = {40000, 30000, 33000};
    unsigned byte40 = 9; short h168 = 100, h170 = 40;
};
static int bad = 0;
static std::string vol(int id, float vx, float vy, float vz, float S) {
    const float speed = std::sqrt(vx * vx + vy * vy + vz * vz);
    return "volume(" + num(id) + "," + num(std::min(1.0f, speed * S)) + ");";
}
static std::string run(const Cfg& c) {
    std::memset(arena, 0, sizeof arena); w11_actor_a_host_base = arena; calls.clear(); allocCount = 0;
    obj[156] = c.D; obj[152] = c.A; obj[153] = c.B; obj[154] = c.C; put32(148, c.id);
    for (int i = 0; i < 3; ++i) { putf(76 + 4 * i, c.v[i]); putf(16 + 4 * i, c.pos[i]); }
    putf(28, c.r); putf(164, c.f164); putf(116, c.f116); obj[40] = c.byte40;
    *reinterpret_cast<short*>(obj + 168) = c.h168; *reinterpret_cast<short*>(obj + 170) = c.h170;
    world = arena + 0x3000; heightResult = c.height; frameNm = c.frame; cutMode = c.cut; syncRandState = c.sync; rolls = c.rolls; nextRoll = 0; foundList = c.found;
    msg_run__11FireBomblet(obj);
    return calls;
}
static void expect(const char* name, const std::string& got, const std::string& want) {
    if (got != want) { std::printf("fail: %s\n  want %s\n  got  %s\n", name, want.c_str(), got.c_str()); bad = 1; }
}
int main() {
    const float G = w11a::bits(0xbde0bb27u), V = w11a::bits(0xc0e1163bu), S = w11a::bits(0x3e119451u);
    // D set: only the trailing check, the smoke and nothing else.
    { Cfg c; c.D = 1; c.v[0] = 1; c.pos[0] = 1;
      expect("frozen", run(c), "fire(1:2:3,1:0:0,9,1,0);"); if (get32(148) != 5) { std::puts("fail: id changed"); bad = 1; } }
    // D set and A set: removed when below the floor value at +164, with the sound stopped for id >= 0.
    { Cfg c; c.D = 1; c.A = 1; c.f164 = 10; expect("remove below the floor", run(c), "delete(4096);stop(5);"); }
    { Cfg c; c.D = 1; c.A = 1; c.f164 = 10; c.id = -2; expect("remove, no sound", run(c), "delete(4096);"); }
    // Movement: id == -1 asks for visibility first; gravity is added; the volume is capped at 1.
    { Cfg c; c.id = -1; c.v[0] = 1; c.v[1] = 2; c.v[2] = 3; c.A = 0;
      const std::string got = run(c);
      const float vz = 3 + G;
      const float speed = std::sqrt(1.0f * 1.0f + 2.0f * 2.0f + vz * vz);
      const float volume = std::min(1.0f, speed * S);
      expect("movement", got, "visible(1:2:3,960);volume(3," + num(volume) + ");grid(4096);lightpos(4184);fire(" + pt(Point3{2, 4, 3 + 3}) + ",1:2:" + num(vz) + ",9,1,0);");
      if (getf(84) != vz || getf(16) != 2 || getf(20) != 4 || getf(24) != 6) { std::puts("fail: movement fields"); bad = 1; }
      float p[3]; std::memcpy(p, obj + 136, 12); if (p[0] != 2 || p[1] != 4 || p[2] != 6) { std::puts("fail: position copy"); bad = 1; } }
    // Terminal velocity: a very negative vz is clamped to V and the falling flag becomes 1; without bounce it stays quiet.
    { Cfg c; c.id = -2; c.v[2] = -10; c.A = 0; c.height = -1000;
      const std::string got = run(c);
      if (getf(84) != V || obj[152] != 1) { std::puts("fail: terminal velocity"); bad = 1; } (void)got; }
    // A stays zero while vz >= 0: the collision part does not run (no height query).
    { Cfg c; c.id = -2; c.v[2] = 1; expect("no fall yet", run(c), vol(-2, 0, 0, 1 + G, S) + "grid(4096);lightpos(4184);fire(" + pt(Point3{1, 2, 3 + 1}) + ",0:0:" + num(1 + G) + ",9,1,0);"); }
    // Above the ground: height queried, no bounce, no box when frame & 7 != 0.
    { Cfg c; c.id = -2; c.A = 1; c.v[2] = -2; c.height = -50; c.frame = 3;
      const float vz = -2 + G; const float z = 3 + -2.0f;
      expect("above the ground", run(c), vol(-2, 0, 0, vz, S) + "grid(4096);lightpos(4184);height(w," + pt(Point3{1, 2, z}) + ",4);fire(" + pt(Point3{1, 2, z}) + ",0:0:" + num(vz) + ",9,1,0);"); }
    // Slow and on the ground: removed (after the box search when the frame says so).
    { Cfg c; c.id = -2; c.A = 1; c.v[2] = -0.5f; c.height = 0; c.frame = 4; c.pos[2] = 2;
      const float vz = -0.5f + G;
      const std::string pos = pt(Point3{1, 2, 2 + -0.5f});
      expect("slow", run(c), vol(-2, 0, 0, vz, S) + "grid(4096);lightpos(4184);height(w," + pos + ",4);delete(4096);"); }
    // Fast and on the ground: bounce; z = height + r; the +160 word gets height.
    { Cfg c; c.id = -2; c.A = 1; c.v[0] = 3; c.v[1] = 4; c.v[2] = -5; c.height = 1; c.frame = 3; c.C = 2; c.cut = 0; c.byte40 = 200;
      const float vz = -5 + G;
      const std::string got = run(c);
      if (got.find("bounce(4096);") == std::string::npos || getf(24) != 3 || getf(160) != 1 || getf(76) != 3 || getf(80) != 4) { std::printf("fail: bounce %s z=%g w160=%g\n", got.c_str(), getf(24), getf(160)); bad = 1; }
      (void)vz;
      // 77 sparks for C >= 2, no extra bomblets, the halfword at +170 halved.
      size_t sparks = 0; for (size_t at = 0; (at = got.find("spark(", at)) != std::string::npos; ++at) ++sparks;
      if (sparks != 77 || got.find("alloc") != std::string::npos || *reinterpret_cast<short*>(obj + 170) != 20) { std::printf("fail: bounce effects (%zu sparks)\n", sparks); bad = 1; } }
    // A deep landing (penetration above 18) negates vx and vy.
    { Cfg c; c.id = -2; c.A = 1; c.v[0] = 3; c.v[1] = 4; c.v[2] = -5; c.height = 30; c.frame = 3; c.C = 2; c.pos[2] = 3;
      run(c); if (getf(76) != -3 || getf(80) != -4 || getf(24) != 32) { std::printf("fail: deep landing vx=%g vy=%g z=%g\n", getf(76), getf(80), getf(24)); bad = 1; } }
    // Box search every 8th frame: the missile itself is skipped; the description carries {21,1,1,0}.
    { Cfg c; c.id = -2; c.A = 1; c.v[2] = -2; c.height = -50; c.frame = 8; c.found = {arena + 0x2000, obj, arena + 0x2800};
      const float vz = -2 + G; const float z = 3 + -2.0f;
      const std::string got = run(c);
      const std::string desc = "desc[-1,-1,21:1:1:0,-1]";
      const std::string a1 = "attack(8192,4096,2,2,100000,0,0,16826432," + desc + ",0);", a2 = "attack(10240,4096,2,2,100000,0,0,16826432," + desc + ",0);";
      expect("box search", got, vol(-2, 0, 0, vz, S) + "grid(4096);lightpos(4184);height(w," + pt(Point3{1, 2, z}) + ",4);box(" + pt(Point3{1 - 2, 2 - 2, z - 2}) + "," + pt(Point3{1 + 2, 2 + 2, z + 2}) + ",64);" + a1 + a2 + "fire(" + pt(Point3{1, 2, z}) + ",0:0:" + num(vz) + ",9,1,0);"); }
    // Light fade after a landing and after the first flash.
    { Cfg c; c.D = 0; c.B = 1; c.f116 = 40; c.id = -2; c.A = 0; c.v[2] = 1;
      run(c); const float t = 40 - 12; if (getf(116) != t || getf(124) != t || getf(120) != 1.0f / (t * t) || getf(128) != 1.0f / (t * t)) { std::puts("fail: light fade"); bad = 1; } }
    { Cfg c; c.D = 0; c.B = 1; c.f116 = 20; c.id = -2; c.A = 0; c.v[2] = 1;
      run(c); if (getf(116) != 20 || getf(120) != 0) { std::puts("fail: light kept"); bad = 1; } }
    return bad;
}
