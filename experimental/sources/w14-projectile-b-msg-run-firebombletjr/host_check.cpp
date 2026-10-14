// Host contract for msg_run__13FireBombletJr; not an EE input. The callees log. The authored function was also compared with the
// original in a private R5900 run (2,880 scenarios over vertical speed, the byte at +88, ground height, the bounds float, frame
// number and found-object lists; every original word covered except a padding word and the second pass of the description slot
// search, which cannot run on a freshly initialised description).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cmath>
#include <cstring>
#include <string>
#include <vector>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls;
static float height;
static std::vector<void*> found_list;
struct Attack { void* target; float a, b; int p, q, r, s; short slot[4]; };
static std::vector<Attack> attacks;
static float fx_a, fx_b;
static bool fx_flag;
static float box_low_z, box_high_z, ground_query_z, ground_query_f;
extern "C" {
void* world;
int frameNm;
void objectUpdateInGrid__FP10GameObject(void*) { calls += "grid;"; }
float worldFindHeight__FP12_worldHeaderG6Point3f(void*, Point3 p, float f) { calls += "height;"; ground_query_z = p.z; ground_query_f = f; return height; }
void bounce__13FireBombletJr(void*) { calls += "bounce;"; }
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** out, int capacity) {
    calls += "box" + std::to_string(capacity) + ";";
    box_low_z = low.z; box_high_z = high.z;
    for (size_t i = 0; i < found_list.size(); ++i) out[i] = found_list[i];
    out[found_list.size()] = nullptr;
}
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void*, float x, float y, int p, int q, int r, int s, void* d, bool) {
    calls += "attack;";
    Attack at{a, x, y, p, q, r, s, {}};
    std::memcpy(at.slot, static_cast<unsigned char*>(d) + 36, 8);
    attacks.push_back(at);
    return 0;
}
void objectAddToDeleteList__FP10GameObject(void*) { calls += "del;"; }
void fxFlyingFire__FP6Point3T0ffb(Point3*, Point3*, float a, float b, bool flag) { calls += "fx;"; fx_a = a; fx_b = b; fx_flag = flag; }
}
#include "msg_run.cpp"

static unsigned char object[256] __attribute__((aligned(16)));
static void setf(int off, float v) { std::memcpy(object + off, &v, 4); }
static float getf(int off) { float v; std::memcpy(&v, object + off, 4); return v; }
static short geth(int off) { short v; std::memcpy(&v, object + off, 2); return v; }
static void reset(float z, float vz, float vx, float vy) {
    std::memset(object, 0, sizeof object);
    calls.clear(); attacks.clear(); found_list.clear(); height = -1000.0f; frameNm = 3;
    setf(16, 1); setf(20, 2); setf(24, z); setf(28, 2); setf(76, vx); setf(80, vy); setf(84, vz); setf(96, -500.0f);
    object[40] = 200;
    const short a = 100, b = 50;
    std::memcpy(object + 90, &a, 2); std::memcpy(object + 92, &b, 2);
}
int main() {
    // free flight
    reset(100, 1, 0.5f, 0.25f);
    msg_run__13FireBombletJr(object);
    if (calls != "grid;height;fx;" || getf(16) != 1.5f || getf(20) != 2.25f || getf(24) != 101.0f) return 1;
    if (std::fabs(getf(84) - (1.0f - 0.10973196f)) > 1e-6 || geth(90) != 150 || geth(92) != 50) return 2;
    if (fx_a != 200.0f || fx_b != 0.25f || fx_flag || ground_query_f != 4.0f) return 3;
    // a bounce with a drop above 18 reverses the horizontal velocity; the bounds are checked afterwards
    reset(10, -3, 2, 4);
    height = 30.0f;
    msg_run__13FireBombletJr(object);
    if (calls != "grid;height;bounce;fx;" || getf(24) != 32.0f || getf(76) != -2.0f || getf(80) != -4.0f || geth(92) != 25) return 4;
    // a bounce with a small drop keeps the direction
    reset(10, -3, 2, 4);
    height = 10.0f;
    msg_run__13FireBombletJr(object);
    if (calls != "grid;height;bounce;fx;" || getf(76) != 2.0f || getf(80) != 4.0f || getf(24) != 12.0f || geth(92) != 25) return 5;
    // a slow ball at the ground is removed without a bounce or an effect
    reset(10, 0.2f, 0.1f, 0.1f);
    height = 10.0f;
    msg_run__13FireBombletJr(object);
    if (calls != "grid;height;del;") return 6;
    // damage scan on frames that are multiples of 8, skipping the object itself
    reset(100, 1, 0.5f, 0.25f);
    frameNm = 16;
    unsigned char other[16];
    found_list = {other, object, other + 1};
    msg_run__13FireBombletJr(object);
    if (calls != "grid;height;box64;attack;attack;fx;" || attacks.size() != 2 || attacks[0].target != other || attacks[1].target != other + 1) return 7;
    if (attacks[0].a != 1.0f || attacks[0].b != 1.0f || attacks[0].p != 100000 || attacks[0].s != 0x100c040 || attacks[0].slot[0] != 21 ||
        attacks[0].slot[1] != 1 || attacks[0].slot[2] != 1 || attacks[0].slot[3] != 0)
        return 8;
    if (box_low_z != 99.0f || box_high_z != 103.0f) return 9;
    // below the bounds float: removed
    reset(-600, 1, 0.5f, 0.25f);
    msg_run__13FireBombletJr(object);
    if (calls != "grid;height;del;") return 10;
    // vertical speed under the floor value: pinned to it; a non-zero byte at +88 stops the ground test
    reset(100, -7.0f, 0, 0);
    object[88] = 1;
    msg_run__13FireBombletJr(object);
    if (calls != "grid;fx;" || getf(84) >= -7.03f || getf(84) <= -7.04f) return 11;
    reset(100, -7.0f, 0, 0);
    msg_run__13FireBombletJr(object);
    if (calls != "grid;height;fx;" || getf(84) >= -7.03f || getf(84) <= -7.04f) return 12;
    // the halfword at +92 is halved toward zero when negative
    reset(10, -3, 2, 4);
    height = 10.0f;
    const short neg = -7;
    std::memcpy(object + 92, &neg, 2);
    msg_run__13FireBombletJr(object);
    if (geth(92) != -3) return 13;
    return 0;
}
