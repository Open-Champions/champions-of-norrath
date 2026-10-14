#include <cstdint>
#include <cstdio>
#include "font_init.cpp"
static void* seen = nullptr;
extern "C" void textureInitTexture__FP8_texture(void* texture) { seen = texture; }
static unsigned word_at(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void put_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    unsigned char font[32] = {};
    put_word(font + 8, 4);
    put_word(font + 12, 8);
    put_word(font + 16, 20);
    fontInitFont__FP4Font(font);
    const unsigned base = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(font));
    if (word_at(font + 8) != base + 4u || word_at(font + 12) != base + 8u || word_at(font + 16) != base + 20u) return 1;
    if (seen != font + 20) return 2;
    std::puts("font init contract passed");
    return 0;
}
