// Host check for helpfulMessagesDraw__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -ffp-contract=off -fno-builtin host_check.cpp
#include "helpful_messages_draw.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
short infoMessageTimer;
signed char infoMessageAlways;
unsigned char hudNoDraw;
unsigned short infoMessage[8] = {72, 105, 0};
int fontA, fontB;
void* defaultFont = &fontA;
void* smallFont = &fontB;
signed char interfaceOn;
unsigned char cameraBytes[64];
unsigned char* gameCam = cameraBytes;
int bossMeterCurrent;
int bossMeterMax;
int bossMeterAlpha;
char bossMeterName[128] = "Boss";
unsigned char language;
}

static std::string trace;
static int stringWidth;
static unsigned short nameText[2] = {66, 0};

static void add(const char* format, long a = 0, long b = 0, long c = 0, long d = 0, long e = 0, long f = 0) {
    char text[200];
    std::snprintf(text, sizeof text, format, a, b, c, d, e, f);
    trace += text;
}

extern "C" {
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last) {
    add("S%d,", font == &fontA ? 1 : font == &fontB ? 2 : 9);
    add("%d,%x,%d;", mode, color, last);
}
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int limit) {
    add("P%d,%d,", x, y);
    add("%d,%d;", text == infoMessage ? 1 : text == nameText ? 2 : 9, limit);
}
void fontEnd__Fv() { trace += "E;"; }
int fontStringSizeu__FP4FontPUsi(void* font, unsigned short* text, int count) {
    add("W%d,", font == &fontA);
    add("%d,%d;", text == infoMessage, count);
    return stringWidth;
}
void textureSmoothBorder__FiiiiiUiiii(int x, int y, int w, int h, int mode, unsigned color, int a, int b, int c) {
    add("B%d,%d,%d,%d,", x, y, w, h);
    add("%d,%x,", mode, color);
    add("%d,%d,%d;", a, b, c);
}
unsigned short* textGetString__FPc(char* key) { add("K%s;", reinterpret_cast<long>(key)); return nameText; }
void rectangleDraw__FiiiiiUi(int x1, int y1, int x2, int y2, int z, unsigned color) {
    add("R%d,%d,%d,%d,", x1, y1, x2, y2);
    add("%d,%x;", z, color);
}
int mapCount;
void __11ResourceMapPcT1(void* map, char* group, char* name) {
    add("M%s,", reinterpret_cast<long>(group));
    add("%s,", reinterpret_cast<long>(name));
    add("%d;", map == bossMeterTex ? 1 : map == bossMeterTexPal ? 2 : 9);
    ++mapCount;
}
void* get__11ResourceMap(void* map) { return map; }
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color) {
    add("T%d,", texture == bossMeterTex ? 1 : texture == bossMeterTexPal ? 2 : 9);
    add("%d,%d,%d,%x;", x, y, z, color);
}
}

static void reset() {
    trace.clear();
    infoMessageTimer = 0;
    infoMessageAlways = 0;
    hudNoDraw = 0;
    interfaceOn = 0;
    std::memset(cameraBytes, 0, sizeof cameraBytes);
    cameraBytes[52] = 1;
    bossMeterCurrent = bossMeterMax = bossMeterAlpha = 0;
    language = 0;
    stringWidth = 250;
    messageAlpha = 0;
}

static int expect(int code, const char* want) {
    if (trace == want) return 0;
    std::printf("case %d\n got  %s\n want %s\n", code, trace.c_str(), want);
    return code;
}

