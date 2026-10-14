// Host contract for msg_draw__5Wheel; not an EE input. The callees log. 32-bit words are offsets from a host base. The
// authored function was also compared with the original in a private R5900 run (84 scenarios over the block index, the
// selector float and the angle, including saturating angles; all but one delay-slot nop covered).
#define W13_SPELL_MSG_B_HOST 1
#include "../w13-spell-msg-b-common/access.hpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_spell_msg_b_host_base;
unsigned w13_spell_msg_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls;
static float rot_matrix[12];
static short rot_angle;
static int rot_axis, draw_mode, draw_extra;
static float draw_matrix[12];
static Point3 draw_pos;
static void *draw_model, *draw_texture, *draw_anim, *draw_record;
static unsigned long long draw_flags;
#include "msg_draw.cpp"
extern "C" {
void matRot__FP8Matrix34is(Matrix34* m, int axis, short angle) { calls += "rot;"; std::memcpy(rot_matrix, m->m, 48); rot_axis = axis; rot_angle = angle; }
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, Matrix34* m, void* anim, unsigned long long flags, void* record, int extra) {
    calls += "draw;"; draw_model = model; draw_texture = texture; draw_mode = mode; draw_pos = p; std::memcpy(draw_matrix, m->m, 48);
    draw_anim = anim; draw_flags = flags; draw_record = record; draw_extra = extra;
}
void modelResetOptionalParams__Fv() { calls += "reset;"; }
}

static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
enum { O = 0x100, B = 0x800 };
static void reset(float selector, float angle) {
    w13_spell_msg_b_host_base = arena;
    std::memset(arena, 0, sizeof arena); calls.clear();
    put32(O + 396, B); put32(O + 384, 1); putf(B + 12 + 1236, selector);
    put32(O + 76, 0x400); put32(O + 80, 0x500);
    for (int i = 0; i < 3; ++i) putf(O + 16 + 4 * i, 1.0f + i);
    for (int i = 0; i < 9; ++i) putf(O + 400 + 4 * i, 10.0f + i);
    putf(O + 368, angle);
    const unsigned long long flags = 0x0123456789abcdefull;
    std::memcpy(arena + O + 352, &flags, 8);
}
int main() {
    unsigned char* const o = arena + O;
    reset(1.0f, 90.0f);
    msg_draw__5Wheel(o);
    if (calls != "rot;draw;reset;") return 1;
    for (int i = 0; i < 12; ++i) if (rot_matrix[i] != draw_matrix[i]) return 2;
    const float expect[12] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 1, 2, 3};
    for (int i = 0; i < 12; ++i) if (draw_matrix[i] != expect[i]) return 3;
    if (rot_axis != 0 || rot_angle != static_cast<short>(static_cast<int>(90.0f * w13smb::bits(0x3bb60b60u) * w13smb::bits(0x46fffe00u)))) return 4;
    if (rot_angle < 16380 || rot_angle > 16384) return 5;  // 90 / 180 of 32767
    if (draw_model != arena + 0x400 || draw_texture != arena + 0x500 || draw_mode != 1 || draw_anim || draw_record || draw_extra) return 6;
    if (draw_flags != 0x0123456789abcdefull || draw_pos.x != 1.0f || draw_pos.z != 3.0f) return 7;
    reset(-2.0f, 0.0f);
    msg_draw__5Wheel(o);
    if (draw_matrix[0] != -10.0f || draw_matrix[1] != -11.0f || draw_matrix[2] != -12.0f || draw_matrix[3] != 13.0f || rot_angle != 0) return 8;
    reset(0.0f, 0.0f);
    msg_draw__5Wheel(o);
    if (draw_matrix[0] != -10.0f) return 9;
    return 0;
}
