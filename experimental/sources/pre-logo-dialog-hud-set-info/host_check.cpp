// Host contract for hudSetInfoMessage. Not an EE plan input.
#include "hud_set_info.cpp"

#include <cstdio>

extern "C" signed char interfaceOn = 0;
extern "C" unsigned short infoMessage[32] = {};
extern "C" unsigned short infoMessageTimer = 9;
extern "C" signed char infoMessageAlways = 9;

static int local_calls;
static int local_player;
static int copies;
static unsigned short* copy_dest;
static unsigned short* copy_src;

extern "C" int netPlayerIsLocal__Fi(int player) {
    ++local_calls;
    local_player = player;
    return player == 2 ? 1 : 0;
}

extern "C" void ustrcpy__FPUsT0(unsigned short* dest, unsigned short* src) {
    ++copies;
    copy_dest = dest;
    copy_src = src;
    unsigned index = 0;
    while (src[index] != 0) {
        dest[index] = src[index];
        ++index;
    }
    dest[index] = 0;
}

int main() {
    unsigned short text[3] = {0x41, 0x42, 0};
    interfaceOn = 1;
    infoMessageTimer = 9;
    infoMessageAlways = 9;
    local_calls = 0;
    copies = 0;
    hudSetInfoMessage__FPUsiic(text, 180, 1, 0);
    if (local_calls != 1 || local_player != 1 || copies != 0) return 1;
    if (infoMessageTimer != 9 || infoMessageAlways != 9) return 2;

    hudSetInfoMessage__FPUsiic(text, 180, -1, 0);
    if (local_calls != 1 || copies != 0) return 3;

    interfaceOn = 0;
    hudSetInfoMessage__FPUsiic(text, 0x12345, -1, 0x180);
    if (copies != 1 || copy_dest != infoMessage || copy_src != text) return 4;
    if (infoMessage[0] != 0x41 || infoMessage[1] != 0x42 || infoMessage[2] != 0) return 5;
    if (infoMessageTimer != 0x2345 || infoMessageAlways != static_cast<signed char>(0x80)) return 6;

    interfaceOn = 1;
    copies = 0;
    hudSetInfoMessage__FPUsiic(text, 7, 2, 0);
    if (local_calls != 2 || local_player != 2 || copies != 0) return 7;
    hudSetInfoMessage__FPUsiic(text, 8, 2, 1);
    if (copies != 1 || infoMessageTimer != 8 || infoMessageAlways != 1) return 8;
    return 0;
}
