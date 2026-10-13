// Host check for shopDrawInfoBox__FiP12screenStructPQ24item11descriptioniiiii. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -ffp-contract=off -fno-builtin host_check.cpp
// The image's 32-bit pointer words are offsets into one host arena. Callees append to a trace; the
// expected traces are written out by hand from the rules in the source header, and the thousands
// separator cases (including the odd nine-digit result the original code gives) are the outputs
// observed from the original function.
#define NORRATH_HOST_ARENA
#include "shop_draw_info_box.cpp"

#include <cstdio>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>

static unsigned char arena[1 << 16];
extern "C" {
unsigned char* norrath_host_base = arena;
int itemInfoHeight[4];
int smallFontObject;
void* smallFont = &smallFontObject;
void* tinyIcon[9];
}

static std::string trace;
static std::vector<unsigned short> nameText = {'A', 'b', 'c', 0};
static unsigned short nameStorage[64];
static int valueAnswer, wrapAnswer, widthPerChar;
static float hermiteAnswer;
static std::string utf8(const unsigned short* t) { std::string s; for (; *t; ++t) s += static_cast<char>(*t); return s; }
static std::string num(long v) { return std::to_string(v); }
static std::string hex(unsigned v) { char b[16]; std::snprintf(b, sizeof b, "%x", v); return b; }

