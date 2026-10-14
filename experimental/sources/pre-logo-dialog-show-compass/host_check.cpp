// Host contract for hudShowCompass. Not an EE plan input.
#include "show_compass.cpp"

#include <cstring>

extern "C" signed char hudNoDraw = 0;
extern "C" signed char interfaceOn = 0;
extern "C" unsigned char camera_bytes[64] = {};
extern "C" unsigned char* gameCam = camera_bytes;
extern "C" signed char coreLoadingScreenRunning = 0;
extern "C" unsigned gameNoDraw = 0;
extern "C" signed char drawCompassOn = 0;
extern "C" unsigned short drawCompassTitle[8] = {};
extern "C" unsigned char drawCompassPoint[12] = {};

static int copies;
static signed char on_at_copy;

extern "C" void ustrcpy__FPUsT0(unsigned short* dest, unsigned short* src) {
    ++copies;
    on_at_copy = drawCompassOn;
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
    drawCompassOn = 0;
    copies = 0;
    std::memset(drawCompassPoint, 0, 12);
}

int main() {
    unsigned char point[12];
    unsigned short text[3] = {11, 22, 0};
    for (int i = 0; i < 12; ++i) point[i] = static_cast<unsigned char>(30 + i);
    clear_flags();
    hudNoDraw = 1;
    hudShowCompass__FG6Point3PUs(point, text);
    if (copies != 0 || drawCompassOn != 0 || drawCompassPoint[0] != 0) return 1;
    clear_flags();
    interfaceOn = static_cast<signed char>(0x80);
    hudShowCompass__FG6Point3PUs(point, text);
    if (copies != 0) return 2;
    clear_flags();
    camera_bytes[52] = 0;
    hudShowCompass__FG6Point3PUs(point, text);
    if (copies != 0) return 3;
    clear_flags();
    camera_bytes[52] = 2;
    hudShowCompass__FG6Point3PUs(point, text);
    if (copies != 0) return 4;
    clear_flags();
    coreLoadingScreenRunning = 1;
    hudShowCompass__FG6Point3PUs(point, text);
    if (copies != 0) return 5;
    clear_flags();
    gameNoDraw = 1;
    hudShowCompass__FG6Point3PUs(point, text);
    if (copies != 0) return 6;
    clear_flags();
    hudShowCompass__FG6Point3PUs(point, text);
    if (copies != 1 || on_at_copy != 5 || drawCompassOn != 5) return 7;
    if (drawCompassTitle[0] != 11 || drawCompassTitle[1] != 22 || drawCompassTitle[2] != 0) return 8;
    for (int i = 0; i < 12; ++i) if (drawCompassPoint[i] != point[i]) return 9;
    return 0;
}
