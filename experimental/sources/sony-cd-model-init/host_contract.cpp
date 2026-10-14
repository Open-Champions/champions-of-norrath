#include <cstdint>
#include <cstdio>
#include "model_init.cpp"
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
    unsigned char model[64] = {};
    model[18] = 1;
    model[38] = 2;
    put_word(model + 40, 4);
    put_word(model + 44, 8);
    put_word(model + 48, 12);
    put_word(model + 52, 16);
    modelInitModel__FPv(model);
    const unsigned base = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(model));
    if (word_at(model + 40) != base + 4u || word_at(model + 52) != base + 16u) return 1;
    if (model[38] != 2) return 2;
    unsigned char empty[64] = {};
    empty[18] = static_cast<unsigned char>(-1);
    empty[38] = 0;
    modelInitModel__FPv(empty);
    if (empty[38] != 1 || word_at(empty + 40) != 0u) return 3;
    unsigned char replaced[80] = {};
    replaced[18] = 0;
    replaced[38] = 0xff;
    put_word(replaced + 40, 3);
    modelInitModel__FPv(replaced);
    const unsigned replaced_base = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(replaced));
    if (replaced[38] != 1 || word_at(replaced + 40) != replaced_base + 3u) return 4;
    std::puts("model init contract passed");
    return 0;
}