extern "C" float evalHermite__Ffffff(float t, float a, float b, float c, float d) {
    trace += "herm(" + num(static_cast<long>(t * 1000 + (t < 0 ? -0.5 : 0.5))) + "," + num(static_cast<long>(a)) + "," + num(static_cast<long>(b)) + "," + num(static_cast<long>(c)) + "," + num(static_cast<long>(d)) + ");";
    return hermiteAnswer;
}
extern "C" unsigned short* getName__4itemPQ24item11descriptioni(void* cell, int flag) {
    trace += "name(" + num(cell != nullptr) + "," + num(flag) + ");";
    std::memset(nameStorage, 0, sizeof nameStorage);
    for (size_t i = 0; i < nameText.size(); ++i) nameStorage[i] = nameText[i];
    return nameStorage;
}
extern "C" void ustrcpy__FPUsT0(unsigned short* to, const unsigned short* from) { trace += "ucpy;"; while ((*to++ = *from++) != 0) {} }
extern "C" int ustrlen__FPUs(const unsigned short* t) { int n = 0; while (t[n]) ++n; trace += "ulen" + num(n) + ";"; return n; }
extern "C" int fontStringSizeu__FP4FontPUsi(void* font, unsigned short* t, int limit) {
    int n = 0; while (t[n]) ++n;
    trace += std::string("size") + (font == smallFont ? "S" : "?") + num(n) + "," + num(limit) + ";";
    return n * widthPerChar;
}
extern "C" void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags) {
    trace += std::string("start") + (font == smallFont ? "S" : "?") + num(size) + "," + hex(color) + "," + num(flags) + ";";
}
extern "C" void fontColor__FUiii(unsigned color, int a, int b) { trace += "color" + hex(color) + "," + num(a) + "," + num(b) + ";"; }
extern "C" int fontPrintWrapCenteredu__FiiPUsiiii(int x, int y, unsigned short* t, int a, int b, int c, int d) {
    trace += "wrap(" + num(x) + "," + num(y) + "," + utf8(t) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");";
    return wrapAnswer;
}
extern "C" void fontEnd__Fv() { trace += "end;"; }
extern "C" void fontScissor__Fiiii(int a, int b, int c, int d) { trace += "scissor(" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + ");"; }
extern "C" int getValue__4itemPQ24item11description(void*) { trace += "value;"; return valueAnswer; }
extern "C" void fontPrint__FiiPci(int x, int y, char* t, int limit) { trace += "print(" + num(x) + "," + num(y) + "," + t + "," + num(limit) + ");"; }
extern "C" void fontPrintCentered__FiiPci(int x, int y, char* t, int limit) { trace += "center(" + num(x) + "," + num(y) + "," + t + "," + num(limit) + ");"; }
extern "C" void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* tex, int x, int y, int mode, unsigned color, int a, int b, int c, int d, char e, char f) {
    trace += std::string("tex") + (tex == tinyIcon[7] ? "7" : "?") + "(" + num(x) + "," + num(y) + "," + num(mode) + "," + hex(color) + "," + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num(f) + ");";
}
extern "C" void displayItemStats__FiiiPQ24item11descriptioniii(int player, int x, int y, void* cell, int a, int b, int c) {
    trace += "stats(" + num(player) + "," + num(x) + "," + num(y) + "," + num(cell != nullptr) + "," + num(a) + "," + num(b) + "," + num(c) + ");";
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

struct Fixture {
    alignas(8) unsigned char screen[96];
    alignas(8) unsigned char cell[48];
    alignas(8) unsigned char description[48];
    char descriptionName[32];
};
template <class T> static void put(unsigned char* base, int off, T v) { std::memcpy(base + off, &v, sizeof v); }
static unsigned wordOf(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

static void reset(Fixture& f, const char* itemName, int kind, int byte29, unsigned long long flags, int count) {
    std::memset(&f, 0, sizeof f);
    f.cell[0] = 0;
    std::strcpy(f.descriptionName, itemName);
    // the fixture lives inside the arena so its address words can be offsets
    put<unsigned>(f.cell, 0, wordOf(f.description));
    put<unsigned>(f.description, 0, wordOf(f.descriptionName));
    f.description[28] = static_cast<unsigned char>(kind);
    f.description[29] = static_cast<unsigned char>(byte29);
    put<unsigned long long>(f.cell, 8, flags);
    put<short>(f.cell, 4, static_cast<short>(count));
    f.cell[33] = 255;
    put<int>(f.screen, 0, 10);
    put<int>(f.screen, 44, 0);
    put<float>(f.screen, 64, 0.5f);
    trace.clear();
    valueAnswer = 100; wrapAnswer = 20; widthPerChar = 1; hermiteAnswer = 0.25f;
    nameText = {'A', 'b', 'c', 0};
    for (int i = 0; i < 4; ++i) itemInfoHeight[i] = 10 * i;
    tinyIcon[7] = reinterpret_cast<void*>(0x1234);
}

struct Expect {
    int pn = 2, mode = 0, shade = 0x40, tint = 0x80, width = 100;
    int x = 174, y = 205;           // after the slide: y = 229 - 24
    std::string name = "Abc";
    std::string colorCalls;         // fontColor calls, if any
    int lines = 1;
    std::string price = "100";
    int dy = 0;
    bool wide = true;               // byte 29 != 2 and flag 0x100: the first print/icon position
    int count = 1;
    std::string herm = "herm(409,0,1,0,0);";
    std::string measure;            // measurement loop text; the default is a single fitting pass
    std::string build() const {
        std::string s = herm;
        s += "name(1,1);ucpy;ulen" + num(static_cast<long>(name.size())) + ";";
        s += measure.empty() ? "ucpy;sizeS" + num(static_cast<long>(name.size())) + ",1000000;" : measure;
        s += "name(1,1);startS6," + hex(0x80000000u | shade * 0x010101u) + ",-1;" + colorCalls;
        s += "wrap(" + num(x) + "," + num(y) + "," + name + ",216,-100,580,20);end;";
        const int y2 = y + 22 * lines - 22;
        s += "startS6," + hex(0x80000000u | shade * 0x010101u) + ",-1;scissor(0," + num(y2) + ",640,407);value;";
        if (wide) s += "print(" + num(x - 64) + "," + num(y2 + dy + 64) + "," + price + ",100000);";
        else s += "print(" + num(x - 74) + "," + num(y2 + dy + 43) + "," + price + ",100000);";
        if (count >= 2) s += "center(" + num(x) + "," + num(y2 + 20) + ",x " + num(count) + ",10000);";
        s += "end;";
        if (wide) s += "tex7(" + num(x - 85) + "," + num(y2 + dy + 68) + ",6," + hex(0x80000000u | tint * 0x010101u) + ",0,92,639,447,0,0);";
        else s += "tex7(" + num(x - 95) + "," + num(y2 + dy + 47) + ",6," + hex(0x80000000u | tint * 0x010101u) + ",0,92,639,447,0,0);";
        s += "stats(" + num(pn) + "," + num(x) + "," + num(y2 + 23) + ",1," + num(tint) + "," + num(shade) + "," + num(lines) + ");";
        return s;
    }
};

static void call(Fixture& f, const Expect& e) {
    shopDrawInfoBox__FiP12screenStructPQ24item11descriptioniiiii(e.pn, f.screen, f.cell, e.mode, e.shade, e.tint, 7, e.width);
}

int main() {
    // The fixtures must sit inside the arena for the offset words; place them at its start.
    Fixture& f = *new (arena + 0x100) Fixture;
    Expect e;
    // 1. the plain case
    reset(f, "axe", 0, 0, 0x100, 1);
    call(f, e);
    CHECK(trace == e.build());
    CHECK(itemInfoHeight[2] == 21 && itemInfoHeight[1] == 10 && itemInfoHeight[3] == 30);
    {
        float slide; std::memcpy(&slide, f.screen + 64, 4);
        CHECK(slide > 0.409f && slide < 0.4091f);       // 0.5 - 1/11
    }
    // 2. narrow/wide width: the box moves up and one pixel left at 262 and above
    reset(f, "axe", 0, 0, 0x100, 1);
    Expect wideCase = e; wideCase.width = 262; wideCase.x = 173; wideCase.y = 78 - 24;
    call(f, wideCase);
    CHECK(trace == wideCase.build());
    reset(f, "axe", 0, 0, 0x100, 1);
    Expect narrowCase = e; narrowCase.width = 261; narrowCase.x = 174; narrowCase.y = 229 - 24;
    call(f, narrowCase);
    CHECK(trace == narrowCase.build());
    // the easing value scales: (1.0 + 0.01) * 96 = 96.96 -> 96, (0.0 + 0.01) * 96 -> 0
    for (float h : {1.0f, 0.0f}) {
        reset(f, "axe", 0, 0, 0x100, 1);
        hermiteAnswer = h;
        Expect c = e; c.y = 229 - (h == 1.0f ? 96 : 0);
        call(f, c);
        CHECK(trace == c.build());
    }
    // 3. a null description ends after the slide update and the hermite call
    reset(f, "axe", 0, 0, 0x100, 1);
    put<unsigned>(f.cell, 0, 0);
    call(f, e);
    CHECK(trace == "herm(409,0,1,0,0);" && itemInfoHeight[2] == 20);
    // 4. the slide: opening adds 1/11, closing subtracts it, the position is limited to 0 and 1
    reset(f, "axe", 0, 0, 0x100, 1);
    put<int>(f.screen, 44, 1);
    put<unsigned>(f.cell, 0, 0);
    call(f, e);
    CHECK(trace == "herm(591,0,1,0,0);");
    for (int variant = 0; variant < 4; ++variant) {
        reset(f, "axe", 0, 0, 0x100, 1);
        const float start[] = {0.05f, 0.95f, -2.0f, 3.0f};
        const int open[] = {0, 1, 0, 1};
        put<float>(f.screen, 64, start[variant]);
        put<int>(f.screen, 44, open[variant]);
        put<unsigned>(f.cell, 0, 0);
        call(f, e);
        float slide; std::memcpy(&slide, f.screen + 64, 4);
        const float want[] = {0.0f, 1.0f, 0.0f, 1.0f};
        CHECK(slide == want[variant]);
        CHECK(trace == std::string(variant == 0 || variant == 2 ? "herm(0,0,1,0,0);" : "herm(1000,0,1,0,0);"));
    }
    // 5. the name colour: byte 33 not 255 beats the flag; flag 0x2000000000 alone gives the other colour
    {
        reset(f, "axe", 0, 0, 0x100, 1);
        f.cell[33] = 7;
        Expect c = e; c.colorCalls = "color805c2913,-1,0;";
        call(f, c);
        CHECK(trace == c.build());
        reset(f, "axe", 0, 0, 0x100 | 0x2000000000ull, 1);
        c.colorCalls = "color80004444,-1,0;";
        call(f, c);
        CHECK(trace == c.build());
        reset(f, "axe", 0, 0, 0x100 | 0x2000000000ull, 1);
        f.cell[33] = 0;
        c.colorCalls = "color805c2913,-1,0;";
        call(f, c);
        CHECK(trace == c.build());
    }
    // 6. long names are measured again after each cut: ten characters at 30 each need four cuts
    {
        reset(f, "axe", 0, 0, 0x100, 1);
        nameText = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 0};
        widthPerChar = 30;
        Expect c = e; c.name = "ABCDEFGHIJ";
        // sizes of the measured copies: 10 chars, then "ABCDEFGHI..." (12), "ABCDEFGH..." (11), "ABCDEFG..." (10), "ABCDEF..." (9)
        c.measure = "ucpy;sizeS10,1000000;ucpy;sizeS12,1000000;ucpy;sizeS11,1000000;ucpy;sizeS10,1000000;ucpy;sizeS9,1000000;";
        call(f, c);
        CHECK(trace == c.build());
    }
    // 7. lines: the wrapped height divided by 20 grows the info height and moves the later parts
    for (int height : {0, 19, 20, 39, 40, 41, 100, -20}) {
        reset(f, "axe", 0, 0, 0x100, 1);
        wrapAnswer = height;
        Expect c = e; c.lines = height / 20;
        call(f, c);
        CHECK(trace == c.build());
        CHECK(itemInfoHeight[2] == 20 + height / 20);
    }
    // 8. price: shop mode divides by the stack count unless the kind is 8 or the name has "spear", then
    //    multiplies by about 0.2 (the float just below it); the kind 8 offset moves the price and icon up by 19
    {
        reset(f, "axe", 0, 0, 0x100, 5);
        Expect c = e; c.mode = 1; c.count = 5; c.price = "3";   // 100 / 5 * 0.19999999 = 3.9999998, truncated
        call(f, c);
        CHECK(trace == c.build());
        reset(f, "axe", 8, 0, 0x100, 5);
        c.price = "19"; c.dy = -19;   // 100 * 0.19999999 = 19.999998
        call(f, c);
        CHECK(trace == c.build());
        reset(f, "iron spear", 0, 0, 0x100, 5);
        c.dy = 0;
        call(f, c);
        CHECK(trace == c.build());
        reset(f, "axe", 0, 0, 0x100, 1);
        valueAnswer = 50;
        c.count = 1; c.price = "9";
        call(f, c);
        CHECK(trace == c.build());
        reset(f, "axe", 0, 0, 0x100, 1);
        c.mode = 0; c.price = "100"; c.count = 1;
        call(f, c);
        CHECK(trace == c.build());
        // a stack of 3 at 100 each: 100 / 3 * 0.2 truncates to 6
        reset(f, "axe", 0, 0, 0x100, 3);
        c.mode = 1; c.count = 3; c.price = "6";
        call(f, c);
        CHECK(trace == c.build());
    }
    // 9. byte 29 equal to 2, or no 0x100 flag: the second print and icon positions
    {
        reset(f, "axe", 0, 2, 0x100, 1);
        Expect c = e; c.wide = false;
        call(f, c);
        CHECK(trace == c.build());
        reset(f, "axe", 0, 0, 0, 1);
        call(f, c);
        CHECK(trace == c.build());
        reset(f, "axe", 0, 0, 0xffffffdffffffeffull, 1);
        call(f, c);
        CHECK(trace == c.build());
    }
    // 10. thousands separators (as printed by the original; the nine-digit result is its odd one)
    {
        struct Case { int value; const char* text; } cases[] = {
            {0, "0"}, {5, "5"}, {12, "12"}, {123, "123"}, {1234, "1,234"}, {12345, "12,345"}, {123456, "123,456"},
            {1234567, "1,234,567"}, {12345678, "12,345,678"}, {123456789, "1234,,56792"}, {1234567890, "1,234,567,936"},
            {-5, "-5"}, {-1234, "-1,234"}};
        for (const Case& c : cases) {
            reset(f, "axe", 0, 0, 0x100, 1);
            valueAnswer = c.value;
            Expect x = e; x.price = c.text;
            call(f, x);
            if (trace != x.build()) std::printf("value %d wanted %s\n", c.value, c.text);
            CHECK(trace == x.build());
        }
    }
    // 11. the stack count line only from 2 up; negative and zero counts print nothing
    for (int count : {-1, 0, 1, 2, 3, 99}) {
        reset(f, "axe", 0, 0, 0x100, count);
        Expect c = e; c.count = count;
        call(f, c);
        CHECK(trace == c.build());
    }
    std::puts("shop draw info box passed");
    return 0;
}
