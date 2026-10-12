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
static unsigned pointer_word(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}

static int order[4];
static int steps;
static float shadow_matrix_copy[12], draw_matrix_copy[12];
static void* shadow_matrix_ptr;
static void* draw_matrix_ptr;
static unsigned shadow_model, draw_model, draw_texture;
static int shadow_mode, draw_mode, draw_last;
static void* shadow_animation = reinterpret_cast<void*>(1);
static void* draw_animation = reinterpret_cast<void*>(1);
static char shadow_flag = 7;
static unsigned draw_flags;
static void* draw_record;
static unsigned char draw_point[12];
static char shadow_token;

extern "C" void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
    void* model, int mode, void* matrix, void* animation, char flag) {
    order[steps++] = 1;
    shadow_model = pointer_word(model);
    shadow_mode = mode;
    shadow_matrix_ptr = matrix;
    std::memcpy(shadow_matrix_copy, matrix, sizeof shadow_matrix_copy);
    shadow_animation = animation;
    shadow_flag = flag;
    return &shadow_token;
}
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* record, int last) {
    order[steps++] = 2;
    draw_model = pointer_word(model);
    draw_texture = pointer_word(texture);
    draw_mode = mode;
    std::memcpy(draw_point, point, 12);
    draw_matrix_ptr = matrix;
    std::memcpy(draw_matrix_copy, matrix, sizeof draw_matrix_copy);
    draw_animation = animation;
    draw_flags = flags;
    draw_record = record;
    draw_last = last;
    return 0;
}

int main() {
    alignas(8) unsigned char object[320];
    std::memset(object, 0, sizeof object);
    const unsigned offsets[12] = {148, 160, 172, 152, 164, 176, 156, 168, 180, 96, 100, 104};
    for (unsigned i = 0; i < 12u; ++i) {
        float value = 1.5f + static_cast<float>(i);
        std::memcpy(object + offsets[i], &value, 4);
    }
    unsigned model = 0x00112233u, texture = 0x00445566u;
    std::memcpy(object + 260, &model, 4);
    std::memcpy(object + 264, &texture, 4);
    msg_draw__15PushPhysicsProp(object);
    expect(steps == 2 && order[0] == 1 && order[1] == 2, "shadow then draw");
    bool matrix_ok = true;
    for (unsigned i = 0; i < 12u; ++i) {
        float want = 1.5f + static_cast<float>(i);
        if (shadow_matrix_copy[i] != want || draw_matrix_copy[i] != want) matrix_ok = false;
    }
    expect(matrix_ok, "matrix floats follow the offset order");
    expect(shadow_matrix_ptr == draw_matrix_ptr && shadow_matrix_ptr != nullptr, "one matrix");
    expect(shadow_model == model && draw_model == model && draw_texture == texture, "model words");
    expect(shadow_mode == 1 && draw_mode == 1 && shadow_flag == 0 && draw_last == 0, "constants");
    expect(shadow_animation == nullptr && draw_animation == nullptr, "no animation");
    expect(draw_flags == 0xffffffffu, "flags are -1");
    expect(draw_record == &shadow_token, "shadow result is the draw record");
    expect(std::memcmp(draw_point, object + 96, 12) == 0, "point is the position bytes");
    return failures ? 1 : 0;
}
