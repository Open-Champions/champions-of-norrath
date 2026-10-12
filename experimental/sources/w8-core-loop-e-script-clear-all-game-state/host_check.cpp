// Host check for scriptClearAllGameState__Fv. Not an EE plan input.
#include <cstring>
extern "C" unsigned char norrath_script_game_state_data_view[8192] = {0};
extern "C" unsigned norrath_host_strlen(const char* text) { return (unsigned)std::strlen(text); }
#define strlen norrath_host_strlen
#include "script_clear_all_game_state.cpp"
#undef strlen
int main() {
    unsigned char* d = norrath_script_game_state_data_view;
    // Empty first byte: nothing changes.
    std::memset(d, 0x55, 64); d[0] = 0;
    scriptClearAllGameState__Fv();
    for (int i = 1; i < 64; ++i) if (d[i] != 0x55) return 1;
    // Three strings, each followed by one byte that is cleared; a zero after that ends the walk.
    std::memset(d, 0x55, 64);
    const unsigned char in[] = {'a','b',0,'X','c','d','e',0,'Y','f',0,'Z',0,0x77};
    std::memcpy(d, in, sizeof in);
    scriptClearAllGameState__Fv();
    const unsigned char want[] = {'a','b',0,0,'c','d','e',0,0,'f',0,0,0,0x77};
    if (std::memcmp(d, want, sizeof want) != 0) return 2;
    if (d[14] != 0x55) return 3;
    // A non-zero byte after the skipped byte keeps the loop going.
    std::memset(d, 0x55, 64);
    const unsigned char in2[] = {'q',0,'X','r',0,'Y',0};
    std::memcpy(d, in2, sizeof in2);
    scriptClearAllGameState__Fv();
    const unsigned char want2[] = {'q',0,0,'r',0,0,0};
    if (std::memcmp(d, want2, sizeof want2) != 0) return 4;
    if (d[7] != 0x55) return 5;
    return 0;
}
