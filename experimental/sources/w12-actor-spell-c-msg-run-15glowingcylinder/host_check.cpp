// Host contract for msg_run__15GlowingCylinder; not an EE input. 32-bit words are offsets from a host base and
// the callees log. The authored function was also compared with the original in a private R5900 run (26
// scenarios over the countdown, frame and table adjustments; every one of its 115 words reached).
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
extern "C" int frameNm = 2;
#include "msg_run.cpp"

static std::string num(float v) { char b[32]; std::snprintf(b, sizeof b, "%g", v); return b; }
extern "C" {
float icos__Fi(int a) { calls += "cos(" + std::to_string(a) + ");"; return 0.5f; }
float isin__Fi(int a) { calls += "sin(" + std::to_string(a) + ");"; return -0.25f; }
void P_ExtendTrail__FiG6Point3(int t, Point3 p) { calls += "ext(" + std::to_string(t) + "," + num(p.x) + ":" + num(p.y) + ":" + num(p.z) + ");"; }
void P_SetTrailColor__Fiiiii(int t, int a, int b, int c, int d) {
    calls += "col(" + std::to_string(t) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d) + ");";
}
}

static unsigned char arena[0x1000] __attribute__((aligned(16)));
static unsigned char* const obj = arena + 0x100;
static void put32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void putf(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
static int get32(unsigned char* p) { int v; std::memcpy(&v, p, 4); return v; }
static void reset(int n) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    calls.clear();
    putf(obj + 16, 100); putf(obj + 20, 200); putf(obj + 24, 300);
    put32(obj + 72, 0x800); put32(obj + 76, static_cast<unsigned>(n));
    put32(obj + 88, 11); put32(obj + 92, 22);
    short adjust = -4;
    std::memcpy(arena + 0x800 + 8, &adjust, 2);
    put32(arena + 0x800 + 12, 0x7777);
}

int main() {
    int bad = 0;
    reset(50);
    msg_run__15GlowingCylinder(obj);
    // frameNm 2 -> angle 4096 for each trig call; z = 300 + (70 - 50) * 2 = 340
    if (calls != "cos(4096);sin(4096);ext(11,110:195:340);cos(4096);sin(4096);ext(22,90:205:340);" || get32(obj + 76) != 49) {
        std::puts(calls.c_str()); std::puts("fail: n = 50"); bad = 1;
    }
    reset(7);
    msg_run__15GlowingCylinder(obj);
    // f = 7 / 35 -> 0.2: colours (int)(0.2*86)=17, 25, 50 and (int)(0.2*200)=40
    const float f = 7.0f * 0.028571427f;
    const std::string c = "col(11," + std::to_string(static_cast<int>(f * 86.0f)) + "," + std::to_string(static_cast<int>(f * 126.0f)) + "," +
                          std::to_string(static_cast<int>(f * 250.0f)) + "," + std::to_string(static_cast<int>(f * 200.0f)) + ");";
    const std::string c2 = "col(22," + c.substr(7);
    if (calls.find(c) == std::string::npos || calls.find(c2) == std::string::npos || calls.find("slot") != std::string::npos || get32(obj + 76) != 6) {
        std::puts(calls.c_str()); std::puts("fail: n = 7"); bad = 1;
    }
    reset(35);
    msg_run__15GlowingCylinder(obj);
    if (calls.find("col(") != std::string::npos) { std::puts("fail: colour at 35"); bad = 1; }
    reset(1);
    msg_run__15GlowingCylinder(obj);
    const std::string end = "slot(30583," + std::to_string(obj - 4 - arena) + ",1,3);";
    if (calls.size() < end.size() || calls.compare(calls.size() - end.size(), end.size(), end) != 0 || get32(obj + 76) != 0) {
        std::puts(calls.c_str()); std::puts("fail: delete request"); bad = 1;
    }
    return bad;
}
