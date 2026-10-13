// Host check for drawGold__Fv (source: ../w12-hud-loop-a-draw-gold). Not an EE plan input.
// This is the w12 check with its last case corrected: the print follows the colour call (the
// format call comes before both), so the old search for a colour followed by the format call could
// never match and the check exited 10 on every host.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -Wno-deprecated-declarations -ffp-contract=off -fno-builtin host_check.cpp
// The expected velocity words in the first case were taken once from a private run of the original
// function with the same record, goal and frame counter; they are pinned here as literals.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

#include "../w12-hud-loop-a-draw-gold/draw_gold.cpp"

extern "C" {
int gameNoDraw;
unsigned char hudNoDraw;
int fontObject;
void* defaultFont = &fontObject;
int frameNm;
alignas(8) unsigned char goldNumber[8 * 36];
int goldAlpha[4];
int playerGettinIt[4];
int playerGoldx[4] = {50, 590, 50, 590};
int playerGoldy[4] = {200, 200, 250, 250};
int playerGoldIconx[4] = {16, 593, 16, 593};
alignas(8) unsigned char player[23984 * 4];
}

static std::string trace;
static unsigned localMask;
static int stringWidth;

static void add(const char* format, long a = 0, long b = 0, long c = 0, long d = 0) {
    char text[160];
    std::snprintf(text, sizeof text, format, a, b, c, d);
    trace += text;
}

extern "C" {
int netPlayerIsLocal__Fi(int index) {
    add("L%ld;", index);
    return index >= 0 && index < 32 && ((localMask >> index) & 1u) ? 1 : 0;
}
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last) {
    add("S%ld,", font == &fontObject);
    add("%ld,%lx,%ld;", mode, color, last);
}
int sprintf(char* buffer, const char* format, ...) {
    va_list arguments;
    va_start(arguments, format);
    int value = va_arg(arguments, int);
    va_end(arguments);
    trace += std::string("F") + format + ";";
    return std::snprintf(buffer, 64, "%d", value);
}
void fontColor__FUiii(unsigned color, int a, int b) { add("C%lx,", color); add("%ld,%ld;", a, b); }
void fontPrint__FiiPci(int x, int y, char* text, int limit) {
    add("P%ld,%ld,", x, y);
    trace += text;
    add(",%ld;", limit);
}
void commaString__FPc(char* text) { trace += "K"; trace += text; trace += ";"; std::strcat(text, "!"); }
int fontStringSize__FP4FontPci(void* font, char* text, int count) {
    add("W%ld,", font == &fontObject);
    trace += text;
    add(",%ld;", count);
    return stringWidth;
}
void fontEnd__Fv() { trace += "E;"; }
void* lumpFindResource__FPcT0(const char* group, const char* name) {
    trace += std::string("G") + group + "," + name + ";";
    return &fontObject;
}
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color) {
    add("T%ld,", texture == &fontObject);
    add("%ld,%ld,%ld,%lx;", x, y, z, color);
}
}

static void put_u32(unsigned char* p, unsigned v) { std::memcpy(p, &v, 4); }
static void put_f32(unsigned char* p, float v) { std::memcpy(p, &v, 4); }
static unsigned get_u32(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }

static void reset() {
    trace.clear();
    std::memset(goldNumber, 0, sizeof goldNumber);
    std::memset(player, 0, sizeof player);
    for (int i = 0; i < 4; ++i) goldAlpha[i] = playerGettinIt[i] = 0;
    gameNoDraw = 0;
    hudNoDraw = 0;
    localMask = 0;
    frameNm = 200;
    stringWidth = 40;
}

static int expect(int code, const char* want) {
    if (trace == want) return 0;
    std::printf("case %d\n got  %s\n want %s\n", code, trace.c_str(), want);
    return code;
}

