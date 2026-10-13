// Host contract for msg_draw__19MultiFireProjectile; not an EE input. The callees log; memset is renamed so the host
// library one is untouched. The authored function was also compared with the original in a private R5900 run
// (8 scenarios, every original word covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include "msg_draw.cpp"
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls;
static short seen_angle;
static Point3 seen_pos, draw_pos;
static int draw_mode, draw_extra;
static void *draw_model, *draw_texture, *draw_anim, *draw_record;
static unsigned long long draw_flags;
extern "C" {
unsigned char globalItem[64];
void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* m, Point3 p, short a) {
    calls += "trans;"; (void)m; seen_pos = p; seen_angle = a;
}
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, Matrix34*, void* anim, unsigned long long flags, void* record, int extra) {
    calls += "draw;"; draw_model = model; draw_texture = texture; draw_mode = mode; draw_pos = p; draw_flags = flags; draw_anim = anim; draw_record = record; draw_extra = extra;
}
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
    msg_draw__19MultiFireProjectile(o);
    if (calls != "trans;draw;") { std::puts(calls.c_str()); return 1; }
    if (seen_angle != static_cast<short>(5 - 16384) || std::memcmp(&seen_pos, pos, 12) != 0 || std::memcmp(&draw_pos, pos, 12) != 0) return 3;
    if (draw_model != arena + 0x400 || draw_texture != arena + 0x500 || draw_mode != 1 || draw_flags != ~0ull || draw_anim || draw_record || draw_extra) return 5;
    return 0;
}
