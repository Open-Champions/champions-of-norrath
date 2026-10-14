// Host check for drawBust__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Wno-deprecated-declarations -ffp-contract=off -fno-builtin host_check.cpp
// The expected strings were worked out by hand from the observed rules (alpha step 0.0844495, byte-wise
// colour = (unsigned)(alpha * 110 or 128), fixed positions, rounded bar width) and cross-checked against
// the original in a private paired run.
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

#include "draw_bust.cpp"

extern "C" {
signed char drawBustOn;
signed char hudBustCovered;
float bustAlpha;
int drawBustCur;
int drawBustMax;
int nmPlayers;
int frameNm;
int fontObject;
void* smallFont = &fontObject;
unsigned short drawBustTitle[4] = {66, 111, 0, 0};
char drawBustTexName[32] = "name.tex";
}

static unsigned char texFrame[4], texName[4], texMeter[4], texNumber[4], texBar[10][4], texOther[4];
static std::string trace;

static void put_half(unsigned char* p, int v) { p[0] = static_cast<unsigned char>(v); p[1] = static_cast<unsigned char>(v >> 8); }
static void set_size(unsigned char* tex, int width, int height) { put_half(tex, width); put_half(tex + 2, height); }

static std::string label(const void* tex) {
    if (tex == texFrame) return "frame";
    if (tex == texName) return "name";
    if (tex == texMeter) return "meter";
    if (tex == texNumber) return "number";
    for (int i = 0; i < 10; ++i) if (tex == texBar[i]) return "bar" + std::to_string(i);
    return "?";
}

static void add(const char* format, long a = 0, long b = 0, long c = 0, long d = 0) {
    char text[200];
    std::snprintf(text, sizeof text, format, a, b, c, d);
    trace += text;
}

extern "C" {
void fontStart__FP4FontiUii(void* font, int mode, unsigned color, int last) {
    add("S%ld,", font == &fontObject);
    add("%ld,%lx,%ld;", mode, color, last);
}
void fontPrintu__FiiPUsi(int x, int y, unsigned short* text, int limit) {
    add("U%ld,%ld,", x, y);
    add("%ld,%ld;", text == drawBustTitle, limit);
}
void fontColor__FUiii(unsigned color, int a, int b) { add("C%lx,", color); add("%ld,%ld;", a, b); }
int sprintf(char* buffer, const char* format, ...) {
    va_list arguments;
    va_start(arguments, format);
    int value = va_arg(arguments, int);
    va_end(arguments);
    trace += std::string("F") + format + ":" + std::to_string(value) + ";";
    std::string text = format;
    size_t at = text.find("%d");
    text.replace(at, 2, std::to_string(value));
    std::strcpy(buffer, text.c_str());
    return static_cast<int>(text.size());
}
void fontPrintCentered__FiiPci(int x, int y, char* text, int limit) {
    add("P%ld,%ld,", x, y);
    trace += text;
    add(",%ld;", limit);
}
void fontEnd__Fv() { trace += "E;"; }
unsigned char* lumpFindResource__FPcT0(const char* group, const char* name) {
    trace += std::string("L") + group + "/" + name + ";";
    std::string n = name;
    if (n == "frame.tex") return texFrame;
    if (n == "name.tex") return texName;
    if (n == "fieldmeter.tex") return texMeter;
    if (n == "fieldnumber.tex") return texNumber;
    if (n.compare(0, 10, "bar_yellow") == 0 && n.size() == 16) return texBar[n[11] - '0'];
    return texOther;
}
void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* texture, int x, int y, int z, unsigned color, int a, int b,
                                                   int c, int d, char e, char f) {
    trace += "Q" + label(texture) + ",";
    add("%ld,%ld,%ld,%lx,", x, y, z, color);
    add("%ld,%ld,%ld,%ld,", a, b, c, d);
    add("%ld,%ld;", e, f);
}
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color) {
    trace += "T" + label(texture) + ",";
    add("%ld,%ld,%ld,%lx;", x, y, z, color);
}
}

static void reset() {
    trace.clear();
    drawBustOn = 1;
    hudBustCovered = 0;
    bustAlpha = 0.5f;
    drawBustCur = 7;
    drawBustMax = -1;
    nmPlayers = 1;
    frameNm = 47;
    set_size(texFrame, 60, 40);
    set_size(texName, 31, 10);
    set_size(texMeter, 20, 20);
    set_size(texNumber, 20, 20);
    for (int i = 0; i < 10; ++i) set_size(texBar[i], 64, 8);
}

static int expect(int code, const char* want) {
    if (trace == want) return 0;
    std::printf("case %d\n got  %s\n want %s\n", code, trace.c_str(), want);
    return code;
}

