#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "msg_draw.cpp"
static int failures = 0, yaw_calls = 0, draw_calls = 0;
static short seen_yaw;
static unsigned char yaw_point[12], draw_point[12];
static void *yaw_matrix, *draw_matrix, *seen_model, *seen_texture, *seen_anim, *seen_record;
static int seen_mode, seen_last;
static unsigned seen_flags;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    ++yaw_calls; yaw_matrix = matrix; seen_yaw = yaw; std::memcpy(yaw_point, point, 12);
}
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned flags, void* draw_record, int last) {
    if (yaw_calls == 0) return -1;
    ++draw_calls; seen_model = model; seen_texture = texture; seen_mode = mode; draw_matrix = matrix;
    std::memcpy(draw_point, point, 12); seen_anim = animation; seen_flags = flags;
    seen_record = draw_record; seen_last = last;
    return 0;
}
int main() {
    using namespace vslots_b;
    unsigned char object[160], model[4], texture[4];
    host_note(model);
    host_note(texture);
    std::memset(object, 0, sizeof object);
    for (unsigned i = 0; i < 12u; ++i) object[16 + i] = static_cast<unsigned char>(0x40 + i);
    write_u16(address(object) + 56u, 0xc001u);
    write_word(address(object) + 92u, address_bits(model));
    write_word(address(object) + 96u, address_bits(texture));
    write_word(address(object) + 144u, 0xdeadbeefu);
    msg_draw__16CosmeticPropAnim(object);
    expect(yaw_calls == 1 && draw_calls == 1, "one call each, matrix first");
    expect(seen_yaw == 1, "0xc001 plus 16384 wraps to 1");
    expect(std::memcmp(yaw_point, object + 16, 12) == 0 && std::memcmp(draw_point, object + 16, 12) == 0, "point copies");
    expect(yaw_matrix == draw_matrix && yaw_matrix != nullptr, "same local matrix");
    expect(seen_model == model && seen_texture == texture && seen_mode == 1, "model, texture, mode");
    expect(seen_anim == object + 100 && seen_flags == 0xdeadbeefu, "animation at +100, flags at +144");
    expect(seen_record == nullptr && seen_last == 0, "null record, last 0");
    write_u16(address(object) + 56u, 0x7fffu);
    msg_draw__16CosmeticPropAnim(object);
    expect(seen_yaw == static_cast<short>(0xbfff), "sum is sign-extended through 16 bits");
    return failures == 0 ? 0 : 1;
}
