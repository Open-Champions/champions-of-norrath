// Host contract for drawArrow__14WoodElfSoldier; not an EE input. Pointer words are offsets from a host base
// and the callees record their arguments.
#define W11_ACTOR_C_HOST 1
#include "draw_arrow.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
W11ActorCHostSlot w11_actor_c_host_slots[1];
unsigned char globalItem[64];
static std::string calls;
static short last_trans_angle;
static Point3 last_trans_pos;
static Matrix34* trans_matrix;
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* m, Point3 p, short a) {
    trans_matrix = m; last_trans_pos = p; last_trans_angle = a; calls += "trans;";
    for (int i = 0; i < 16; ++i) m->m[i] = 100.0f + i;
}
extern "C" void matRot__FP8Matrix34is(Matrix34* m, int axis, short angle) {
    calls += "rot(" + std::to_string(axis) + "," + std::to_string(angle) + "," + (m == trans_matrix ? "same" : "other") + ");";
}
static Point3 draw_pos; static int draw_mode, draw_extra; static unsigned long long draw_flags; static void *draw_model, *draw_texture, *draw_anim, *draw_record;
static float draw_matrix0;
extern "C" void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 p, Matrix34* m, void* anim, unsigned long long flags, void* record, int extra) {
    calls += "draw;"; draw_model = model; draw_texture = texture; draw_mode = mode; draw_pos = p; draw_flags = flags; draw_anim = anim;
    draw_record = record; draw_extra = extra; draw_matrix0 = m->m[0]; if (m != trans_matrix) calls += "badmatrix;";
}
int main() {
    static unsigned char base[8192];
    w11_actor_c_host_base = base;
    unsigned char* obj = base + 128;
    int failures = 0;
    for (unsigned flag : {0u, 1u, 0x80000000u})
        for (unsigned short yaw : {0, 16384, 5, 65535, 40000}) {
            std::memset(base, 0, sizeof base);
            std::memcpy(obj + 988, &flag, 4);
            std::memcpy(obj + 984, &yaw, 2);
            const float pos[3] = {1.5f, -2.0f, 3.25f};
            std::memcpy(obj + 972, pos, 12);
            *reinterpret_cast<unsigned*>(globalItem + 8) = 1000;
            *reinterpret_cast<unsigned*>(globalItem + 12) = 2000;
            calls.clear();
            drawArrow__14WoodElfSoldier(obj);
            if (flag == 0) { if (!calls.empty()) { std::fprintf(stderr, "drew with zero word\n"); ++failures; } continue; }
            const short want_yaw = static_cast<short>(yaw - 16384);
            if (calls != "trans;rot(0,-7192,same);draw;" || last_trans_angle != want_yaw || std::memcmp(&last_trans_pos, pos, 12) != 0 ||
                std::memcmp(&draw_pos, pos, 12) != 0 || draw_model != base + 1000 || draw_texture != base + 2000 || draw_mode != 3 ||
                draw_flags != ~0ull || draw_anim || draw_record || draw_extra != 0) {
                std::fprintf(stderr, "yaw %u: %s\n", yaw, calls.c_str()); ++failures;
            }
        }
    if (!failures) std::puts("drawArrow ok");
    return failures != 0;
}
