#define NORRATH_VSLOTS_B_HOST 1
#include "draw_spider.cpp"
#include <cstdio>
static int step = 0;
static float seen_brightness = 1.0f;
static void* seen_extra = reinterpret_cast<void*>(1);
extern "C" void modelSetShadowBrightness__Ff(float brightness) {
    if (step != 0) step = -10;
    seen_brightness = brightness;
    ++step;
}
extern "C" void draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* extra) {
    (void)self; (void)model; (void)texture;
    if (step != 1) step = -10;
    seen_extra = extra;
    ++step;
}
extern "C" void modelResetOptionalParams__Fv() {
    if (step != 2) step = -10;
    ++step;
}
int main() {
    unsigned char object[40];
    unsigned char group[24];
    unsigned char model[4] = {};
    unsigned char texture[4] = {};
    vslots_b::host_note(group);
    vslots_b::host_note(model);
    vslots_b::host_note(texture);
    vslots_b::write_word(vslots_b::address(object) + 36u, vslots_b::address_bits(group));
    vslots_b::write_word(vslots_b::address(group) + 12u, vslots_b::address_bits(model));
    vslots_b::write_word(vslots_b::address(group) + 20u, vslots_b::address_bits(texture));
    msg_draw__11SmallSpider(object);
    if (step != 3 || seen_brightness != 0.0f || seen_extra != nullptr) return 1;
    return 0;
}
