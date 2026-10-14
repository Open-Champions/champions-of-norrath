// Host check for drawNamePad__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "draw_name_pad.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
int curPlayer, nameGridPos, nameGridW, nameGridCount, throbR, throbG, throbB, throbA, frameNm;
float flareAlpha = 1.0f;
float otherHermiteTime, nameGridSrcX, nameGridSrcY, nameGridSrcW, nameGridDstX, nameGridDstY, nameGridDstW;
signed char language;
unsigned mediumFont = 0x11, smallFont = 0x22;
unsigned char fontGrid[40];
unsigned char playerSelectInfo[41 * 4];
}
static std::vector<std::string> events;
static std::string str(int v) { return std::to_string(v); }
extern "C" {
void fontStart__FP4FontiUii(void* f, int a, unsigned color, int b) { events.push_back("start:" + str(static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(f))) + "," + str(a) + "," + std::to_string(color) + "," + str(b)); }
void fontEnd__Fv() { events.push_back("end"); }
void fontColor__FUiii(unsigned color, int a, int b) { events.push_back("color:" + std::to_string(color) + "," + str(a) + "," + str(b)); }
void fontPrintCenteredu__FiiPUsi(int x, int y, UString* t, int w) { events.push_back("glyph:" + str(x) + "," + str(y) + ":" + str(t[0]) + ":" + str(w)); }
void fontPrintu__FiiPUsi(int x, int y, UString* t, int w) { events.push_back("line:" + str(x) + "," + str(y) + ":" + str(t[0]) + ":" + str(w)); }
int fontPrintWrapu__FiiPUsiiii(int x, int y, UString*, int a, int b, int c, int d) { events.push_back("wrap:" + str(x) + "," + str(y) + "," + str(a) + "," + str(b) + "," + str(c) + "," + str(d)); return 0; }
void fontPrint__FiiPci(int x, int y, char* t, int w) { events.push_back("name:" + str(x) + "," + str(y) + ":" + t + ":" + str(w)); }
int fontStringSize__FP4FontPci(void*, char* t, int) { return static_cast<int>(std::strlen(t)) * 20; }
void fontScissor__Fiiii(int a, int b, int c, int d) { events.push_back("scissor:" + str(a) + "," + str(b) + "," + str(c) + "," + str(d)); }
UString* textGetString__FPc(const char* k) { events.push_back(std::string("key:") + k); static UString t[2] = {0x41, 0}; return t; }
void usprintf__FPUsT0ffffffff(UString* d, UString*, float a, float, float, float, float, float, float, float) { events.push_back("icon:" + str(static_cast<int>(a))); d[0] = 0x49; d[1] = 0; }
int gameButtonToIcon__Fi(int b) { return b * 2; }
float evalHermite__Ffffff(float t, float, float, float, float) { return t; }
void rectangleDraw__FiiiiiUi(int a, int b, int c, int d, int e, unsigned color) { events.push_back("rect:" + str(a) + "," + str(b) + "," + str(c) + "," + str(d) + "," + str(e) + "," + std::to_string(color)); }
}
// the source binds the image's float, strcpy and strcat through labels; the host supplies the same names
extern "C" char* copyText(char* d, const char* s) { return std::strcpy(d, s); }
extern "C" char* appendText(char* d, const char* s) { return std::strcat(d, s); }

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    nameGridCount = 12; nameGridPos = 4; curPlayer = 1; language = 0; frameNm = 0x20;
    throbR = 1; throbG = 2; throbB = 3; throbA = 4;
    otherHermiteTime = 0.5f;
    nameGridSrcX = 100.0f; nameGridDstX = 200.0f; nameGridSrcY = 50.0f; nameGridDstY = 70.0f; nameGridSrcW = 20.0f; nameGridDstW = 60.0f;
    for (int i = 0; i < 40; ++i) fontGrid[i] = static_cast<unsigned char>('a' + i);
    std::strcpy(reinterpret_cast<char*>(playerSelectInfo + 41), "Bob");
    drawNamePad__Fv();
    CHECK(nameGridW == 10);
    const std::string normal = std::to_string((79u << 24) | 0x585e5du);
    const std::string throb = std::to_string(1u | (4u << 24) | (3u << 16) | (2u << 8));
    // fontStart for the grid, then twelve glyphs; cell 4 is wrapped in the throb colour
    CHECK(events[0] == "start:17,6," + normal + ",-1");
    CHECK(events[1] == "glyph:61,109:97:1000000");
    CHECK(events[5] == "color:" + throb + ",-1,0" && events[6] == "glyph:141,109:101:1000000" && events[7] == "color:" + normal + ",-1,0");
    bool row2 = false, space = false, done = false, line1 = false, line3 = false, rect = false, wrap = false, name = false;
    int fontEnds = 0;
    for (const std::string& e : events) {
        if (e == "glyph:61,129:107:1000000") row2 = true;
        if (e == "key:selectSetNameSpace") space = true;
        if (e == "key:selectSetNameDone") done = true;
        if (e == "line:81,169:73:1000000") line1 = true;
        if (e == "line:81,209:73:1000000") line3 = true;
        if (e == "rect:130,51,170,71,6," + std::to_string((96u << 24) | 0x1e3053u)) rect = true;
        if (e == "wrap:41,49,219,-100,580,17") wrap = true;
        if (e == "name:50,83:Bob_:100000") name = true;
        if (e == "end") ++fontEnds;
    }
    CHECK(row2 && space && done && line1 && line3 && rect && wrap && name && fontEnds == 3);
    bool buttons = false;
    for (const std::string& e : events) if (e == "glyph:163,129:65:1000000" || e == "glyph:233,129:65:1000000") buttons = true;
    CHECK(buttons);
    // language shifts the buttons, the pane and the name origin; no underscore when the blink bit is clear
    events.clear(); language = 1; frameNm = 0; nameGridPos = -1;
    drawNamePad__Fv();
    bool shiftedSpace = false, shiftedDone = false, shiftedWrap = false, shiftedName = false, doneThrob = false;
    for (size_t i = 0; i < events.size(); ++i) {
        if (events[i] == "glyph:192,129:65:1000000") shiftedSpace = true;
        if (events[i] == "glyph:262,129:65:1000000") { shiftedDone = true; if (events[i - 2] == "color:" + throb + ",-1,0") doneThrob = true; }
        if (events[i] == "wrap:41,49,289,-100,580,17") shiftedWrap = true;
        if (events[i] == "name:45,83:Bob:100000") shiftedName = true;
    }
    CHECK(shiftedSpace && shiftedDone && shiftedWrap && shiftedName && doneThrob);
    std::puts("ok");
    return 0;
}
