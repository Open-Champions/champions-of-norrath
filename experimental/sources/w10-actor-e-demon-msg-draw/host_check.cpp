// Host contract for msg_draw__5Demon. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_draw.cpp"
#include <cstdio>
#include <initializer_list>
static int order = 0, draw_at = -1, att_at = -1, att_calls = 0;
static void *d_self, *d_model, *d_tex, *d_extra, *ret_token = reinterpret_cast<void*>(0x1234);
static void *a_model, *a_tex, *a_gm, *a_rec; static int a_zero, a_a, a_b; static unsigned long a_mask;
extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra) {
    draw_at = order++; d_self = self; d_model = model; d_tex = texture; d_extra = extra; return ret_token;
}
extern "C" void modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii(
    void* model, void* texture, int zero, void* gm, void* rec, unsigned long mask, int a, int b) {
    att_at = order++; ++att_calls; a_model = model; a_tex = texture; a_zero = zero; a_gm = gm; a_rec = rec; a_mask = mask; a_a = a; a_b = b;
}
int main() {
    alignas(16) unsigned char object[1000] = {}; alignas(16) unsigned char group[64] = {};
    unsigned char m0[4], t0[4], m2[4], t2[4], m3[4], t3[4];
    for (void* p : {(void*)group, (void*)m0, (void*)t0, (void*)m2, (void*)t2, (void*)m3, (void*)t3}) vslots_b::host_note(p);
    using namespace vslots_b;
    write_word(address(object) + 36u, address_bits(group));
    write_word(address(group) + 12u, address_bits(m0)); write_word(address(group) + 20u, address_bits(t0));
    write_word(address(group) + 4 * 8 + 4, address_bits(m2)); write_word(address(group) + 4 * 8 + 12, address_bits(t2));
    write_word(address(group) + 6 * 8 + 4, address_bits(m3)); write_word(address(group) + 6 * 8 + 12, address_bits(t3));
    msg_draw__5Demon(object);
    if (order != 1 || draw_at != 0 || att_calls != 0 || d_self != object || d_model != m0 || d_tex != t0 || d_extra) return 1;
    for (unsigned idx : {4u, 6u}) {
        order = 0; att_calls = 0;
        write_word(address(object) + 960u, idx);
        msg_draw__5Demon(object);
        if (order != 2 || draw_at != 0 || att_at != 1 || att_calls != 1) return 2;
        if (a_model != (idx == 4 ? m2 : m3) || a_tex != (idx == 4 ? t2 : t3) || a_zero != 0 || a_gm != m0 || a_rec != ret_token) return 3;
        if (a_mask != static_cast<unsigned long>(-1) || a_a != -1 || a_b != 0) return 4;
    }
    return 0;
}
