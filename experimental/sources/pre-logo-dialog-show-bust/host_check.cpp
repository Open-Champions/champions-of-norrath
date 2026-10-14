// Host contract for hudShowBust. Not an EE plan input.
#include "show_bust.cpp"

#include <cstring>

extern "C" signed char hudNoDraw = 0;
extern "C" signed char interfaceOn = 0;
extern "C" unsigned char camera_bytes[64] = {};
extern "C" unsigned char* gameCam = camera_bytes;
extern "C" signed char coreLoadingScreenRunning = 0;
extern "C" unsigned gameNoDraw = 0;
extern "C" signed char drawBustOn = 0;
extern "C" char drawBustTexName[32] = {};
extern "C" unsigned short drawBustTitle[8] = {};
extern "C" int drawBustCur = 0;
extern "C" int drawBustMax = 0;

static int copies;
static signed char on_at_strcpy;

extern "C" char* strcpy(char* dest, const char* src) {
    on_at_strcpy = drawBustOn;
    char* out = dest;
    while ((*out++ = *src++) != 0) {
    }
    return dest;
}

extern "C" void ustrcpy__FPUsT0(unsigned short* dest, unsigned short* src) {
    ++copies;
    unsigned index = 0;
    while (src[index] != 0) {
        dest[index] = src[index];
        ++index;
    }
    dest[index] = 0;
}

static void clear_flags() {
    hudNoDraw = 0;
    interfaceOn = 0;
    coreLoadingScreenRunning = 0;
    gameNoDraw = 0;
    camera_bytes[52] = 1;
    camera_bytes[53] = 0;
    camera_bytes[54] = 0;
    camera_bytes[55] = 0;
    drawBustOn = 0;
    copies = 0;
    drawBustCur = 0;
    drawBustMax = 0;
    drawBustTexName[0] = 0;
}

int main() {
    char name[] = "guard";
    unsigned short text[2] = {9, 0};
    clear_flags();
    hudNoDraw = 1;
    hudShowBust__FPcPUsii(name, text, 3, 4);
    if (copies != 0 || drawBustOn != 0 || drawBustCur != 0) return 1;
    clear_flags();
    gameNoDraw = 0x100;
    hudShowBust__FPcPUsii(name, text, 3, 4);
    if (copies != 0) return 2;
    clear_flags();
    hudShowBust__FPcPUsii(name, text, -5, 12);
    if (on_at_strcpy != 5 || drawBustOn != 5 || copies != 1) return 3;
    if (std::strcmp(drawBustTexName, "guard") != 0) return 4;
    if (drawBustTitle[0] != 9 || drawBustTitle[1] != 0) return 5;
    if (drawBustCur != -5 || drawBustMax != 12) return 6;
    return 0;
}
