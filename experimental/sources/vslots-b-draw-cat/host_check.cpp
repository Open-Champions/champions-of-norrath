#define NORRATH_VSLOTS_B_HOST 1
#include "draw_cat.cpp"
#include <cstdio>
static int calls = 0;
static unsigned during = 0;
static void* seen_model = nullptr;
static void* seen_texture = nullptr;
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra) {
    ++calls;
    during = vslots_b::read_u16(vslots_b::address(self) + 56u);
    vslots_b::write_u16(vslots_b::address(self) + 56u, during + 1u);
    seen_model = model;
    seen_texture = texture;
    if (extra != nullptr) calls = -10;
}
int main() {
    unsigned char object[1000];
    unsigned char group[64];
    unsigned char model[4] = {};
    unsigned char texture[4] = {};
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0;
    vslots_b::host_note(group);
    vslots_b::host_note(model);
    vslots_b::host_note(texture);
    vslots_b::write_word(vslots_b::address(object) + 36u, vslots_b::address_bits(group));
    vslots_b::write_word(vslots_b::address(object) + 960u, 2u);
    vslots_b::write_u16(vslots_b::address(object) + 56u, 30u);
    vslots_b::write_u16(vslots_b::address(object) + 976u, 7u);
    vslots_b::write_word(vslots_b::address(group) + 12u, vslots_b::address_bits(model));
    vslots_b::host_note(group + 16);
    vslots_b::write_word(vslots_b::address(group) + 36u, vslots_b::address_bits(texture));
    msg_draw__3Cat(object);
    if (calls != 1 || during != 23u || seen_model != model || seen_texture != texture) return 1;
    if (vslots_b::read_u16(vslots_b::address(object) + 56u) != 31u) return 1;
    return 0;
}
