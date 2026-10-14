// Host contract for move__10ProjectileG6Point3; not an EE input. 32-bit words are offsets from a host base and
// the callees log. The authored function was also compared with the original in a private R5900 run (38
// scenarios: ground and no-ground, water or not, object lists with every skip rule and the equality edges; all 268
// original words reached).
#define W12_SPELL_C_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string calls;
unsigned w12_spell_c_host_call(unsigned function_word, void* adjusted_object, int argument_count, unsigned long a1, unsigned long) {
    calls += "slot(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted_object) - w12_spell_c_host_base) + "," +
             std::to_string(argument_count) + "," + std::to_string(a1) + ");";
    return 0;
}
extern "C" { void* world = reinterpret_cast<void*>(0x55); }
#include "move.cpp"

static std::string num(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
static unsigned char arena[0x4000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x100;
static unsigned char* const other = arena + 0x600;
static unsigned char* const table = arena + 0xa00;
static float groundHeight;
static int waterReply;
static int heightCalls;
static void* boxList[8];
static int boxCount;
extern "C" {
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, Point3 p, float r) {
    ++heightCalls;
    calls += "height(" + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + "," + num(r) + ");";
    return groundHeight;
}
int worldCheckForWater__FP12_worldHeaderG6Point3fPf(void*, Point3 p, float r, float*) { calls += "water(" + num(p.z) + "," + num(r) + ");"; return waterReply; }
void worldPerturbWater__FP12_worldHeaderG6Point3ff(void*, Point3 p, float a, float b) { calls += "perturb(" + num(p.x) + "," + num(a) + "," + num(b) + ");"; }
int objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 lo, Point3 hi, void** out, int cap) {
    calls += "box(" + num(lo.x) + ":" + num(lo.y) + ":" + num(lo.z) + "," + num(hi.x) + ":" + num(hi.y) + ":" + num(hi.z) + "," + std::to_string(cap) + ");";
    for (int i = 0; i < boxCount; ++i) out[i] = boxList[i];
    out[boxCount] = nullptr;
    return boxCount;
}
void* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("lump(") + a + "," + b + ");"; return reinterpret_cast<void*>(0x77); }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* s, void*, float v) { calls += "sound(" + std::to_string(reinterpret_cast<unsigned long>(s)) + "," + num(v) + ");"; }
int sprintf(char* buffer, const char* format, ...) {
    // only the one format used by the function: "%d" with the value 1
    const char* p = std::strstr(format, "%d");
    std::strncpy(buffer, format, static_cast<size_t>(p - format));
    std::strcpy(buffer + (p - format), "1");
    std::strcat(buffer, p + 2);
    return static_cast<int>(std::strlen(buffer));
}
}

