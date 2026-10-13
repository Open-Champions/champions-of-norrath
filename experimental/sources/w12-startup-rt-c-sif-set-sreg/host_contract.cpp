// Host check only; not an EE build input. Build the source with -DsceSifSetSreg=w12_set.
#include <cstdio>
extern "C" {
unsigned norrath_sound_system_register_words_view[8];
unsigned w12_set(int, unsigned);
}
int main() {
    for (int i = 0; i < 8; ++i) norrath_sound_system_register_words_view[i] = 0x11111111u * (i + 1);
    if (w12_set(3, 0xdeadbeefu) != 0xdeadbeefu) return 1;
    for (int i = 0; i < 8; ++i)
        if (norrath_sound_system_register_words_view[i] != (i == 3 ? 0xdeadbeefu : 0x11111111u * (i + 1))) return 2;
    if (w12_set(0, 0) != 0 || norrath_sound_system_register_words_view[0] != 0) return 3;
    if (w12_set(7, 0x80000000u) != 0x80000000u || norrath_sound_system_register_words_view[7] != 0x80000000u) return 4;
    std::puts("sceSifSetSreg ok");
    return 0;
}
