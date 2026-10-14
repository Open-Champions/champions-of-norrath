#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "msg_draw.cpp"
using namespace vslots_b;
static int failures = 0, order_count = 0, order[8];
static int yaw_calls, hermite_calls, rot_calls, draw_calls;
static short seen_yaw, seen_rot_angle;
static int seen_axis, seen_mode, seen_last;
static float herm[5];
static float herm_result;
static unsigned char yaw_point[12], draw_point[12];
static void *yaw_matrix, *rot_matrix, *draw_matrix, *seen_model, *seen_texture, *seen_anim, *seen_record;
static unsigned seen_flags;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    ++yaw_calls; order[order_count++] = 1; yaw_matrix = matrix; seen_yaw = yaw; std::memcpy(yaw_point, point, 12);
}
extern "C" float evalHermite__Ffffff(float a, float b, float c, float d, float e) {
    ++hermite_calls; order[order_count++] = 2;
    herm[0] = a; herm[1] = b; herm[2] = c; herm[3] = d; herm[4] = e;
    return herm_result;
}
extern "C" void matRot__FP8Matrix33is(void* matrix, int axis, short angle) {
    ++rot_calls; order[order_count++] = 3; rot_matrix = matrix; seen_axis = axis; seen_rot_angle = angle;
}
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last) {
    ++draw_calls; order[order_count++] = 4;
    seen_model = model; seen_texture = texture; seen_mode = mode; draw_matrix = matrix;
    std::memcpy(draw_point, point, 12); seen_anim = animation; seen_flags = flags;
    seen_record = draw_record; seen_last = last;
    return 0;
}
int main() {
    unsigned char object[100], group[32], model[4], texture[4];
    host_note(group); host_note(model); host_note(texture);
    std::memset(object, 0, sizeof object);
    std::memset(group, 0, sizeof group);
    for (unsigned i = 0; i < 12u; ++i) object[16 + i] = static_cast<unsigned char>(0x40 + i);
    write_word(address(object) + 36u, address_bits(group));
    write_word(address(group) + 12u, address_bits(model));
    write_word(address(group) + 20u, address_bits(texture));
    write_u16(address(object) + 56u, 0xfff0u);
    write_single(address(object) + 76u, 0.25f);
    herm_result = -123.9f;
    msg_draw__5Lever(object);
    expect(order_count == 4 && order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 4, "yaw, hermite, rotate, draw");
    expect(seen_yaw == -16, "yaw is the signed halfword, no offset");
    expect(herm[0] == 0.25f && herm[1] == -7000.0f && herm[2] == 7000.0f && herm[3] == 0.0f && herm[4] == 0.0f, "hermite arguments");
    expect(seen_axis == 0 && seen_rot_angle == -123, "axis 0, angle truncated toward zero");
    expect(yaw_matrix == rot_matrix && rot_matrix == draw_matrix && yaw_matrix != nullptr, "one matrix");
    expect(std::memcmp(yaw_point, object + 16, 12) == 0 && std::memcmp(draw_point, object + 16, 12) == 0, "point copies");
    expect(seen_model == model && seen_texture == texture && seen_mode == 3, "model +12, texture +20, mode 3");
    expect(seen_anim == nullptr && seen_flags == 0xffffffffu && seen_record == nullptr && seen_last == 0, "draw tail arguments");
    herm_result = 40000.0f;  // 40000 = 0x9c40 wraps to -25536 through 16 bits
    msg_draw__5Lever(object);
    expect(seen_rot_angle == -25536, "angle narrows to 16 bits");
    return failures == 0 ? 0 : 1;
}
