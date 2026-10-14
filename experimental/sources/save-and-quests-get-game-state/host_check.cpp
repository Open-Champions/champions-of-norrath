#include <cstdio>
#include <cstring>
extern "C" {
unsigned char norrath_script_game_state_data_view[8192];
unsigned char _ctype_[257];
}
#include "get_game_state.cpp"
int main() {
    for (int c = 'A'; c <= 'Z'; ++c) _ctype_[c + 1] = 0x01;
    for (int c = 'a'; c <= 'z'; ++c) _ctype_[c + 1] = 0x02;
    unsigned char* t = norrath_script_game_state_data_view;
    std::memset(t, 0, 8192);
    // missing key is appended lower-cased with a zero value
    char k1[] = "QuestOne";
    if (scriptGetGameState__FPc(k1) != 0) return 1;
    if (std::strcmp((char*)t, "questone") != 0 || t[9] != 0) return 2;
    // two existing records, the second with a negative value
    std::memset(t, 0, 8192);
    std::memcpy(t, "door\0\x05" "gate\0\xfe", 12);
    char g[] = "GATE";
    if (scriptGetGameState__FPc(g) != -2) return 3;
    char d[] = "door";
    if (scriptGetGameState__FPc(d) != 5) return 4;
    if (t[12] != 0) return 5;  // lookups do not append
    // a new key lands after the last record
    char n[] = "NEW";
    if (scriptGetGameState__FPc(n) != 0) return 6;
    if (std::strcmp((char*)t + 12, "new") != 0 || t[16] != 0) return 7;
    // a prefix is not a match
    char p[] = "do";
    scriptGetGameState__FPc(p);
    if (std::strcmp((char*)t + 17, "do") != 0) return 8;
    // non-letters and bytes above 127 are left alone
    std::memset(t, 0, 8192);
    char odd[] = {'A', '1', (char)0xc9, '_', 0};
    scriptGetGameState__FPc(odd);
    if ((unsigned char)t[0] != 'a' || t[1] != '1' || (unsigned char)t[2] != 0xc9 || t[3] != '_') return 9;
    std::puts("ok");
    return 0;
}
