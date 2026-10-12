#include <cstdio>
#include <cstring>
extern "C" {
unsigned char norrath_script_game_state_data_view[8192];
unsigned char _ctype_[257];
}
#include "set_game_state.cpp"
int main() {
    for (int c = 'A'; c <= 'Z'; ++c) _ctype_[c + 1] = 0x01;
    for (int c = 'a'; c <= 'z'; ++c) _ctype_[c + 1] = 0x02;
    unsigned char* t = norrath_script_game_state_data_view;
    std::memset(t, 0, 8192);
    char a[] = "Door";
    scriptSetGameState__FPci(a, 5);
    if (std::strcmp((char*)t, "door") != 0 || t[5] != 5 || t[6] != 0) return 1;
    // update in place, case-insensitively
    char b[] = "DOOR";
    scriptSetGameState__FPci(b, -7);
    if ((signed char)t[5] != -7 || t[6] != 0) return 2;
    // a second key follows the first
    char c2[] = "gate";
    scriptSetGameState__FPci(c2, 1);
    if (std::strcmp((char*)t + 6, "gate") != 0 || t[11] != 1 || t[12] != 0) return 3;
    if ((signed char)t[5] != -7) return 4;
    // clamping, with both edges and the values just outside
    scriptSetGameState__FPci(c2, 300);
    if (t[11] != 127) return 5;
    scriptSetGameState__FPci(c2, 127);
    if (t[11] != 127) return 6;
    scriptSetGameState__FPci(c2, -128);
    if ((signed char)t[11] != -128) return 7;
    scriptSetGameState__FPci(c2, -129);
    if ((signed char)t[11] != -128) return 8;
    scriptSetGameState__FPci(c2, -100000);
    if ((signed char)t[11] != -128) return 9;
    scriptSetGameState__FPci(c2, 126);
    if (t[11] != 126) return 10;
    // a prefix of an existing name is a different key
    char d[] = "do";
    scriptSetGameState__FPci(d, 3);
    if (std::strcmp((char*)t + 12, "do") != 0 || t[15] != 3) return 11;
    std::puts("ok");
    return 0;
}
