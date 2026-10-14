#define NORRATH_VSLOTS_B_HOST 1
#include <cstdio>
#include <cstring>
#include "msg_draw.cpp"
static int failures = 0, order_count = 0, order[8];
static int yaw_calls = 0, halo_calls = 0, shadow_calls = 0, draw_calls = 0, reset_calls = 0, isin_calls = 0;
static short seen_yaw;
static unsigned char yaw_point[12], draw_point[12], halo_copy[8];
static void *yaw_matrix, *draw_matrix, *shadow_matrix, *seen_model, *shadow_model, *seen_texture, *seen_anim, *shadow_anim, *seen_record, *halo_ptr, *first_halo_ptr;
static int seen_mode, seen_last, seen_halo_value, seen_isin_arg, shadow_value;
static signed char shadow_flag = 99;
static unsigned long long seen_flags;
static float isin_result = 0.0f;
static int marker;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" int frameNm = 0;
extern "C" float isin__Fi(int angle) { ++isin_calls; order[order_count++] = 5; seen_isin_arg = angle; return isin_result; }
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw) {
    ++yaw_calls; order[order_count++] = 1; yaw_matrix = matrix; seen_yaw = yaw; std::memcpy(yaw_point, point, 12);
}
extern "C" void modelSetHalo__FP7HaloDefi(void* halo, int value) {
    ++halo_calls; order[order_count++] = 2; halo_ptr = halo; seen_halo_value = value; std::memcpy(halo_copy, halo, 8);
    if (first_halo_ptr == nullptr) first_halo_ptr = halo;
}
extern "C" void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
    void* model, int value, void* matrix, void* animation, signed char flag) {
    ++shadow_calls; order[order_count++] = 3; shadow_model = model; shadow_value = value; shadow_matrix = matrix;
    shadow_anim = animation; shadow_flag = flag;
    return &marker;
}
extern "C" int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last) {
    ++draw_calls; order[order_count++] = 4;
    seen_model = model; seen_texture = texture; seen_mode = mode; draw_matrix = matrix;
    std::memcpy(draw_point, point, 12); seen_anim = animation; seen_flags = flags;
    seen_record = draw_record; seen_last = last;
    return 0;
}
extern "C" void modelResetOptionalParams__Fv() { ++reset_calls; order[order_count++] = 6; }
static void reset() {
    yaw_calls = halo_calls = shadow_calls = draw_calls = reset_calls = isin_calls = 0;
    order_count = 0;
}
int main() {
    using namespace vslots_b;
    unsigned char object[400], model[4], texture[4];
    host_note(model);
    host_note(texture);
    std::memset(object, 0, sizeof object);
    for (unsigned i = 0; i < 12u; ++i) object[16 + i] = static_cast<unsigned char>(0x40 + i);
    write_u16(address(object) + 56u, 0xc001u);
    write_word(address(object) + 76u, address_bits(model));
    write_word(address(object) + 80u, address_bits(texture));
    write_u64(address(object) + 352u, 0x1122334455667788ull);
    write_word(address(object) + 360u, 5u);
    frameNm = 8;  // 8 - 3 is not below 5: no halo
    reset();
    msg_draw__13UserParamProp(object);
    expect(halo_calls == 0 && isin_calls == 0, "no halo when the word is not above frameNm - 3");
    expect(order_count == 4 && order[0] == 1 && order[1] == 3 && order[2] == 4 && order[3] == 6, "unlit order");
    expect(seen_yaw == 1, "0xc001 plus 16384 wraps to 1");
    expect(std::memcmp(yaw_point, object + 16, 12) == 0 && std::memcmp(draw_point, object + 16, 12) == 0, "point copies");
    expect(yaw_matrix == shadow_matrix && yaw_matrix == draw_matrix, "one local matrix");
    expect(shadow_model == model && shadow_value == 1 && shadow_anim == object + 84 && shadow_flag == 0, "shadow arguments");
    expect(seen_model == model && seen_texture == texture && seen_mode == 1 && seen_anim == object + 84, "draw arguments");
    expect(seen_record == &marker && seen_last == 0, "shadow result becomes the record");
    expect(seen_flags == 0x1122334455667788ull, "flags are the 8 bytes at object+352");
    frameNm = 7;  // 4 < 5: halo
    isin_result = -0.5f;  // abs 0.5 * 12 + 12 = 18
    reset();
    msg_draw__13UserParamProp(object);
    expect(halo_calls == 1 && isin_calls == 1 && seen_isin_arg == 7 << 9, "halo and isin once, isin of frameNm << 9");
    expect(order_count == 6 && order[0] == 1 && order[1] == 5 && order[2] == 2 && order[3] == 3 && order[4] == 4 && order[5] == 6, "lit order");
    expect(seen_halo_value == 1, "halo value 1");
    const unsigned char want[8] = {0xff, 0xff, 0xff, 18, 0x00, 0x80, 0xcc, 0x43};
    expect(std::memcmp(halo_copy, want, 8) == 0, "byte 3 is 18, the rest keep their initial bytes");
    isin_result = 0.99f;  // 0.99 * 12 + 12 = 23.88 -> 23
    reset();
    msg_draw__13UserParamProp(object);
    expect(halo_copy[3] == 23 && halo_ptr == first_halo_ptr, "truncation, and the same static halo each time");
    frameNm = -100;
    write_word(address(object) + 360u, 0xfffffff0u);
    reset();
    msg_draw__13UserParamProp(object);
    expect(halo_calls == 1, "signed compare: -103 is below -16");
    frameNm = 5;
    reset();
    msg_draw__13UserParamProp(object);
    expect(halo_calls == 0, "signed compare: 2 is not below -16");
    return failures == 0 ? 0 : 1;
}
