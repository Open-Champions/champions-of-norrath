#define NORRATH_HOST_ARENA
#define strstr norrath_fake_strstr
#define strcmp norrath_fake_strcmp
#include "menu_draw.cpp"
#undef strstr
#undef strcmp

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
unsigned gstart = 0;
unsigned glistp = 0;
unsigned GLISTSIZE = 35000;
float norrath_menu_local_t_004844e8 = 1.0f;
int norrath_menu_word_00484514 = 116;
float coreInterfaceBrightness[4] = {1.0f, 0, 0, 0};
float commThresh = 0.25f;
int frontEndRunning = 0;
}

static void* slots[64];
extern "C" void* norrath_host_pointer(unsigned word) { return slots[word]; }

static std::vector<std::string> trace;
static std::string fmt(const char* f, ...) __attribute__((format(printf, 1, 2)));
static std::string fmt(const char* f, ...) {
    char buffer[256];
    va_list ap;
    va_start(ap, f);
    std::vsnprintf(buffer, sizeof buffer, f, ap);
    va_end(ap);
    return buffer;
}
static std::string narrow(const unsigned short* s) {
    std::string out;
    while (*s) out += static_cast<char>(*s++);
    return out;
}
static void widen(unsigned short* d, const std::string& s) {
    size_t i = 0;
    for (; i < s.size(); ++i) d[i] = static_cast<unsigned char>(s[i]);
    d[i] = 0;
}

static unsigned short strings[8][64];
static int string_count;
static float hermite_t = -1;
static int owner_seen = -99;
static int inventory = 7;

extern "C" {
float evalHermite__Ffffff(float t, float p0, float p1, float m0, float m1) {
    hermite_t = t;
    trace.push_back(fmt("hermite %g %g %g %g %g", t, p0, p1, m0, m1));
    return 0.5f;
}
unsigned colorScale__FUif(unsigned color, float scale) {
    trace.push_back(fmt("scale %08x %g", color, scale));
    return color ^ 0x1u;
}
unsigned lerpColor__FUiUif(unsigned from, unsigned to, float amount) {
    trace.push_back(fmt("lerp %08x %08x %g", from, to, amount));
    return from + 0x100u;
}
void fontStart__FP4FontiUii(void*, int mode, unsigned color, int last) {
    trace.push_back(fmt("fontStart %d %08x %d", mode, color, last));
}
void fontEnd__Fv() { trace.push_back("fontEnd"); }
void fontColor__FUiii(unsigned color, int a, int b) { trace.push_back(fmt("fontColor %08x %d %d", color, a, b)); }
int fontStringSizeu__FP4FontPUsi(void*, unsigned short* text, int scale) {
    trace.push_back(fmt("size %s %d", narrow(text).c_str(), scale));
    return static_cast<int>(narrow(text).size()) * 10;
}
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* text, int scale) {
    trace.push_back(fmt("print %d %d %s %d", x, y, narrow(text).c_str(), scale));
}
char* norrath_fake_strstr(const char* hay, const char* needle) {
    const char* at = std::strstr(hay, needle);
    return const_cast<char*>(at);
}
int norrath_fake_strcmp(const char* a, const char* b) { return std::strcmp(a, b); }
unsigned short* textGetString__FPc(char* key) {
    unsigned short* out = strings[string_count++ % 8];
    widen(out, std::string("T:") + key);
    trace.push_back(fmt("get %s", key));
    return out;
}
int queryInventoryItem__FPci(char* name, int owner) {
    owner_seen = owner;
    trace.push_back(fmt("inv %s %d", name, owner));
    return inventory;
}
void usprintf__FPUsT0ffffffff(unsigned short* dest, unsigned short* format, float a, float b, float c, float d,
                              float e, float f, float g, float h) {
    widen(dest, fmt("%s|%g|%g", narrow(format).c_str(), a, b + c + d + e + f + g + h));
}
void ustrcpy__FPUsT0(unsigned short* d, unsigned short* s) { widen(d, narrow(s)); }
void ustrcpy__FPUsPc(unsigned short* d, char* s) { widen(d, s); }
void ustrcat__FPUsPc(unsigned short* d, char* s) { widen(d, narrow(d) + s); }
void ustrcat__FPUsT0(unsigned short* d, unsigned short* s) { widen(d, narrow(d) + narrow(s)); }
void rectangleDraw__FiiiiiUi(int a, int b, int c, int d, int m, unsigned col) {
    trace.push_back(fmt("rect %d %d %d %d %d %08x", a, b, c, d, m, col));
}
void textureSmoothBorder__FiiiiiUiiii(int a, int b, int c, int d, int m, unsigned col, int e, int f, int g) {
    trace.push_back(fmt("smooth %d %d %d %d %d %08x %d %d %d", a, b, c, d, m, col, e, f, g));
}
void textureDrawBorder__FiiiiiUiPcii(int a, int b, int c, int d, int m, unsigned col, char* tex, int e, int f) {
    trace.push_back(fmt("border %d %d %d %d %d %08x %p %d %d", a, b, c, d, m, col, static_cast<void*>(tex), e, f));
}
void* lumpFindResource__FPcT0(char*, char*) { return nullptr; }
}