int main() {
    int rc;
    reset(); drawBustOn = 0; drawBust__Fv();
    if ((rc = expect(1, ""))) return rc;
    if (bustAlpha != 0.5f) return 2;

    // single local player, drawBustMax -1: the counter is printed; alpha 0.5 + 0.0844495 -> 64 (x110), 74 (x128)
    reset(); drawBust__Fv();
    if ((rc = expect(3, "S1,6,40395f6b,-1;U291,47,1,1000000;C4a395f6b,-1,0;F%d:7;P310,25,7,10000;E;"
                        "Lbust.lmp/frame.tex;Lbust.lmp/name.tex;Lbust.lmp/fieldnumber.tex;"
                        "Tnumber,278,24,6,4a808080;Tname,252,50,6,4a808080;Tframe,237,23,6,4a808080;"))) return rc;
    if (bustAlpha != 0.5f + 0.08444952219724655f || hudBustCovered != 0) return 4;

    // covered, three players, meter on: alpha 0.5 - 0.0844495 -> 45 (x110), 53 (x128); bar picked by frameNm / 2 % 10
    reset(); hudBustCovered = 1; nmPlayers = 3; drawBustMax = 100; drawBustCur = 40;
    drawBust__Fv();
    if ((rc = expect(5, "S1,6,2d395f6b,-1;U377,42,1,1000000;E;"
                        "Lbust.lmp/frame.tex;Lbust.lmp/name.tex;Lbust.lmp/fieldmeter.tex;Fbar_yellow0%d.tex:3;Lbust.lmp/bar_yellow03.tex;"
                        "Qbar3,371,30,6,35808080,371,30,395,38,0,0;Tmeter,361,24,6,35808080;"
                        "Tname,334,49,6,35808080;Tframe,319,22,6,35808080;"))) return rc;
    if (bustAlpha != 0.5f - 0.08444952219724655f || hudBustCovered != 0) return 6;

    // more than three players move the panel down; frameNm 1000 picks bar 0
    reset(); nmPlayers = 4; drawBustMax = 100; drawBustCur = 100; frameNm = 1000; bustAlpha = 0.0f;
    drawBust__Fv();
    if ((rc = expect(7, "S1,6,9395f6b,-1;U295,95,1,1000000;E;"
                        "Lbust.lmp/frame.tex;Lbust.lmp/name.tex;Lbust.lmp/fieldmeter.tex;Fbar_yellow0%d.tex:0;Lbust.lmp/bar_yellow00.tex;"
                        "Qbar0,289,83,6,a808080,289,83,351,91,0,0;Tmeter,279,77,6,a808080;"
                        "Tname,252,102,6,a808080;Tframe,237,75,6,a808080;"))) return rc;

    if (bustAlpha != 0.08444952219724655f) return 16;
    // alpha limits: stepping over 1 stores 1, under 0 stores 0 (covered); a negative current count becomes 0
    reset(); bustAlpha = 0.99f; drawBustCur = -5; drawBust__Fv();
    if (bustAlpha != 1.0f || drawBustCur != 0) return 8;
    if (trace.find("P310,25,0,10000;") == std::string::npos) return 9;
    if (trace.find("S1,6,6e395f6b,-1;") == std::string::npos || trace.find("C80395f6b,-1,0;") == std::string::npos) return 10;
    reset(); bustAlpha = 0.05f; hudBustCovered = 1; drawBustCur = -1; drawBust__Fv();
    if (bustAlpha != 0.0f || drawBustCur != 0) return 11;
    reset(); bustAlpha = 0.05f; hudBustCovered = 1; drawBust__Fv();
    if (trace.find("S1,6,395f6b,-1;") == std::string::npos) return 12;

    // fill width: nearest integer, halves away from zero; negative widths round the other way
    reset(); drawBustMax = 4; drawBustCur = 1; set_size(texBar[3], 10, 8); drawBust__Fv();    // 2.5 -> 3
    if (trace.find("Qbar3,289,31,6,4a808080,289,31,290,39,0,0;") == std::string::npos) { std::printf("%s\n", trace.c_str()); return 13; }
    reset(); drawBustMax = 4; drawBustCur = 1; set_size(texBar[3], -10, 8); drawBust__Fv();   // -2.5 -> -3
    if (trace.find("Qbar3,289,31,6,4a808080,289,31,284,39,0,0;") == std::string::npos) { std::printf("%s\n", trace.c_str()); return 14; }
    // frame and name widths are signed halfwords, halved toward zero
    reset(); set_size(texFrame, -61, 40); set_size(texName, -31, 10); drawBust__Fv();
    if (trace.find("Tname,282,50,6,4a808080;Tframe,297,23,6,4a808080;") == std::string::npos) return 15;
    std::puts("draw bust passed");
    return 0;
}