int main() {
    int rc;
    reset(); gameNoDraw = 1; drawGold__Fv();
    if ((rc = expect(1, ""))) return rc;
    reset(); hudNoDraw = 0xff; drawGold__Fv();
    if ((rc = expect(2, ""))) return rc;
    // nothing local: only the setup, the record and player tests and the font end.
    reset(); drawGold__Fv();
    if ((rc = expect(3, "S1,6,804080e4,-1;L0;L0;L0;L0;L0;L0;L0;L0;L0;L1;L2;L3;E;"))) return rc;

    // one flying amount for player 1 (local), plus its counter, which grows in alpha
    reset(); localMask = 1u << 1;
    unsigned char* record = goldNumber;
    put_f32(record + 0, 100.0f); put_f32(record + 4, 200.0f); put_f32(record + 8, 50.0f);
    put_f32(record + 12, 2.0f); put_f32(record + 16, -3.0f); put_f32(record + 20, 1.0f);
    put_u32(record + 24, 190); put_u32(record + 28, 1234); record[32] = 1;
    goldAlpha[1] = 100; put_u32(player + 23984 + 232, 9999);
    drawGold__Fv();
    if ((rc = expect(4, "S1,6,804080e4,-1;L1;F%d;C624080e4,-1,0;P102,197,1234,100000;"
                        "L0;L0;L0;L0;L0;L0;L0;L0;L1;C684080e4,-1,0;F%d;K9999;W1,9999!,100000;P550,200,9999!,100000;"
                        "L2;L3;E;Ghud.lmp,gold03.tex;T1,593,190,6,68808080;"))) return rc;
    if (get_u32(record + 0) != 0x42cc0000u || get_u32(record + 4) != 0x43450000u || get_u32(record + 8) != 0x424c0000u) return 5;
    if (get_u32(record + 12) != 0x409511cbu || get_u32(record + 16) != 0xc02e08b1u || get_u32(record + 20) != 0x3f1db64au) return 6;
    if (playerGettinIt[1] != 59 || goldAlpha[1] != 104) return 7;

    // counters without flying amounts: players 0, 2 print at the plain x, player 3 shifted by the width;
    // a counter with nothing arriving fades by 4 and one at zero is skipped.
    reset(); localMask = 0xf;
    goldAlpha[0] = 40; goldAlpha[1] = 0; goldAlpha[3] = 128; playerGettinIt[3] = 2; goldAlpha[2] = 7;
    put_u32(player + 232, 7); put_u32(player + 23984 * 2 + 232, 123456); put_u32(player + 23984 * 3 + 232, 5);
    drawGold__Fv();
    if ((rc = expect(8, "S1,6,804080e4,-1;L0;L0;L0;L0;L0;L0;L0;L0;L0;C244080e4,-1,0;F%d;K7;P50,200,7!,100000;"
                        "L1;L2;C34080e4,-1,0;F%d;K123456;P50,250,123456!,100000;"
                        "L3;C804080e4,-1,0;F%d;K5;W1,5!,100000;P550,250,5!,100000;E;"
                        "Ghud.lmp,gold03.tex;T1,16,190,6,24808080;Ghud.lmp,gold03.tex;T1,16,240,6,3808080;"
                        "Ghud.lmp,gold03.tex;T1,593,240,6,80808080;"))) return rc;
    if (goldAlpha[0] != 36 || goldAlpha[1] != 0 || goldAlpha[2] != 3 || playerGettinIt[3] != 1) return 9;

    // the flying amount's alpha is limited to 0..128 and a zero amount is skipped
    reset(); localMask = 1;
    put_u32(goldNumber + 28, 5); goldNumber[32] = 0; put_u32(goldNumber + 24, 0);
    put_u32(goldNumber + 36 + 28, 6); goldNumber[36 + 32] = 0; put_u32(goldNumber + 36 + 24, 300);
    drawGold__Fv();
    // record 0 (stamp 0, frame 200) clamps to alpha 0, record 1 (stamp 300) clamps to 128; the colour is
    // set, then the amount printed, record 0 first
    if (trace.find("F%d;C4080e4,-1,0;P") == std::string::npos || trace.find("F%d;C804080e4,-1,0;P") == std::string::npos) return 10;
    if (trace.find("C4080e4") > trace.find("C804080e4")) return 11;
    std::puts("draw gold passed");
    return 0;
}
