#include <cstdint>
#include <cstdio>
#include "anim_init.cpp"
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
    alignas(8) unsigned char header[96] = {};
    put_word(header + 8, 1);
    put_word(header + 12, 2);
    put_word(header + 16, 3);
    put_word(header + 52, 64);
    put_word(header + 56, 9);
    put_word(header + 64, 11);
    put_word(header + 72, 13);
    put_word(header + 80, 0);
    animInitAnimation__FP15AnimationHeader(header);
    const unsigned base = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(header));
    if (word_at(header + 8) != base + 1u || word_at(header + 12) != base + 2u || word_at(header + 16) != base + 3u)
        return 1;
    if (word_at(header + 52) != base + 64u || word_at(header + 56) != base + 9u) return 2;
    if (word_at(header + 64) != base + 11u || word_at(header + 72) != base + 13u || word_at(header + 80) != 0u) return 3;
    unsigned char plain[64] = {};
    put_word(plain + 52, 0);
    put_word(plain + 56, 0);
    animInitAnimation__FP15AnimationHeader(plain);
    if (word_at(plain + 52) != 0u || word_at(plain + 56) != 0u) return 4;
    std::puts("anim init contract passed");
    return 0;
}
