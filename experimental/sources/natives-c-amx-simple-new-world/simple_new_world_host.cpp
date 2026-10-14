#include <cstdio>
#include <cstring>

extern "C" int amxSimpleNewWorld__FP5__amxPi(void*, int*);
extern "C" unsigned char scriptInPostAction = 0;
extern "C" unsigned char netNetworkMode = 0;
extern "C" int activePlayer = 0;
extern "C" unsigned char player[96000] = {};
extern "C" char levelName[322] = {};

static int kicks;
static char copied[160];
static char looked[32];
static int ticks;
static int message_player;
static int unused;
static int local = 1;
static int local_id = 7;
static unsigned short text_body[] = {1, 0};
static unsigned short* shown;

extern "C" int amx_GetAddr(void*, int, int** physical) {
    static int slot;
    *physical = &slot;
    return 0;
}

extern "C" int amx_GetString(char* dest, int*) {
    std::snprintf(dest, 160, "%s", "arena");
    return 0;
}

extern "C" int netPlayerIsLocal__Fi(int player) {
    return player == activePlayer ? local : 0;
}

extern "C" unsigned short* textGetString__FPc(char* key) {
    std::snprintf(looked, sizeof looked, "%s", key);
    return text_body;
}

extern "C" int netGetLocalPlayerNm__Fi(int player) {
    return player == 0 ? local_id : -1;
}

extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int duration, int player, int extra) {
    shown = text;
    ticks = duration;
    message_player = player;
    unused = extra;
}

extern "C" void gameNetKick__Fi(int reason) {
    if (reason == 0) kicks += 1;
}

extern "C" char* strcpy(char* dest, const char* source) {
    std::snprintf(copied, sizeof copied, "%s", source);
    char* out = dest;
    while ((*out++ = *source++) != '\0') {
    }
    return dest;
}

static unsigned half_at(int player_index) {
    const unsigned char* slot = player + 23968 + player_index * 23984 + 2;
    return slot[0] | (static_cast<unsigned>(slot[1]) << 8);
}

int main() {
    int params[2] = {4, 1};
    scriptInPostAction = 1;
    netNetworkMode = 1;
    activePlayer = 0;
    levelName[80] = 0;
    if (amxSimpleNewWorld__FP5__amxPi(nullptr, params) != 0) return 1;
    if (kicks != 1 || levelName[80] != static_cast<char>(0xff) || std::strcmp(copied, "arena") != 0) return 2;

    scriptInPostAction = 0;
    netNetworkMode = 0;
    kicks = 0;
    if (amxSimpleNewWorld__FP5__amxPi(nullptr, params) != 0 || kicks != 1) return 3;

    netNetworkMode = 1;
    activePlayer = -1;
    kicks = 0;
    if (amxSimpleNewWorld__FP5__amxPi(nullptr, params) != 0 || kicks != 1) return 4;

    activePlayer = 0;
    player[23970] = 0;
    player[23971] = 0;
    kicks = 0;
    if (amxSimpleNewWorld__FP5__amxPi(nullptr, params) != 0 || kicks != 1) return 5;
    if (half_at(0) != 900) return 6;

    player[23970] = 3;
    player[23971] = 0;
    kicks = 0;
    shown = nullptr;
    local = 1;
    if (amxSimpleNewWorld__FP5__amxPi(nullptr, params) != 0 || kicks != 0) return 7;
    if (std::strcmp(looked, "noZone3") != 0 || shown != text_body) return 8;
    if (ticks != 180 || message_player != 7 || unused != 0) return 9;
    if (half_at(0) != 3) return 10;

    local = 0;
    shown = text_body;
    looked[0] = 0;
    if (amxSimpleNewWorld__FP5__amxPi(nullptr, params) != 0 || kicks != 0) return 11;
    if (shown != text_body || looked[0] != 0) return 12;
    std::printf("simple new world passed\n");
    return 0;
}
