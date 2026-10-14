// Host contract for msg_run__8HateSoul; not an EE input. The callees log. The authored function was also compared with the
// original in a private R5900 run (153 scenarios over frameNm, the start word, the two switch words, the heading and roll
// values; every original word covered but two delay-slot nops), including the exact rsqrt.s result.
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_spell_msg_b_host_base;
static std::string calls;
unsigned w13_spell_msg_b_host_call(unsigned fn, void*, int, unsigned long a1, unsigned long) {
    calls += "entry" + std::to_string(fn) + "(" + std::to_string(a1) + ");";
    return 0;
}
static unsigned roll;
static int width_arg, trail_arg, cos_arg, sin_arg;
static Point3 move_arg, extend_arg;
extern "C" {
int frameNm;
unsigned eRand__10GameObject(void*) { calls += "rand;"; return roll; }
float icos__Fi(int a) { calls += "cos;"; cos_arg = a; return 0.5f; }
float isin__Fi(int a) { calls += "sin;"; sin_arg = a; return 0.25f; }
void P_SetTrailWidth__Fii(int trail, int width) { calls += "width;"; trail_arg = trail; width_arg = width; }
void P_ExtendTrail__FiG6Point3(int, Point3 p) { calls += "extend;"; extend_arg = p; }
int move__10ProjectileG6Point3(void*, Point3 v) { calls += "move;"; move_arg = v; return 0; }
void lightUpdatePosition__FP5Light(void*) { calls += "light;"; }
}
#include "msg_run.cpp"

static void setf(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static unsigned char* const object = arena + 0x100;
static short half_at(int off) { short v; std::memcpy(&v, object + off, 2); return v; }
static void reset(int frame, int start, int w160, int w164, short heading) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena);
    const unsigned table = 0x800, function = 0x900;
    std::memcpy(object + 72, &table, 4); std::memcpy(arena + 0x800 + 8 + 4, &function, 4);
    calls.clear(); frameNm = frame; roll = 0;
    std::memcpy(object + 84, &start, 4); std::memcpy(object + 160, &w160, 4); std::memcpy(object + 164, &w164, 4);
    std::memcpy(object + 168, "\x05\0\0\0", 4); std::memcpy(object + 56, &heading, 2);
    setf(object, 16, 1); setf(object, 20, 2); setf(object, 24, 3);
}
int main() {
    reset(1010, 1000, 0, 0, 100);  // age 10: the trail width and the fixed heading
    msg_run__8HateSoul(object);
    if (calls != "width;cos;sin;move;cos;sin;extend;") { std::puts(calls.c_str()); return 1; }
    if (trail_arg != 5 || width_arg != static_cast<int>(10.0f * w13smb::bits(0x3d888888u) * 700.0f + 35.0f)) return 2;
    if (move_arg.x != 270.0f || move_arg.y != 135.0f || move_arg.z != 50.0f || cos_arg != 1010 * 6880 || sin_arg != 1010 * 6880) return 3;
    float vector[3]; std::memcpy(vector, object + 88, 12);
    if (vector[0] != 270.0f || vector[1] != 135.0f || vector[2] != 50.0f || half_at(56) != 100) return 4;
    // the extended point: A = (135, -270, 0) made unit length and scaled by 0.5 * 0.754, B = (0, 0, 1) scaled by 0.25 * 0.754
    {
        const float inverse = 1.0f / __builtin_sqrtf(135.0f * 135.0f + 270.0f * 270.0f), k = w13smb::bits(0x3f410624u);
        auto near = [](float x, float y) { return x - y < 1e-3f && y - x < 1e-3f; };
        if (!near(extend_arg.x, 1.0f + 135.0f * inverse * 0.5f * k) || !near(extend_arg.y, 2.0f - 270.0f * inverse * 0.5f * k) || !near(extend_arg.z, 3.0f + 0.25f * k)) return 5;
    }
    // an older object with the switch word clear: the heading grows by r % 500 + 500
    reset(1020, 1000, 0, 0, 100); roll = 7;
    msg_run__8HateSoul(object);
    if (calls != "rand;cos;sin;move;cos;sin;extend;" || half_at(56) != 100 + 7 + 500) return 6;
    // with the switch word set it shrinks
    reset(1020, 1000, 1, 0, 100); roll = 7;
    msg_run__8HateSoul(object);
    if (half_at(56) != 100 - 7 - 500) return 7;
    reset(1020, 1000, 0, 0, 0x7fff); roll = 0xffffffffu;  // 4294967295 % 500 = 295; the sum wraps to a negative halfword
    msg_run__8HateSoul(object);
    if (half_at(56) != static_cast<short>(0x7fff + 295 + 500)) return 8;
    // the light is moved only with the +164 word
    reset(1020, 1000, 0, 1, 0);
    msg_run__8HateSoul(object);
    float copied[3]; std::memcpy(copied, object + 148, 12);
    if (calls.find("light;") == std::string::npos || copied[0] != 1.0f || copied[1] != 2.0f || copied[2] != 3.0f) return 9;
    // the end of life: age 601 calls entry 8 with 3, age 600 does not
    reset(1600, 1000, 0, 0, 0);
    msg_run__8HateSoul(object);
    if (calls.find("entry") != std::string::npos) return 10;
    reset(1601, 1000, 0, 0, 0);
    msg_run__8HateSoul(object);
    if (calls.find("entry2304(3);") == std::string::npos) return 11;
    return 0;
}
