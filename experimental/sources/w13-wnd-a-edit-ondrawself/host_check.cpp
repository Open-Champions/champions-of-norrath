// Host check only; not a build input. Build as x86_64 with statics below 4 GiB.
#include "ondrawself.cpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

alignas(8) static unsigned char object[640];
alignas(8) static int table[24];
alignas(8) static unsigned char options[160];
alignas(8) static unsigned short lines[4][16];
alignas(8) static unsigned char entries[4][8];
unsigned _5VIWnd$mUI = static_cast<unsigned>(reinterpret_cast<unsigned long>(table));
int gKeyboardAttached;
int sCompactKeyboard;
float sTranslucency = 1.0f;
unsigned char sKeyMap[66];
static std::string trace;
static unsigned long long clock_values[4];
static int clock_index;
static Vect2 caret_position;

static std::string num(long v) { return std::to_string(v); }
static std::string text_of(const unsigned short* t) {
    std::string s;
    for (; *t; ++t) s += static_cast<char>(*t);
    return s;
}

extern "C" {
unsigned long long GetTime__7VITimer() { return clock_values[clock_index++]; }
unsigned char* VIGetGameOptions__Fv() { return options; }
void Begin2D__4VIUI(void*) { trace += "begin;"; }
void End2D__4VIUI(void*) { trace += "end;"; }
int GetStringWidth__6VIFontP8VIRasterP12VIDictionaryPCUs(void*, void*, void*, const unsigned short* t) { return 10 * static_cast<int>(text_of(t).size()); }
int GetHeight__6VIFontP8VIRasterP12VIDictionary(void*, void*, void*) { return 20; }
void BlitBar__4VIUIPCUsG10VIColor32FG8VIVect2I9VIUITexIdT4i16VIAlphaBlendModefUii(void*, const unsigned short* t, const Color*, const Vect2* p, int a, int b,
                                                                                 int w, int, float, unsigned, int) {
    trace += "bar:" + text_of(t) + "," + num(p->x) + "," + num(p->y) + "," + num(a) + "," + num(b) + "," + num(w) + ";";
}
void BlitRect__4VIUI9VIUITexIdG8VIVect2IT216VIAlphaBlendModef(void*, int tex, const Vect2* p, const Vect2* s, int, float) {
    trace += "rect:" + num(tex) + "," + num(p->x) + "," + num(p->y) + "," + num(s->x) + "," + num(s->y) + ";";
}
void BlitFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam16VIAlphaBlendModefUc(void*, const Vect2*, const Vect2*, const FrameParam* f, int, float, unsigned char) {
    trace += "frame:" + num(f->v[0]) + "," + num(f->v[6]) + ";";
}
void BlitFillFrame__4VIUIG8VIVect2IT1G14VIUIFrameParam9VIUITexId16VIAlphaBlendModefUc(void*, const Vect2* p, const Vect2* s, const FrameParam* f, int tex, int,
                                                                                      float, unsigned char) {
    trace += "fill:" + num(p->x) + "," + num(p->y) + "," + num(s->x) + "," + num(s->y) + "," + num(f->v[0]) + "," + num(tex) + ";";
}
void BlitEx__4VIUI9VIUITexIdG8VIVect2I16VIAlphaBlendModefUi(void*, int tex, const Vect2* p, int, float, unsigned flags) {
    trace += "blit:" + num(tex) + "," + num(p->x) + "," + num(p->y) + "," + num(flags) + ";";
}
const unsigned short* GetString__13VIStringTableii(const void*, int id, int) {
    static unsigned short buffer[8][8];
    static int next;
    unsigned short* b = buffer[next++ & 7];
    std::string s = "s" + num(id);
    for (size_t i = 0; i <= s.size(); ++i) b[i] = static_cast<unsigned short>(s[i]);
    return b;
}
int snprintf16__8VIStringPUsiPCce(unsigned short* dest, int, const char* format, ...) {
    // Only the formats used by the handler: each "%S" is replaced by the argument strings in order; the host check
    // passes them through a fixed reading of the variable arguments.
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    std::string out;
    for (const char* f = format; *f; ++f) {
        if (f[0] == '%' && f[1] == 'S') {
            out += text_of(__builtin_va_arg(ap, const unsigned short*));
            ++f;
        } else {
            out += *f;
        }
    }
    __builtin_va_end(ap);
    for (size_t i = 0; i <= out.size(); ++i) dest[i] = static_cast<unsigned short>(out[i]);
    return static_cast<int>(out.size());
}
void DrawStringEx__4VIUIPCUsiiG10VIColor32Fi(void*, const unsigned short* t, int x, int y, const Color*, int) {
    trace += "ex:" + text_of(t) + "," + num(x) + "," + num(y) + ";";
}
void SetTextColor__6VIFontRC10VIColor32F(void*, const Color* c) { trace += c->r == 0.0f ? "k;" : "w;"; }
void DrawCenteredString__6VIFontP8VIRasterP12VIDictionaryPCciiUi(void*, void*, void*, const char* t, int x, int y, unsigned) {
    trace += std::string("ch:") + t + "," + num(x) + "," + num(y) + ";";
}
void DrawString__6VIFontP8VIRasterP12VIDictionaryPCUsii(void*, void*, void*, const unsigned short* t, int x, int y) {
    trace += "str:" + text_of(t) + "," + num(x) + "," + num(y) + ";";
}
void FillRectangle__4VIUIPC8VIVect2IPC10VIColor32F(void*, const Vect2* c, const Color*) {
    trace += "caret:" + num(c[0].x) + "," + num(c[0].y) + "," + num(c[1].x) + "," + num(c[1].y) + ";";
}
void MoveCursor__9VIWndEditii(void*, int dx, int dy) { trace += "move:" + num(dx) + "," + num(dy) + ";"; }
void ComputeTextRows__9VIWndEdit(void*) { trace += "rows;"; }
void GetCaretPos__9VIWndEditP8VIVect2I(void*, Vect2* p) { *p = caret_position; }
}

