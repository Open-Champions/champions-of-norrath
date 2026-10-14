#include <cstdint>
#include <cstdio>
#include "texture_init.cpp"
static unsigned word_at(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static int half_at(const unsigned char* slot) {
    const unsigned value = slot[0] | (slot[1] << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 65536;
}
static void put_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    unsigned char ready[32] = {};
    put_word(ready + 28, 4);
    textureInitTexture__FP8_texture(ready);
    if (word_at(ready + 28) != 4u || half_at(ready + 10) != 0) return 1;
    unsigned char flagged[80] = {};
    flagged[8] = 1;
    put_word(flagged + 16, 32);
    textureInitTexture__FP8_texture(flagged);
    const unsigned flagged_base = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(flagged));
    if (word_at(flagged + 28) != 0xffffffffu || half_at(flagged + 10) != -22533) return 2;
    if (word_at(flagged + 16) != flagged_base + 32u || half_at(flagged + 6) != 0) return 3;
    unsigned char texture[128] = {};
    texture[9] = 1;
    put_word(texture + 16, 64);
    put_word(texture + 64, 80);
    texture[68] = 0;
    texture[69] = 0;
    texture[70] = 0;
    texture[71] = 0;
    put_word(texture + 72, 90);
    texture[76] = 0xff;
    textureInitTexture__FP8_texture(texture);
    const unsigned base = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(texture));
    if (word_at(texture + 64) != base + 80u || word_at(texture + 72) != base + 90u) return 4;
    if (half_at(texture + 6) != 94) return 5;
    unsigned char skipped[96] = {};
    skipped[9] = 1;
    put_word(skipped + 16, 40);
    put_word(skipped + 40, 8);
    skipped[44] = 0;
    skipped[45] = 5;
    skipped[46] = 0;
    skipped[47] = 4;
    skipped[48] = 0xff;
    put_word(skipped + 52, 7);
    textureInitTexture__FP8_texture(skipped);
    if (word_at(skipped + 52) != 7u || half_at(skipped + 6) != 78) return 6;
    std::puts("texture init contract passed");
    return 0;
}
