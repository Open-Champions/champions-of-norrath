#include <cstdio>
extern "C" unsigned char norrath_script_current_amx_word_view[4];
int main() {
    if (sizeof norrath_script_current_amx_word_view != 4) return 1;
    for (unsigned i = 0; i < 4; ++i)
        if (norrath_script_current_amx_word_view[i] != 0) return 2;
    norrath_script_current_amx_word_view[0] = 0x78;
    norrath_script_current_amx_word_view[1] = 0x56;
    norrath_script_current_amx_word_view[2] = 0x34;
    norrath_script_current_amx_word_view[3] = 0x12;
    const unsigned value = unsigned(norrath_script_current_amx_word_view[0])
        | (unsigned(norrath_script_current_amx_word_view[1]) << 8)
        | (unsigned(norrath_script_current_amx_word_view[2]) << 16)
        | (unsigned(norrath_script_current_amx_word_view[3]) << 24);
    if (value != 0x12345678u) return 3;
    std::puts("current AMX word passed");
}