static int failures;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)
static bool has(const std::string& s) {
    for (const auto& line : trace) if (line == s) return true;
    return false;
}
static int index_of(const std::string& s, int from = 0) {
    for (size_t i = from; i < trace.size(); ++i) if (trace[i] == s) return static_cast<int>(i);
    return -1;
}
static int count_prefix(const char* p) {
    int n = 0;
    for (const auto& line : trace) if (line.compare(0, std::strlen(p), p) == 0) ++n;
    return n;
}

static void dump() { for (auto& l : trace) std::printf("  %s\n", l.c_str()); }
static void put(unsigned char* at, unsigned v) { at[0] = v; at[1] = v >> 8; at[2] = v >> 16; at[3] = v >> 24; }

struct Entry { const char* label; unsigned kind; void* value; unsigned flags; };
static unsigned char menu[40 * 16];
static char labels[40][32];
static void build(const std::vector<Entry>& entries, unsigned cursor, unsigned previous) {
    std::memset(menu, 0, sizeof menu);
    for (size_t i = 0; i < entries.size(); ++i) {
        std::strcpy(labels[i], entries[i].label);
        slots[1 + i] = labels[i];
        slots[41 + i % 20] = entries[i].value;
        put(menu + i * 16, 1 + i);
        put(menu + i * 16 + 4, entries[i].kind);
        put(menu + i * 16 + 8, entries[i].value ? 41 + i % 20 : 0);
        put(menu + i * 16 + 12, entries[i].flags);
    }
    put(menu + entries.size() * 16 + 4, cursor | (previous << 8));
    trace.clear();
    string_count = 0;
    norrath_menu_local_t_004844e8 = 1.0f;
    frontEndRunning = 0;
}

static int scenario;
static void draw(int x, int y, unsigned char* m, unsigned a, unsigned b, void* f, void* t, int o) {
    menuDraw__FiiP4MenuUiUiP4FontP8_texturei(x, y, m, a, b, f, t, o);
    if (std::getenv("DUMP")) { std::printf("== draw %d\n", ++scenario); dump(); }
}