static void set_word(int offset, int v) { std::memcpy(object + offset, &v, 4); }
static void set_float(int offset, float v) { std::memcpy(object + offset, &v, 4); }
static int word(int offset) { int v; std::memcpy(&v, object + offset, 4); return v; }
static float float_at(int offset) { float v; std::memcpy(&v, object + offset, 4); return v; }
static void reset() {
    std::memset(object, 0, sizeof object);
    std::memset(options, 0, sizeof options);
    std::memset(sKeyMap, 0, sizeof sKeyMap);
    table[12] = 3;
    gKeyboardAttached = 0;
    sCompactKeyboard = 7;
    sTranslucency = 1.0f;
    clock_values[0] = 1000; clock_values[1] = 1000; clock_values[2] = 1000; clock_index = 0;
    caret_position = Vect2{0, 0};
    trace.clear();
    set_word(56, 100); set_word(60, 50); set_word(392, 3);
    for (int i = 0; i < 4; ++i) {
        std::memset(lines[i], 0, sizeof lines[i]);
        std::memset(entries[i], 0, 8);
    }
    set_word(432, static_cast<int>(reinterpret_cast<unsigned long>(entries)));
    set_word(436, 0);
}
static void add_line(int i, const char* s) {
    int n = 0;
    for (; s[n]; ++n) lines[i][n] = static_cast<unsigned char>(s[n]);
    unsigned p = static_cast<unsigned>(reinterpret_cast<unsigned long>(lines[i]));
    std::memcpy(entries[i], &p, 4);
    short len = static_cast<short>(n);
    std::memcpy(entries[i] + 4, &len, 2);
    set_word(436, i + 1);
}
static bool has(const char* s) { return trace.find(s) != std::string::npos; }
static bool ok = true;
#define CHECK(c) do { if (!(c)) { ok = false; std::printf("line %d failed\n", __LINE__); if (std::getenv("TRACE")) std::puts(trace.c_str()); } } while (0)

