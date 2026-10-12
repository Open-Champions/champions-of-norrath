#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "msg_draw.cpp"
using namespace vslots_b;
static int failures = 0, order_count = 0, order[8];
static int yaw_calls, rot_calls, draw_calls;
static short seen_yaw, seen_angle;
static float seen_scale;
static int seen_axis;
static unsigned char yaw_point[12];
static unsigned char draw_points[2][12];
static void* yaw_matrix;
static void* rot_matrix;
static void* draw_matrix[2];
static void* models[2];
static void* textures[2];
static unsigned long long flags_seen[2];
static void* anim_seen[2];
static void* record_seen[2];
static int mode_seen[2], last_seen[2];
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void matMakeTransYawScale__FP8Matrix34G6Point3sf(void* matrix, const void* point, short yaw, float scale) {
    ++yaw_calls; order[order_count++] = 1; yaw_matrix = matrix; seen_yaw = yaw; seen_scale = scale; std::memcpy(yaw_point, point, 12);
}
extern "C" void matRot__FP8Matrix34is(void* matrix, int axis, short angle) {
    ++rot_calls; order[order_count++] = 2; rot_matrix = matrix; seen_axis = axis; seen_angle = angle;
}
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last) {
    const int i = draw_calls < 2 ? draw_calls : 1;
    ++draw_calls; order[order_count++] = 3;
    models[i] = model; textures[i] = texture; mode_seen[i] = mode; draw_matrix[i] = matrix;
    std::memcpy(draw_points[i], point, 12); anim_seen[i] = animation; flags_seen[i] = flags;
    record_seen[i] = draw_record; last_seen[i] = last;
    return 0;
}
int main() {
    unsigned char object[1000], group[48], m1[4], t1[4], m2[4], t2[4];
    host_note(group); host_note(m1); host_note(t1); host_note(m2); host_note(t2);
    std::memset(object, 0, sizeof object);
    std::memset(group, 0, sizeof group);
    write_single(address(object) + 16u, 1.5f);
    write_single(address(object) + 20u, -2.5f);
    write_single(address(object) + 24u, 10.0f);
    write_single(address(object) + 32u, 4.0f);
    write_single(address(object) + 224u, 0.75f);
    write_u16(address(object) + 56u, 0xfff0u);
    write_u16(address(object) + 960u, 0x8001u);
    write_u64(address(object) + 552u, 0x1122334455667788ull);
    write_word(address(object) + 36u, address_bits(group));
    write_word(address(group) + 12u, address_bits(m1));
    write_word(address(group) + 20u, address_bits(t1));
    write_word(address(group) + 28u, address_bits(m2));
    write_word(address(group) + 36u, address_bits(t2));
    msg_draw__9CdEyeball(object);
    expect(order_count == 4 && order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 3, "scale-yaw, rotate, two draws");
    expect(seen_yaw == -16 && seen_scale == 0.75f, "signed yaw and scale at +224");
    expect(std::memcmp(yaw_point, object + 16, 12) == 0, "matrix point is the object point");
    expect(seen_axis == 1 && seen_angle == static_cast<short>(0x8001), "axis 1, signed halfword +960");
    expect(yaw_matrix == rot_matrix && rot_matrix == draw_matrix[0] && rot_matrix == draw_matrix[1] && yaw_matrix != nullptr, "one matrix");
    float p[3];
    std::memcpy(p, draw_points[0], 12);
    expect(p[0] == 1.5f && p[1] == -2.5f && p[2] == 12.0f, "draw point z gains half of +32");
    expect(std::memcmp(draw_points[0], draw_points[1], 12) == 0, "both draws use the same point");
    expect(models[0] == m2 && textures[0] == t2, "first draw: group +28 and +36");
    expect(models[1] == m1 && textures[1] == t1, "second draw: group +12 and +20");
    for (int i = 0; i < 2; ++i) {
        expect(mode_seen[i] == 1 && anim_seen[i] == nullptr && record_seen[i] == nullptr && last_seen[i] == 0, "draw tail arguments");
        expect(flags_seen[i] == 0x1122334455667788ull, "flags are the 8 bytes at +552");
    }
    return failures == 0 ? 0 : 1;
}