int main() {
    // Early out when the display list is nearly full.
    int sink = 0;
    build({{"a", 1, &sink, 0}}, 0, 0);
    gstart = 0; glistp = 8 * (35000 - 2999); // (glistp-gstart)>>3 = 32001 -> room 2999
    draw(320, 240, menu, 0x11, 0x22, nullptr, nullptr, 0);
    CHECK(trace.empty());
    glistp = 8 * (35000 - 3000); // room exactly 3000 draws
    draw(320, 240, menu, 0x11, 0x22, nullptr, nullptr, 0);
    CHECK(!trace.empty());

    // Three entries, no shift: "menu_x" through textGetString, a narrow label, a kind 3 toggle.
    int toggle = 1;
    glistp = 0;
    build({{"menu_x", 0, nullptr, 0}, {"Plain", 1, &sink, 0}, {"Flag", 3, &toggle, 0}}, 1, 0);
    draw(320, 240, menu, 0x11, 0x22, nullptr, nullptr, 5);
    CHECK(trace[0] == "hermite 1 0 1 0.5 0.5");
    CHECK(trace[1] == "scale 00000011 1");
    CHECK(trace[2] == "fontStart 7 00000010 -1");
    // y - (height >> 1) = 240 - 36; row text y = 204 + 22 * row - 3.
    CHECK(has("print 320 201 T:menu_x 1000000"));
    CHECK(has("print 320 223 Plain 1000000"));
    CHECK(has("print 320 245 Flag : T:on 1000000"));
    toggle = 0;
    build({{"menu_x", 0, nullptr, 0}, {"Plain", 1, &sink, 0}, {"Flag", 3, &toggle, 0}}, 1, 0);
    draw(320, 240, menu, 0x11, 0x22, nullptr, nullptr, 5);
    CHECK(has("print 320 245 Flag : T:off 1000000"));
    CHECK(has("get off") && !has("get on"));
    // Cursor entry blends by t, previous entry by 1 - t, others stay idle.
    CHECK(has("lerp 00000011 00000022 1"));
    CHECK(has("lerp 00000011 00000022 0"));
    CHECK(count_prefix("lerp") == 2);
    // Font ends before the plate: smooth border width = cursor width 'Plain' + 40 = 90 -> rounded with eased 0.5.
    // widths: menu_x entry is 8 chars ("T:menu_x") * 10 + 40 = 120 (previous), Plain 50+40 = 90 (cursor).
    // eased width = 120 + (90 - 120) * 0.5 = 105; y = 201 + (223 - 201) * 0.5 = 212.
    CHECK(has("smooth 268 212 105 32 7 300000ff 0 0 0"));
    // The toggle entry "Flag : T:off" is the widest: 120 + 2 + 40 = 162 -> x 313 - 81, w 176.
    CHECK(has("border 232 194 176 92 7 80808080 0x0 0 0"));
    CHECK(index_of("fontEnd") < index_of("smooth 268 212 105 32 7 300000ff 0 0 0"));
    CHECK(trace.back().compare(0, 6, "border") == 0);

    // Front end running: no outer border.
    build({{"a", 1, &sink, 0}}, 0, 0);
    frontEndRunning = 1;
    draw(320, 240, menu, 0x11, 0x22, nullptr, nullptr, 0);
    CHECK(count_prefix("border") == 0);
    CHECK(count_prefix("smooth") == 1);

    // menu_recall formats the held count; a mismatched label near it does not.
    build({{"menu_recall", 0, nullptr, 0}, {"menu_recalls", 0, nullptr, 0}}, 0, 0);
    inventory = 3;
    draw(100, 240, menu, 0x11, 0x22, nullptr, nullptr, 9);
    CHECK(owner_seen == 9);
    CHECK(has("inv recallpotion 9"));
    CHECK(has("print 100 212 T:menu_recall|3|0 1000000"));
    CHECK(has("print 100 234 T:menu_recalls 1000000"));
    CHECK(count_prefix("inv") == 1);

    // Hidden entries take no row but still count in the index (cursor 2 is the third record).
    build({{"A", 1, &sink, 0}, {"B", 1, &sink, 1}, {"C", 1, &sink, 0}}, 2, 0);
    draw(320, 240, menu, 0x11, 0x22, nullptr, nullptr, 0);
    CHECK(has("print 320 212 A 1000000"));
    CHECK(has("print 320 234 C 1000000"));
    CHECK(count_prefix("print") == 2);
    CHECK(has("lerp 00000011 00000022 1")); // record index 2 is the cursor
    CHECK(has("lerp 00000011 00000022 0")); // record index 0 is the previous

    // Dim and fixed colors for kinds 5, 6, 7 and 8, with a reset after each.
    float slider = 0.5f;
    coreInterfaceBrightness[0] = 0.8f;
    build({{"S5", 5, nullptr, 0}, {"S6", 6, nullptr, 0}, {"S7", 7, nullptr, 0}, {"S8", 8, &slider, 0}}, 3, 3);
    draw(320, 240, menu, 0x11, 0x22, nullptr, nullptr, 0);
    CHECK(has("scale 00000011 0.4"));
    CHECK(has("scale 80202040 0.8"));
    CHECK(has("scale 80402020 0.8"));
    // Kinds 5..8 each reset to the idle base afterwards (4 resets) plus the fontStart scale.
    CHECK(count_prefix("scale 00000011 0.8") == 1 + 4);
    // Print order is before the reset for kind 6.
    // Height 94, so the S6 row prints at 240 - 47 + 22 - 3 = 212.
    int p6 = index_of("print 320 212 S6 1000000");
    CHECK(p6 > index_of("fontColor 80202041 -1 0"));
    CHECK(index_of("fontColor 00000010 -1 0", p6) == p6 + 2);
    CHECK(p6 >= 0);
    coreInterfaceBrightness[0] = 1.0f;

    // Kind 4 and 8 widen the entry and draw bars. Label "Vol" -> size 30, text x = 320 - 15 - 23.
    float level = 0.25f;
    build({{"Vol", 4, &level, 0}, {"Mic", 8, &level, 0}}, 0, 0);
    draw(320, 240, menu, 0x11, 0x22, nullptr, nullptr, 0);
    CHECK(has("print 282 212 Vol 1000000"));
    CHECK(has("print 282 234 Mic 1000000"));
    CHECK(count_prefix("size Vol") == 2);
    // widths: 30 + 156 + 40 = 226 for both; plate width stays 226, max width 226.
    CHECK(has("smooth 207 212 226 32 7 300000ff 0 0 0"));
    // Rows: base 240 - 25 = 215 (n = 2, height 50); row 0 top 221, bottom 230.
    // Bar length (320+120)-300-4 = 136; filled to 302 + 136 * 0.25 = 336.
    CHECK(has("rect 302 221 336 230 7 800c2b40"));
    CHECK(has("rect 302 221 438 230 7 80000000"));
    CHECK(has("rect 300 219 440 232 7 80808080"));
    // Kind 8 row (rowY 237): marker at 302 + 116 * 0.25 = 331 -> 330..332, then bar.
    CHECK(has("rect 330 243 332 252 7 80808080"));
    CHECK(has("rect 302 243 336 252 7 800c2b40"));
    CHECK(has("border 200 205 240 70 7 80808080 0x0 0 0"));

    // Many entries put the cursor row out of range: rows shift by 22 and t resets to 1.
    std::vector<Entry> many;
    static char names[16][8];
    for (int i = 0; i < 16; ++i) { std::snprintf(names[i], 8, "e%d", i); many.push_back({names[i], 1, &sink, 0}); }
    build(many, 15, 15);
    norrath_menu_local_t_004844e8 = 0.5f;
    draw(320, 240, menu, 0x11, 0x22, nullptr, nullptr, 0);
    // height 358; center = 330 - 179 + 237 = 388 < 401 so no shift: t is kept.
    CHECK(hermite_t == 0.5f);
    CHECK(norrath_menu_local_t_004844e8 == 0.5f);
    CHECK(has("print 320 58 e0 1000000"));
    build(many, 0, 0);
    norrath_menu_local_t_004844e8 = 0.5f;
    // center = 0 - 179 + 237 = 58: no shift.
    draw(320, 100, menu, 0x11, 0x22, nullptr, nullptr, 0);
    CHECK(norrath_menu_local_t_004844e8 == 0.5f);
    // Move the whole menu so the selected row is above 20: y = 100 -> center 58 - 140 ... use cursor 0, y = -50
    build(many, 15, 0);
    norrath_menu_local_t_004844e8 = 0.5f;
    draw(320, 0, menu, 0x11, 0x22, nullptr, nullptr, 0);
    // center = 388: >= 401 false -> still no shift. Raise via 18 entries below.
    CHECK(norrath_menu_local_t_004844e8 == 0.5f);
    std::vector<Entry> twenty;
    static char names2[24][8];
    for (int i = 0; i < 24; ++i) { std::snprintf(names2[i], 8, "f%d", i); twenty.push_back({names2[i], 1, &sink, 0}); }
    build(twenty, 23, 23);
    norrath_menu_local_t_004844e8 = 0.5f;
    draw(320, 240, menu, 0x11, 0x22, nullptr, nullptr, 0);
    // height 534, center = 506 - 267 + 237 = 476 >= 401: shift down by 22 steps until < 401:
    // 476 -> 454 -> 432 -> 410 -> 388 (four steps, shift -88).
    CHECK(norrath_menu_local_t_004844e8 == 1.0f);
    CHECK(has("print 320 " + std::to_string(240 - 267 - 88 - 3) + " f0 1000000"));
    build(twenty, 0, 0);
    norrath_menu_local_t_004844e8 = 0.5f;
    draw(320, 0, menu, 0x11, 0x22, nullptr, nullptr, 0);
    // center = 0 - 267 + 237 = -30 -> +22 twice = 14 -> +22 once more = 36: shift +66.
    CHECK(norrath_menu_local_t_004844e8 == 1.0f);
    CHECK(has("print 320 " + std::to_string(0 - 267 + 66 - 3) + " f0 1000000"));
    // The outer border clamps its top at 0 and its height at 448.
    // The outer border clamps: top 0 - 267 - 10 < 0 clamps to 0; 534 + 20 > 448 clamps to 448; width 70 + 14.
    CHECK(has("border 278 0 84 448 7 80808080 0x0 0 0"));

    // Plate rounding takes half away from zero and keeps the parity row.
    build({{"A", 1, &sink, 0}}, 0, 0);
    draw(320, 240, menu, 0x11, 0x22, nullptr, nullptr, 0);
    // A single entry: previous and cursor equal, width 'A'(2: "A" -> 1 char) 10 + 40 = 50; y 240 - 14 - 3 = 223.
    CHECK(has("smooth 295 223 50 33 7 300000ff 0 0 0"));

    if (failures == 0) std::printf("menu_draw ok\n");
    return failures != 0;
}
