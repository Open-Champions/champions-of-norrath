// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp sif_get_sreg.cpp
extern "C" {
unsigned norrath_sound_system_register_words_view[32];
unsigned sceSifGetSreg(int index);
}
int main() {
    for (unsigned i = 0; i < 32; ++i) norrath_sound_system_register_words_view[i] = 0x1000u + i * 7u;
    // each index reads its own word; neighbours are not mixed in
    for (int i = 0; i < 32; ++i)
        if (sceSifGetSreg(i) != 0x1000u + static_cast<unsigned>(i) * 7u) return 1;
    // the array is read live, not cached
    norrath_sound_system_register_words_view[5] = 0xdeadbeefu;
    if (sceSifGetSreg(5) != 0xdeadbeefu || sceSifGetSreg(4) != 0x1000u + 28u) return 2;
    // no bound or sign handling: the highest word is reachable and the full 32 bits are returned
    norrath_sound_system_register_words_view[31] = 0x80000001u;
    if (sceSifGetSreg(31) != 0x80000001u) return 3;
    return 0;
}
