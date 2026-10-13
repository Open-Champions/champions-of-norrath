// Host contract for msg_draw__7Cyclops. Includes the source; not an EE input.
#define NORRATH_VSLOTS_B_HOST 1
#include "msg_draw.cpp"
#include <cstdio>
static int order = 0, scale_at = -1, draw_at = -1;
static float scale = 0; static void *s_self, *s_model, *s_tex, *s_extra;
extern "C" void modelSetShadowScale__Ff(float v) { scale = v; scale_at = order++; }
extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra) {
    draw_at = order++; s_self = self; s_model = model; s_tex = texture; s_extra = extra; return nullptr;
}
int main() {
    unsigned char object[64] = {}; unsigned char group[32] = {}; unsigned char model[4], texture[4];
    vslots_b::host_note(group); vslots_b::host_note(model); vslots_b::host_note(texture);
    vslots_b::write_word(vslots_b::address(object) + 36u, vslots_b::address_bits(group));
    vslots_b::write_word(vslots_b::address(group) + 12u, vslots_b::address_bits(model));
    vslots_b::write_word(vslots_b::address(group) + 20u, vslots_b::address_bits(texture));
    msg_draw__7Cyclops(object);
    if (order != 2 || scale_at != 0 || draw_at != 1 || scale != 3.0f) return 1;
    if (s_self != object || s_model != model || s_tex != texture || s_extra != nullptr) return 2;
    return 0;
}
