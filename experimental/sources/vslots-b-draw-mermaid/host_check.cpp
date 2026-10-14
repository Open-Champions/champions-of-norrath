#define NORRATH_VSLOTS_B_HOST 1
#include "draw_mermaid.cpp"
#include <cstdio>
static int calls = 0;
static void* seen_self = nullptr;
static void* seen_model = nullptr;
static void* seen_texture = nullptr;
static void* seen_extra = reinterpret_cast<void*>(1);
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra) {
    ++calls;
    seen_self = self;
    seen_model = model;
    seen_texture = texture;
    seen_extra = extra;
}
int main() {
    unsigned char object[40];
    unsigned char group[24];
    unsigned char model[4] = {};
    unsigned char texture[4] = {};
    for (unsigned i = 0; i < sizeof object; ++i) object[i] = 0x41;
    vslots_b::host_note(group);
    vslots_b::host_note(model);
    vslots_b::host_note(texture);
    vslots_b::write_word(vslots_b::address(object) + 36u, vslots_b::address_bits(group));
    vslots_b::write_word(vslots_b::address(group) + 12u, vslots_b::address_bits(model));
    vslots_b::write_word(vslots_b::address(group) + 20u, vslots_b::address_bits(texture));
    msg_draw__7Mermaid(object);
    if (calls != 1 || seen_self != object || seen_model != model || seen_texture != texture || seen_extra != nullptr) return 1;
    if (object[0] != 0x41 || object[35] != 0x41) return 1;
    return 0;
}
