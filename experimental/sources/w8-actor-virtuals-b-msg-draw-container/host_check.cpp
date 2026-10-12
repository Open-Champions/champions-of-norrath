#include <cstdint>
#include <cstdio>
#include <cstring>

#include "msg_draw.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned pointer_word(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}

static int order[4];
static int steps;
static short seen_yaw;
static unsigned char yaw_point[12], draw_point[12];
static void* yaw_matrix;
static void* shadow_matrix;
static void* draw_matrix;
static unsigned shadow_model, draw_model, draw_texture;
static int shadow_mode, draw_mode, draw_last;
static void* shadow_animation = reinterpret_cast<void*>(1);
static void* draw_animation = reinterpret_cast<void*>(1);
static char shadow_flag;
static unsigned draw_flags;
static void* draw_record;
static char shadow_token;

extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    order[steps++] = 1;
    yaw_matrix = matrix;
    seen_yaw = yaw;
    std::memcpy(yaw_point, point, 12);
}
extern "C" void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
    void* model, int mode, void* matrix, void* animation, char flag) {
    order[steps++] = 2;
    shadow_model = pointer_word(model);
    shadow_mode = mode;
    shadow_matrix = matrix;
    shadow_animation = animation;
    shadow_flag = flag;
    return &shadow_token;
}
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* record, int last) {
    order[steps++] = 3;
    draw_model = pointer_word(model);
    draw_texture = pointer_word(texture);
    draw_mode = mode;
    std::memcpy(draw_point, point, 12);
    draw_matrix = matrix;
    draw_animation = animation;
    draw_flags = flags;
    draw_record = record;
    draw_last = last;
    return 0;
}

static void run(unsigned yaw_half, short expected_yaw) {
    unsigned char object[128];
    std::memset(object, 0, sizeof object);
    for (unsigned i = 0; i < 12u; ++i) object[16 + i] = static_cast<unsigned char>(0x50 + i);
    object[56] = static_cast<unsigned char>(yaw_half);
    object[57] = static_cast<unsigned char>(yaw_half >> 8);
    write_word(object + 96, 0x00112233u);
    write_word(object + 100, 0x00445566u);
    steps = 0;
    msg_draw__9Container(object);
    expect(steps == 3 && order[0] == 1 && order[1] == 2 && order[2] == 3, "matrix, shadow, then draw");
    expect(seen_yaw == expected_yaw, "yaw is the sign-extended halfword sum");
    expect(std::memcmp(yaw_point, object + 16, 12) == 0 && std::memcmp(draw_point, object + 16, 12) == 0,
           "point copies");
    expect(yaw_matrix == shadow_matrix && yaw_matrix == draw_matrix && yaw_matrix != nullptr, "one matrix");
    expect(shadow_model == 0x00112233u && draw_model == 0x00112233u && draw_texture == 0x00445566u, "model words");
    expect(shadow_mode == 1 && draw_mode == 1 && shadow_flag == 1 && draw_last == 0, "constants");
    expect(shadow_animation == nullptr && draw_animation == nullptr, "no animation");
    expect(draw_flags == 0xffffffffu, "flags are -1");
    expect(draw_record == &shadow_token, "shadow result feeds the draw record");
}

int main() {
    run(0, 16384);
    run(0x4000, static_cast<short>(0x8000));
    run(0xc000, 0);
    run(0x1234, 0x5234);
    return failures ? 1 : 0;
}