int main() {
    int rc;
    // timer below 32: alpha = timer * 4; narrow text is raised to 300; no boss meter, flag stays clear.
    reset(); infoMessageTimer = 10; bossInitFlag = 1;
    helpfulMessagesDraw__Fv();
    if ((rc = expect(1, "S1,6,281e6464,-1;P320,78,1,1000000;E;W1,1,1000000;B150,76,340,32,6,14000000,0,0,0;"))) return rc;
    if (messageAlpha != 40 || bossInitFlag != 0) return 2;
    // timer >= 32 adds 3, capped at 128; always-on text draws even with hudNoDraw, which then stops the boss part.
    reset(); infoMessageTimer = 40; infoMessageAlways = 1; hudNoDraw = 1; messageAlpha = 126; stringWidth = 500;
    helpfulMessagesDraw__Fv();
    if ((rc = expect(3, "S1,7,801e6464,-1;P320,78,1,1000000;E;W1,1,1000000;B50,76,540,32,7,40000000,0,0,0;"))) return rc;
    if (messageAlpha != 128) return 4;
    reset(); infoMessageTimer = 1; hudNoDraw = 1;
    helpfulMessagesDraw__Fv();
    if ((rc = expect(5, ""))) return rc;
    // gate conditions for the boss part
    reset(); bossMeterCurrent = 5; bossMeterMax = 5; interfaceOn = 1; helpfulMessagesDraw__Fv();
    if ((rc = expect(6, ""))) return rc;
    reset(); bossMeterCurrent = 5; bossMeterMax = 5; cameraBytes[52] = 0; helpfulMessagesDraw__Fv();
    if ((rc = expect(7, ""))) return rc;
    // first boss frame: max follows current, alpha becomes 78, maps are built once, flag and last health set.
    reset(); bossMeterCurrent = 50; bossMeterMax = 0; bossMeterAlpha = 0; bossInitFlag = 0; lastBossHealth = 0;
    bossTexGuard = bossPalGuard = 0; mapCount = 0;
    helpfulMessagesDraw__Fv();
    if ((rc = expect(8, "S2,5,464646,-1;KBoss;P320,408,2,1000000;E;R218,414,421,429,5,4e181850;"
                        "Mhud.lmp,bossmeter.tex,1;Mhud.lmp,bossmeter_pal.tex,2;T1,209,409,6,80808080;"))) return rc;
    if (bossMeterMax != 50 || bossMeterAlpha != 78 || bossInitFlag != 1 || lastBossHealth != 50 || mapCount != 2) return 9;
    // second frame: health dropped, so the fill flashes; the maps are not rebuilt; the width truncates.
    reset(); bossMeterCurrent = 25; bossMeterMax = 50; bossMeterAlpha = 78;
    helpfulMessagesDraw__Fv();
    if ((rc = expect(10, "S2,5,4e464646,-1;KBoss;P320,408,2,1000000;E;R218,414,319,429,5,4e808080;T1,209,409,6,80808080;"))) return rc;
    if (lastBossHealth != 25 || mapCount != 2) return 11;
    // health rose: normal colour. Language 4 uses the wide bar and the second texture.
    reset(); bossMeterCurrent = 40; bossMeterMax = 50; bossMeterAlpha = 78; language = 4;
    helpfulMessagesDraw__Fv();
    if ((rc = expect(12, "S2,5,4e464646,-1;KBoss;P320,408,2,1000000;E;R162,414,414,429,5,4e181850;T2,158,409,6,80808080;"))) return rc;
    // no health left: the alpha fades by 3, current is cleared and the bar has zero width.
    reset(); bossMeterCurrent = -2; bossMeterMax = 50; bossMeterAlpha = 10; bossInitFlag = 1; lastBossHealth = -1;
    helpfulMessagesDraw__Fv();
    if ((rc = expect(13, "S2,5,7464646,-1;KBoss;P320,408,2,1000000;E;R218,414,218,429,5,7181850;T1,209,409,6,80808080;"))) return rc;
    if (bossMeterCurrent != 0 || bossMeterAlpha != 7 || lastBossHealth != 0) return 14;
    // no health and no alpha: the flag clears and nothing is drawn.
    reset(); bossMeterCurrent = 0; bossMeterMax = 50; bossMeterAlpha = 0; bossInitFlag = 1;
    helpfulMessagesDraw__Fv();
    if ((rc = expect(15, ""))) return rc;
    if (bossInitFlag != 0) return 16;
    std::puts("helpful messages draw passed");
    return 0;
}
