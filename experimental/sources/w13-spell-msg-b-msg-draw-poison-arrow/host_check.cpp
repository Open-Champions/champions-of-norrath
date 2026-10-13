// Host contract for msg_draw__21PoisonArrowProjectile; not an EE input. The callees log; memset is renamed so the host
// library one is untouched. The authored function was also compared with the original in a private R5900 run
// (8 scenarios, every original word covered), including the matrix elements the callee sees at the call.
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#define memset w13_host_memset
#include "msg_draw.cpp"
#undef memset
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls;
static float seen_m[3];
static short seen_angle;
static Point3 seen_pos, draw_pos;
static short ambient_seen[4];
static int draw_mode, draw_extra;
static void *draw_model, *draw_texture, *draw_anim, *draw_record;
static unsigned long long draw_flags;
extern "C" {
unsigned char globalItem[64];
float icos__Fi(int a) { calls += "cos" + std::to_string(a) + ";"; return 0.5f; }
float isin__Fi(int a) { calls += "sin" + std::to_string(a) + ";"; return 0.25f; }
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* m, Point3 p, short a) {
    calls += "trans;"; std::memcpy(seen_m, &m->m[12], 12); seen_pos = p; seen_angle = a;
}
void* w13_host_memset(void* d, int v, unsigned long n) { calls += "memset;"; return std::memset(d, v, n); }
void lightSetAmbientOffset__FPs(short* a) { calls += "ambient;"; std::memcpy(ambient_seen, a, 8); }
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, Matrix34*, void* anim, unsigned long long flags, void* record, int extra) {
    calls += "draw;"; draw_model = model; draw_texture = texture; draw_mode = mode; draw_pos = p; draw_flags = flags; draw_anim = anim; draw_record = record; draw_extra = extra;
}
void lightResetOptionalParams__Fv() { calls += "reset;"; }
}

static unsigned char arena[0x2000] __attribute__((aligned(16)));
int main() {
    w13_spell_msg_b_host_base = arena;
    unsigned char* const o = arena + 0x100;
    const float pos[3] = {1.5f, -2.0f, 3.25f};
    std::memcpy(o + 16, pos, 12);
    const short heading = 5;
    std::memcpy(o + 100, &heading, 2);
    unsigned model = 0x400, texture = 0x500;
    std::memcpy(globalItem + 8, &model, 4);
    std::memcpy(globalItem + 12, &texture, 4);
    msg_draw__21PoisonArrowProjectile(o);
    if (calls != "cos5;sin5;trans;memset;ambient;draw;reset;") { std::puts(calls.c_str()); return 1; }
    if (seen_m[0] != 0.5f * 8.0f || seen_m[1] != 0.25f * 8.0f || seen_m[2] != 0.0f) return 2;
    if (seen_angle != static_cast<short>(5 - 16384) || seen_pos.x != 1.5f + 4.0f || seen_pos.y != -2.0f + 2.0f || seen_pos.z != 3.25f) return 3;
    if (std::memcmp(&draw_pos, pos, 12) != 0) return 6;
    if (ambient_seen[0] != 0 || ambient_seen[1] != 0 || ambient_seen[2] != 128 || ambient_seen[3] != 0) return 4;
    if (draw_model != arena + 0x400 || draw_texture != arena + 0x500 || draw_mode != 1 || draw_flags != ~0ull || draw_anim || draw_record || draw_extra) return 5;
    return 0;
}
