#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "msg_draw.cpp"
static int failures = 0;
static int yaw_calls = 0, ambient_calls = 0, reset_calls = 0, draw_calls = 0;
static unsigned order[8];
static unsigned order_count = 0;
static short seen_yaw = 0;
static unsigned char yaw_point[12], draw_point[12], ambient[8];
static void *yaw_matrix, *draw_matrix, *seen_model, *seen_texture, *seen_anim, *seen_record;
static int seen_mode, seen_last;
static unsigned long long seen_flags;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" int frameNm = 0;
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    ++yaw_calls; order[order_count++] = 1; yaw_matrix = matrix; seen_yaw = yaw; std::memcpy(yaw_point, point, 12);
}
extern "C" void lightSetAmbientOffset__FPs(void* offsets) {
    ++ambient_calls; order[order_count++] = 2; std::memcpy(ambient, offsets, 8);
}
extern "C" void lightResetOptionalParams__Fv() { ++reset_calls; order[order_count++] = 4; }
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last) {
    ++draw_calls; order[order_count++] = 3;
    seen_model = model; seen_texture = texture; seen_mode = mode; draw_matrix = matrix;
    std::memcpy(draw_point, point, 12); seen_anim = animation; seen_flags = flags;
    seen_record = draw_record; seen_last = last;
    return 0;
}
static void reset() {
    yaw_calls = ambient_calls = reset_calls = draw_calls = 0;
    order_count = 0;
    std::memset(ambient, 0x55, sizeof ambient);
}
int main() {
    unsigned char object[400];
    unsigned char model[4], texture[4];
    using namespace vslots_b;
    host_note(model);
    host_note(texture);
    std::memset(object, 0, sizeof object);
    for (unsigned i = 0; i < 12u; ++i) object[16 + i] = static_cast<unsigned char>(0x40 + i);
    write_u16(address(object) + 56u, 0xc001u);
    write_word(address(object) + 76u, address_bits(model));
    write_word(address(object) + 80u, address_bits(texture));
    write_u64(address(object) + 232u, 0x1122334455667788ull);
    write_u16(address(object) + 320u, 0u);
    write_word(address(object) + 260u, 5u);
    frameNm = 8;  // 8 - 3 is not below 5: no lighting calls
    reset();
    msg_draw__4Prop(object);
    expect(yaw_calls == 1 && draw_calls == 1 && ambient_calls == 0 && reset_calls == 0, "unlit call counts");
    expect(order_count == 2 && order[0] == 1 && order[1] == 3, "matrix before draw");
    expect(seen_yaw == 1, "0xc001 plus 16384 wraps to 1");
    expect(std::memcmp(yaw_point, object + 16, 12) == 0 && std::memcmp(draw_point, object + 16, 12) == 0, "point copies");
    expect(yaw_matrix == object + 272 && draw_matrix == object + 272, "matrix at object+272");
    expect(seen_model == model && seen_texture == texture && seen_mode == 1, "model, texture, mode");
    expect(seen_anim == nullptr && seen_record == nullptr && seen_last == 0, "null animation, record, last 0");
    expect(seen_flags == 0x1122334455667788ull, "flags are the 8 bytes at object+232");
    frameNm = 7;  // 7 - 3 = 4 < 5: lit
    reset();
    msg_draw__4Prop(object);
    expect(ambient_calls == 1 && reset_calls == 1, "lit call counts");
    expect(order_count == 4 && order[0] == 1 && order[1] == 2 && order[2] == 3 && order[3] == 4, "lit order");
    const unsigned char want[8] = {0, 0, 0, 0, 0, 0, 32, 0};
    expect(std::memcmp(ambient, want, 8) == 0, "ambient local is zero with 32 at byte 6");
    frameNm = 8;
    write_u16(address(object) + 320u, 3u);  // bit 0 set
    write_u16(address(object) + 56u, 0x0010u);
    reset();
    msg_draw__4Prop(object);
    expect(yaw_calls == 0 && draw_calls == 1, "bit 0 set skips the matrix");
    write_u16(address(object) + 320u, 2u);
    reset();
    msg_draw__4Prop(object);
    expect(yaw_calls == 1 && seen_yaw == 0x4010, "bit 1 alone does not skip");
    write_word(address(object) + 260u, 0xfffffff0u);  // negative: signed compare
    frameNm = -100;
    reset();
    msg_draw__4Prop(object);
    expect(ambient_calls == 1 && reset_calls == 1, "signed compare: -103 is below -16");
    frameNm = 5;
    reset();
    msg_draw__4Prop(object);
    expect(ambient_calls == 0 && reset_calls == 0, "signed compare: 2 is not below -16");
    return failures == 0 ? 0 : 1;
}