static void put32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
static float getf(unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
static unsigned get32(unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static void reset() {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear(); heightCalls = 0; waterReply = 0; boxCount = 0;
    groundHeight = -1000.0f;
    putf(obj + 16, 10); putf(obj + 20, 20); putf(obj + 24, 30); putf(obj + 28, 2); putf(obj + 32, 6);
    put32(obj + 72, static_cast<unsigned>(table - arena));
    short adjust = -4; std::memcpy(table + 48, &adjust, 2); put32(table + 52, 0x1111);
    putf(other + 16, 11); putf(other + 20, 20); putf(other + 24, 30); putf(other + 28, 2); putf(other + 32, 6);
    put32(other + 72, static_cast<unsigned>(table - arena) + 0x40);
    std::memcpy(table + 0x40 + 48, &adjust, 2); put32(table + 0x40 + 52, 0x2222);
}

int main() {
    int bad = 0;
    const Point3 v{600.0f, 0.0f, 0.0f};   // 600 * (1/60) = 10 units along x
    const unsigned kk = 0x3c888888u;
    reset();
    int r = move__10ProjectileG6Point3(obj, v);
    float k; std::memcpy(&k, &kk, 4);
    const float nx = 10.0f + 600.0f * k;
    if (r != 0 || getf(obj + 16) != nx || calls.find("grid;height(") != 0 || calls.find("box(") == std::string::npos || calls.find("water") != std::string::npos) {
        std::puts(calls.c_str()); std::puts("fail: free flight"); bad = 1;
    }
    // ground hit: z 30 < 40; the search steps back until the height is not above the candidate (the ground is flat at 40, so
    // it never is: the 19th step is the last with t < 2.0 and the position ends one step-length back)
    reset(); groundHeight = 40.0f;
    r = move__10ProjectileG6Point3(obj, v);
    if ((r & 1) == 0 || heightCalls < 3) { std::puts(calls.c_str()); std::puts("fail: ground search"); bad = 1; }
    // ground hit with the candidate immediately above the ground: the first candidate is accepted (one extra height call)
    reset(); groundHeight = 29.0f;   // 30 < 29 is false -> no hit at all
    r = move__10ProjectileG6Point3(obj, v);
    if ((r & 1) != 0 || heightCalls != 1) { std::puts(calls.c_str()); std::puts("fail: above ground"); bad = 1; }
    // water: ground hit and the water test says yes; the byte at +80 gets set; the pool names the sound file
    reset(); groundHeight = 40.0f; waterReply = 1; put32(obj + 68, 1);
    r = move__10ProjectileG6Point3(obj, v);
    if (calls.find("water(") == std::string::npos || calls.find("lump(fx.lmp,foly_wtrsplash_sml1.anm);") == std::string::npos ||
        calls.find("perturb(") == std::string::npos || obj[80] != 1 || get32(obj + 68) != 1u * 0x19660du + 0x3c6ef35fu) {
        std::puts(calls.c_str()); std::puts("fail: water"); bad = 1;
    }
    reset(); groundHeight = 40.0f; waterReply = 1; obj[80] = 1;
    r = move__10ProjectileG6Point3(obj, v);
    if (calls.find("water(") != std::string::npos) { std::puts("fail: water repeated"); bad = 1; }
    // objects: the other one overlaps; the result gets bit 1 and both slots run (other first)
    reset(); boxList[0] = other; boxCount = 1;
    r = move__10ProjectileG6Point3(obj, Point3{0, 0, 0});
    {
        const std::string want = "slot(8738," + std::to_string(other - arena - 4) + ",1," + std::to_string(obj - arena) + ");slot(4369," +
                                 std::to_string(obj - arena - 4) + ",1," + std::to_string(other - arena) + ");";
        if (r != 2 || calls.find(want) == std::string::npos) { std::puts(calls.c_str()); std::puts("fail: object hit"); bad = 1; }
    }
    // skip rules: itself, the owner's skip object, the flag bits, no height overlap, too far
    reset(); boxList[0] = obj; boxCount = 1;
    if (move__10ProjectileG6Point3(obj, Point3{0, 0, 0}) != 0) { std::puts("fail: skips itself"); bad = 1; }
    reset(); boxList[0] = other; boxCount = 1; put32(obj + 76, 0x300); put32(arena + 0x300 + 12, static_cast<unsigned>(other - arena));
    if (move__10ProjectileG6Point3(obj, Point3{0, 0, 0}) != 0) { std::puts("fail: skip object"); bad = 1; }
    reset(); boxList[0] = other; boxCount = 1; put32(other + 12, 0x1);
    if (move__10ProjectileG6Point3(obj, Point3{0, 0, 0}) != 0) { std::puts("fail: flag 1"); bad = 1; }
    reset(); boxList[0] = other; boxCount = 1; put32(other + 12, 0x200);
    if (move__10ProjectileG6Point3(obj, Point3{0, 0, 0}) != 0) { std::puts("fail: flag 0x200"); bad = 1; }
    reset(); boxList[0] = other; boxCount = 1; put32(other + 12, 0x400);
    if (move__10ProjectileG6Point3(obj, Point3{0, 0, 0}) != 2) { std::puts("fail: other flag bits are ignored"); bad = 1; }
    reset(); boxList[0] = other; boxCount = 1; putf(other + 24, 100);
    if (move__10ProjectileG6Point3(obj, Point3{0, 0, 0}) != 0) { std::puts("fail: above in z"); bad = 1; }
    reset(); boxList[0] = other; boxCount = 1; putf(other + 24, -100);
    if (move__10ProjectileG6Point3(obj, Point3{0, 0, 0}) != 0) { std::puts("fail: below in z"); bad = 1; }
    reset(); boxList[0] = other; boxCount = 1; putf(other + 16, 14.5f);   // distance 4.5 > reach 4
    if (move__10ProjectileG6Point3(obj, Point3{0, 0, 0}) != 0) { std::puts("fail: too far"); bad = 1; }
    reset(); boxList[0] = other; boxCount = 1; putf(other + 16, 14.0f);   // distance 4 == reach 4: not below
    if (move__10ProjectileG6Point3(obj, Point3{0, 0, 0}) != 0) { std::puts("fail: exactly at reach"); bad = 1; }
    return bad;
}
