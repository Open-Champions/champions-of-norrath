// Host contract for msg_draw__8Froglock. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_draw.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
static int order = 0;
static int fx_at = -1, draw_at = -1, att_calls = 0, fx_calls = 0, att_at = -1;
static void* ret_token = reinterpret_cast<void*>(0x1234);
static void *f_desc, *f_model, *f_state, *f_second, *f_anim, *f_matrix;
static unsigned char f_point[12];
static short f_angle;
static float f_first, f_second_f;
static int f_flag, f_zero;
static void *d_self, *d_model, *d_tex, *d_extra;
static void *a_model, *a_tex, *a_gm, *a_rec;
static int a_zero, a_a, a_b;
static unsigned long a_mask;
extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra) {
    draw_at = order++; d_self = self; d_model = model; d_tex = texture; d_extra = extra; return ret_token;
}
extern "C" void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
    void* model, void* texture, int zero, void* gm, void* rec, unsigned long mask, int a, int b) {
    ++att_calls; att_at = order++; a_model = model; a_tex = texture; a_zero = zero; a_gm = gm;
    a_rec = rec; a_mask = mask; a_a = a; a_b = b;
}
extern "C" void fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34(
    void* description, void* model, void* state, const void* point, short angle, void* second_model, void* animation,
    float first, int flag, int zero, float second, void* matrix) {
    ++fx_calls; fx_at = order++; f_desc = description; f_model = model; f_state = state; std::memcpy(f_point, point, 12);
    f_angle = angle; f_second = second_model; f_anim = animation; f_first = first; f_flag = flag; f_zero = zero;
    f_second_f = second; f_matrix = matrix;
}
int main() {
    using namespace vslots_b;
    alignas(16) static unsigned char object[1300];
    alignas(16) static unsigned char group[128];
    static unsigned char m0[4], t0[4], m2[4], m3[4];
    for (void* p : {(void*)group, (void*)m0, (void*)t0, (void*)m2, (void*)m3, (void*)(t0 + 1), (void*)(t0 + 2)}) host_note(p);
    write_word(address(object) + 36u, address_bits(group));
    write_word(address(group) + 12u, address_bits(m0));
    write_word(address(group) + 20u, address_bits(t0));
    write_word(address(group) + 4 * 8 + 4, address_bits(m2));
    write_word(address(group) + 4 * 8 + 20, address_bits(t0 + 1));
    write_word(address(group) + 10 * 8 + 4, address_bits(m3));
    write_word(address(group) + 10 * 8 + 20, address_bits(t0 + 2));
    for (unsigned i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(0xa0 + i);
    write_u16(address(object) + 56u, 0xfff0u);
    msg_draw__8Froglock(object);
    if (order != 1 || draw_at != 0 || fx_calls || att_calls || d_self != object || d_model != m0 || d_tex != t0 || d_extra) return 1;
    for (unsigned half : {6u, 5u}) {
        order = 0; fx_calls = att_calls = 0;
        write_word(address(object) + 960u, 4u);
        write_u16(address(object) + 572u, half);
        msg_draw__8Froglock(object);
        if (order != 3 || fx_at != 0 || draw_at != 1 || att_at != 2 || fx_calls != 1 || att_calls != 1) return 2;
        if (f_desc != object + 1032 || f_model != m2 || f_state != object + 964 || f_second != m0 || f_anim != object + 288 || f_matrix) return 3;
        if (std::memcmp(f_point, object + 16, 12) != 0 || f_angle != -16 || f_first != 0.5f || f_second_f != 1.0f || f_zero != 0) return 4;
        if (f_flag != (half == 6 ? 1 : 0)) return 5;
        if (a_model != m2 || a_tex != t0 + 1 || a_zero != 0 || a_gm != m0 || a_rec != ret_token ||
            a_mask != static_cast<unsigned long>(-1) || a_a != -1 || a_b != 0) return 6;
    }
    order = 0; fx_calls = att_calls = 0;
    write_word(address(object) + 960u, 10u);
    msg_draw__8Froglock(object);
    if (order != 3 || f_model != m3 || a_model != m3 || a_tex != t0 + 2) return 7;
    return 0;
}
