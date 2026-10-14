// Host contract for msg_run__19FireArrowProjectile; not an EE input. The callees log. The authored function was
// also compared with the original in a private R5900 run (60 scenarios: skip word, age limits, hit bits; all
// reachable original words covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls;
static int move_result;
static Point3 move_arg, fx_v;
static float fx_a, fx_b;
extern "C" {
int move__10ProjectileG6Point3(void*, Point3 velocity) { calls += "move;"; move_arg = velocity; return move_result; }
void terminate__19FireArrowProjectilei(void*, int hit) { calls += "term" + std::to_string(hit) + ";"; }
void fxFlyingFire__FP6Point3T0ffb(Point3*, Point3* velocity, float a, float b, bool lead) { calls += lead ? "fx;" : "fx0;"; fx_v = *velocity; fx_a = a; fx_b = b; }
void lightUpdatePosition__FP5Light(void*) { calls += "light;"; }
}
#include "msg_run.cpp"

static void setf(unsigned char* o, int off, float v) { std::memcpy(o + off, &v, 4); }
static unsigned char object[256] __attribute__((aligned(16)));
static void reset() {
    std::memset(object, 0, sizeof object);
    calls.clear(); move_result = 0;
    setf(object, 16, 1); setf(object, 20, 2); setf(object, 24, 3); setf(object, 28, 2);
    setf(object, 84, 60); setf(object, 88, 120); setf(object, 92, 180);
}
int main() {
    reset(); object[96] = 1;
    msg_run__19FireArrowProjectile(object);
    if (!calls.empty() || object[104] != 0) return 1;
    reset(); move_result = 2;  // bit 0 clear: no hit
    msg_run__19FireArrowProjectile(object);
    if (calls != "move;fx;light;" || object[104] != 1 || move_arg.y != 120.0f) return 2;
    auto near = [](float a, float b) { return a - b < 1e-4f && b - a < 1e-4f; };
    if (!near(fx_v.x, 1.0f) || !near(fx_v.y, 2.0f) || !near(fx_v.z, 3.0f) || fx_a != 2.0f) return 3;
    float b = fx_b; float s = 2.0f * 2.0f * 0.021989f + 0.267f;
    if (b < s - 1e-5f || b > s + 1e-5f) return 4;
    float copied[3]; std::memcpy(copied, object + 160, 12);
    if (copied[0] != 1.0f || copied[1] != 2.0f || copied[2] != 3.0f) return 5;
    reset(); move_result = 7;
    msg_run__19FireArrowProjectile(object);
    if (calls != "move;term1;") return 6;
    reset(); object[104] = 0x58; object[105] = 0x02;  // 600: one more tick is past the limit
    msg_run__19FireArrowProjectile(object);
    if (calls != "move;term0;") return 7;
    reset(); object[104] = 0x57; object[105] = 0x02;
    msg_run__19FireArrowProjectile(object);
    if (calls != "move;fx;light;") return 8;
    return 0;
}