int main() {
    // Layout: three rows, keyboard attached off -> full height 17 * 3 + 203; first draw sets the size words.
    reset();
    OnDrawSelf__9VIWndEdit(object);
    CHECK(word(64) == 452 && word(68) == 254 && sCompactKeyboard == 0 && has("begin;rect:51,100,50,452,254;frame:36,42;fill:116,66,420,59,101,96;") &&
         trace.find("end;") == trace.size() - 4);
    // Compact keyboard needs the attached flag and option byte 145.
    reset(); gKeyboardAttached = 1; options[145] = 1;
    OnDrawSelf__9VIWndEdit(object);
    CHECK(sCompactKeyboard == 1 && word(68) == 17 * 3 + 42 + 26 && has("ex:s909,155,") && has("ex:s753,") && has("ex:s905+s906 = s909,") == false);
    reset(); gKeyboardAttached = 1; options[145] = 1; set_word(440, 1);
    OnDrawSelf__9VIWndEdit(object);
    CHECK(has("ex:s905+s906 = s909,376,129;"));
    reset(); gKeyboardAttached = 1; options[145] = 0;
    OnDrawSelf__9VIWndEdit(object);
    CHECK(sCompactKeyboard == 0 && word(68) == 254);
    // Time step: 2000 ms -> dt = 2 (time stamps 3000 and 1000); fade and slide use 5 dt, pulse uses 3 dt / 2 dt.
    reset(); clock_values[0] = 3000; set_float(360, 0.0f); set_float(352, 30.0f); set_float(364, 0.25f); set_word(368, 0);
    std::memcpy(object + 376, &(const unsigned long long&)1000ull, 8);
    OnDrawSelf__9VIWndEdit(object);
    CHECK(float_at(360) == 1.0f && float_at(352) == 20.0f && float_at(364) == 1.0f && word(368) == -1);
    reset(); clock_values[0] = 3000; set_word(368, -1); set_float(364, 0.5f); set_float(360, 0.0f);
    std::memcpy(object + 376, &(const unsigned long long&)1000ull, 8);
    OnDrawSelf__9VIWndEdit(object);
    CHECK(float_at(364) == 0.0f && word(368) == 1 && float_at(360) == 1.0f);
    // Translucency caps the fade value.
    reset(); sTranslucency = 0.25f; clock_values[0] = 2000; set_float(360, 0.2f);
    std::memcpy(object + 376, &(const unsigned long long&)1000ull, 8);
    OnDrawSelf__9VIWndEdit(object);
    CHECK(float_at(360) == 0.25f);
    // Held direction key repeats every 0.05 once the timer runs out; only in the full layout.
    for (int key = 135; key <= 140; ++key) {
        reset(); clock_values[0] = 2000; set_word(384, key); set_float(388, 0.5f);
        std::memcpy(object + 376, &(const unsigned long long&)1000ull, 8);
        OnDrawSelf__9VIWndEdit(object);
        const char* expected = key == 136 ? "move:-1,0;" : key == 137 ? "move:1,0;" : key == 138 ? "move:0,-1;" : key == 139 ? "move:0,1;" : nullptr;
        CHECK((expected ? has(expected) && float_at(388) > 0.049f && float_at(388) < 0.051f : !has("move:")));
    }
    reset(); set_word(384, 137); set_float(388, 0.5f);  // dt = 1 s from the zero stamp: the timer runs out
    OnDrawSelf__9VIWndEdit(object);
    CHECK(has("move:1,0;") && float_at(388) > 0.049f && float_at(388) < 0.051f);
    reset(); set_word(384, 137); set_float(388, 0.5f); clock_values[0] = 1000; clock_values[1] = 1000;
    {
        const unsigned long long stamp = 1000;
        std::memcpy(object + 376, &stamp, 8);  // dt = 0: timer unchanged and above zero
    }
    OnDrawSelf__9VIWndEdit(object);
    CHECK(!has("move:") && float_at(388) == 0.5f);
    reset(); gKeyboardAttached = 1; options[145] = 1; clock_values[0] = 2000; set_word(384, 137); set_float(388, 0.0f);
    std::memcpy(object + 376, &(const unsigned long long&)1000ull, 8);
    OnDrawSelf__9VIWndEdit(object);
    CHECK(!has("move:"));
    // Title bar centred over the window.
    reset();
    object[72] = 'A'; object[74] = 'B';
    OnDrawSelf__9VIWndEdit(object);
    CHECK(has("bar:AB,291,22,33,34,70;"));
    // Key grid: only non-zero cells are drawn; letters are lower-cased when no shift state is active.
    reset(); sKeyMap[0] = 'Q'; sKeyMap[12] = 5; sKeyMap[13] = 1; sKeyMap[14] = 4;
    set_word(344, 1); set_word(348, 1);
    set_word(86, 1); set_word(87, 1);
    set_float(352, 1.0f);
    OnDrawSelf__9VIWndEdit(object);
    CHECK(has("blit:107,116,129,0;k;ch:q,130,141;w;ch:q,129,140;") && has("blit:107,143,156,0;blit:204,143,156,0;blit:156,147,164,0;blit:108,143,156,0;") &&
         has("blit:154,174,164,0;") && has("blit:107,197,156,0;blit:89,112,134,0;"));
    // Upper-case key caps stay upper-case while a shift state is held (words +44/+48/+52 as the handler reads them).
    reset(); sKeyMap[0] = 'Q'; set_word(44, 1); set_word(52, 0);
    OnDrawSelf__9VIWndEdit(object);
    CHECK(has("ch:Q,130,141;") && !has("ch:q,"));
    // Text lines: newline shows as a space, password mode masks every unit, caret blink on even half seconds.
    reset(); add_line(0, "ab"); add_line(1, "c"); lines[0][1] = 10; set_word(436, 2);
    caret_position = Vect2{1, 1}; clock_values[0] = 1000; clock_values[1] = 1000; clock_values[2] = 1250;
    set_word(392, 2);
    OnDrawSelf__9VIWndEdit(object);
    CHECK(has("rows;") && has("str:a ,123,67;") && has("str:a ,122,66;") && has("str:c,123,83;") && has("caret:132,87,134,102;") && word(400) == 0);
    reset(); add_line(0, "ab"); set_word(396, 1); caret_position = Vect2{0, 0};
    clock_values[0] = 1000; clock_values[1] = 1600;
    OnDrawSelf__9VIWndEdit(object);
    CHECK(has("str:**,122,66;") && !has("caret:"));
    // Scrolling keeps the caret row visible.
    reset(); add_line(0, "a"); set_word(436, 4); set_word(392, 2); caret_position = Vect2{0, 3};
    OnDrawSelf__9VIWndEdit(object);
    CHECK(word(400) == 2);
    reset(); set_word(400, 5); set_word(436, 0); caret_position = Vect2{0, 1};
    OnDrawSelf__9VIWndEdit(object);
    CHECK(word(400) == 1);
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
