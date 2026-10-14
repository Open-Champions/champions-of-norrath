#include <cstdio>
#include <cstring>
extern "C" int amxNewWorld__FP5__amxPi(void*, int*);
extern "C" signed char scriptInPostAction = 0;
extern "C" signed char netNetworkMode = 0;
extern "C" int activePlayer = 0;
extern "C" unsigned char norrath_player_progression_records_view[23984] = {};
static int kicks = 0;
static int messages = 0;
static int local_player = 1;
static char world[32];
static unsigned short text_marker = 7;
extern "C" int amx_GetAddr(void*, int, void** physical) {
    static char packed[4];
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "faydark");
    return 0;
}
extern "C" int netPlayerIsLocal__Fi(int player) { return player == 0 && local_player; }
extern "C" unsigned short* textGetString__FPc(char* key) {
    if (std::strcmp(key, "noZone3") != 0) return nullptr;
    return &text_marker;
}
extern "C" int netGetLocalPlayerNm__Fi(int code) { return code == 0 ? 4 : -1; }
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int player, int color) {
    messages += 1;
    if (text != &text_marker || duration != 180 || player != 4 || color != 0) messages = -100;
}
extern "C" void gameNetKick__Fi(int code) { kicks += code == 0 ? 1 : 100; }
extern "C" void doNewWorld__FPc(char* name) { std::strcpy(world, name); }
static unsigned short half_at() {
    const unsigned char* bytes = norrath_player_progression_records_view + 23970;
    return static_cast<unsigned short>(bytes[0] | (bytes[1] << 8));
}

int main() {
    int cells[] = {4, 3};
    norrath_player_progression_records_view[23970] = 9;
    norrath_player_progression_records_view[23971] = 0;
    scriptInPostAction = 1;
    netNetworkMode = 1;
    if (amxNewWorld__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (kicks != 1 || std::strcmp(world, "faydark") != 0 || half_at() != 9) return 2;
    scriptInPostAction = 0;
    netNetworkMode = 0;
    if (amxNewWorld__FP5__amxPi(nullptr, cells) != 0) return 3;
    if (kicks != 2 || half_at() != 9) return 4;
    netNetworkMode = 1;
    activePlayer = -1;
    if (amxNewWorld__FP5__amxPi(nullptr, cells) != 0) return 5;
    if (kicks != 3 || half_at() != 9) return 6;
    activePlayer = 0;
    norrath_player_progression_records_view[23970] = 0;
    if (amxNewWorld__FP5__amxPi(nullptr, cells) != 0) return 7;
    if (kicks != 4 || half_at() != 900) return 8;
    norrath_player_progression_records_view[23970] = 3;
    norrath_player_progression_records_view[23971] = 0;
    local_player = 0;
    if (amxNewWorld__FP5__amxPi(nullptr, cells) != 0) return 9;
    if (kicks != 4 || messages != 0 || half_at() != 3) return 10;
    local_player = 1;
    if (amxNewWorld__FP5__amxPi(nullptr, cells) != 0) return 11;
    if (kicks != 4 || messages != 1) return 12;
    std::puts("amxNewWorld passed");
    return 0;
}
