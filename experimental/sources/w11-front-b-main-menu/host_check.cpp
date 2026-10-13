// Host check for mainMenu__Fii. Not an EE plan input.
// The soft-double routines are mocked with the host double carried in a 64-bit word.
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
// The soft sin shares its name with the host libm; rename it only for the source.
#define sin soft_sin
#include "main_menu.cpp"
#undef sin

alignas(8) unsigned char shop[2 * 116];
alignas(8) unsigned char netLocalPad[2 * 88];
int padAcceptButton = 0x40;
int padBackButton = 0x120;
void* defaultFont = reinterpret_cast<void*>(0x1000);
signed char language;
void* rcursor;
void* lcursor;
int drawFrameNm;
int shopDisableMouse[2];
int shopMouseDecay[2];

static std::string log_;
static int local_player;
static void note(const std::string& s) { log_ += s + ";"; }
static std::string num(long v) { return std::to_string(v); }

static double as_double(Double v) { double d; std::memcpy(&d, &v, 8); return d; }
static Double as_bits(double d) { Double v; std::memcpy(&v, &d, 8); return v; }

extern "C" {
void shopDrawTaskBar__FP12screenStruct(unsigned char* s) { note("bar" + num((s - shop) / 116)); }
void updateCursor__FiiiiiP12screenStructi(int a, int b, int c, int d, int e, unsigned char* s, int f) {
    note("cur" + num(a) + "," + num(b) + "," + num(c) + "," + num(d) + "," + num(e) + "," + num((s - shop) / 116) + "," + num(f));
}
void* lumpFindResource__FPcT0(char* lump, char* name) {
    static std::string found[8];
    static int k;
    found[k & 7] = std::string(lump) + "/" + name;
    return &found[k++ & 7];
}
static std::string tex_name(void* t) { return t == rcursor || t == lcursor ? "cursor" : *static_cast<std::string*>(t); }
void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int flags, unsigned color) {
    note("tex:" + tex_name(t) + "@" + num(x) + "," + num(y) + "," + num(flags) + "," + num(color));
}
void fontStart__FP4FontiUii(void* f, int a, unsigned c, int b) { note("fs" + num(f == defaultFont) + num(a) + "," + num(c) + "," + num(b)); }
void fontColor__FUiii(unsigned c, int a, int b) { note("fc" + num(c) + "," + num(a) + "," + num(b)); }
static unsigned short text_ids[4];
unsigned short* textGetString__FPc(char* key) {
    note(std::string("txt:") + key);
    return &text_ids[key[0] == 'B' ? 0 : 1];
}
void fontPrintCenteredu__FiiPUsi(int x, int y, unsigned short* t, int w) {
    note("pr" + num(x) + "," + num(y) + "," + num(t - text_ids) + "," + num(w));
}
void fontEnd__Fv() { note("fe"); }
void resetMenu__FP12screenStruct(unsigned char* s) { note("rst" + num((s - shop) / 116)); }
void inventoryUnpack__Fiib(int a, int b, bool c) { note("unp" + num(a) + "," + num(b) + "," + num(c)); }
int netPlayerIsLocal__Fi(int index) { note("loc" + num(index)); return index == local_player; }
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float v) {
    note("snd:" + *static_cast<std::string*>(h) + "," + num(o != nullptr) + "," + num(static_cast<long>(v * 10)));
}
Double litodp(int v) { return as_bits(static_cast<double>(v)); }
Double fptodp(float v) { return as_bits(static_cast<double>(v)); }
Double dpadd(Double a, Double b) { return as_bits(as_double(a) + as_double(b)); }
Double dpmul(Double a, Double b) { return as_bits(as_double(a) * as_double(b)); }
Double soft_sin(Double v) { return as_bits(std::sin(as_double(v))); }
int dptoli(Double v) { return static_cast<int>(as_double(v)); }
}

static void put(unsigned char* base, int off, int v) { std::memcpy(base + off, &v, 4); }
static void putf(unsigned char* base, int off, float v) { std::memcpy(base + off, &v, 4); }
static void reset() {
    std::memset(shop, 0, sizeof shop);
    std::memset(netLocalPad, 0, sizeof netLocalPad);
    for (int n = 0; n < 2; ++n) {
        put(shop + 116 * n, 0, 100 + 50 * n);
        put(shop + 116 * n, 8, 300);
        put(shop + 116 * n, 36, 7 * n);
        put(shop + 116 * n, 76, 3);
    }
    shopDisableMouse[0] = shopDisableMouse[1] = shopMouseDecay[0] = shopMouseDecay[1] = 0;
    language = 0;
    drawFrameNm = 1000;
    local_player = -1;
    log_.clear();
}
static int get(int n, int off) { int v; std::memcpy(&v, shop + 116 * n + off, 4); return v; }
static bool has(const std::string& s) { return log_.find(s) != std::string::npos; }
static int fail(int code, const char* what) { std::fprintf(stderr, "check %d failed: %s\n  log: %s\n", code, what, log_.c_str()); return code; }

int main() {
    static short cursor_texture_header[4] = {20, 0, 0, 0};
    rcursor = cursor_texture_header;
    static short left_header[4] = {30, 0, 0, 0};
    lcursor = left_header;

    // Plain pass, nothing pressed, mode 3, screen 1 for player 0, item 0 selected, language 0.
    reset();
    mainMenu__Fii(1, 0);
    {
        // 150 + 300 / 2 = 300 is the centre; "Buy" is highlighted
        const double step = static_cast<double>(__builtin_bit_cast(float, 0x3dccccccu));
        const double angle = 0.0 * 1.5 + static_cast<double>(static_cast<float>(1000) * static_cast<float>(step));
        const int d = static_cast<int>(10.0 * std::sin(angle));
        std::string want =
            "bar1;cur0,0,2,0,39,1,1;"
            "tex:shop.lmp/buttontop.tex@216,210,0,2155905152;"
            "fs10,2150775868,-1;"
            "fc2152292714,-1,0;txt:Buy;pr300,203,0,1000000;"
            "fc2150775868,-1,0;txt:Sell;pr300,243,1,1000000;"
            "fe;"
            "tex:shop.lmp/buysell.tex@210,174,0,2155905152;"
            "tex:cursor@" + num(300 - 0 - 20 - d - 38) + ",213,1,2155905152;"
            "tex:cursor@" + num(300 + d + 0 + 44) + ",213,1,2155905152;";
        if (log_ != want) return fail(1, "plain pass");
        if (get(1, 84) != 0 || get(1, 76) != 3) return fail(2, "words");
    }
    // Accept pressed on Buy with the float zero: reset, mode 0, then no buysell drawing.
    reset();
    put(netLocalPad, 12, 0x40);
    mainMenu__Fii(0, 0);
    if (!has("fc2152292714,-1,0;txt:Buy;pr250,203,0,1000000;rst0;") || get(0, 76) != 0) return fail(3, "accept Buy");
    if (has("buysell") || has("cursor")) return fail(4, "no arrows once the mode left 3");
    // The float nonzero blocks accept; negative zero compares equal to zero.
    reset();
    put(netLocalPad, 12, 0x40);
    putf(shop, 60, 0.5f);
    mainMenu__Fii(0, 0);
    if (has("rst") || get(0, 76) != 3) return fail(5, "float blocks");
    reset();
    put(netLocalPad, 12, 0x40);
    putf(shop, 60, -0.0f);
    mainMenu__Fii(0, 0);
    if (!has("rst0;") || get(0, 76) != 0) return fail(6, "negative zero");
    // Accept with nothing in the mask does nothing; another button bit is not accept.
    reset();
    put(netLocalPad, 12, 0x80);
    mainMenu__Fii(0, 0);
    if (has("rst") || get(0, 76) != 3) return fail(7, "other bits");
    // Sell selected: highlighted second item, accept unpacks, resets and sets mode 1.
    reset();
    put(shop, 24, 1);
    put(netLocalPad, 12, 0x40);
    mainMenu__Fii(0, 1 - 1);
    if (!has("tex:shop.lmp/buttonbottom.tex@166,248,0,2155905152;") || get(0, 84) != 1) return fail(8, "sell texture");
    if (!has("fc2150775868,-1,0;txt:Buy;pr250,203,0,1000000;fc2152292714,-1,0;txt:Sell;pr250,243,1,1000000;unp0,0,0;rst0;")) return fail(9, "sell accept");
    if (get(0, 76) != 1) return fail(10, "sell mode");
    // Another selection value highlights neither and Sell needs exactly 1.
    reset();
    put(shop, 24, 2);
    put(netLocalPad, 12, 0x40);
    mainMenu__Fii(0, 0);
    if (has("2152292714") || has("rst") || has("unp") || !has("buttonbottom")) return fail(11, "selection 2");
    // Back button on player 1 of screen 1: local player plays the sound; mode 2 ends the arrows.
    reset();
    put(netLocalPad, 88 + 12, 0x100);
    local_player = 1;
    mainMenu__Fii(1, 1);
    if (!has("loc1;snd:fx.lmp/menu_shop_back.anm,0,10;fe;") || get(1, 76) != 2) return fail(12, "back local");
    if (has("buysell")) return fail(13, "no arrows after back");
    reset();
    put(netLocalPad, 88 + 12, 0x20);
    local_player = 0;
    mainMenu__Fii(1, 1);
    if (!has("loc1;fe;") || has("snd") || get(1, 76) != 2) return fail(14, "back not local");
    // The mouse guards on this screen's slot only.
    reset();
    put(netLocalPad, 12, 0x120);
    shopDisableMouse[0] = 1;
    mainMenu__Fii(0, 0);
    if (has("loc") || get(0, 76) != 3) return fail(15, "disable mouse");
    reset();
    put(netLocalPad, 12, 0x100);
    shopMouseDecay[0] = 5;
    mainMenu__Fii(0, 0);
    if (has("loc") || get(0, 76) != 3) return fail(16, "mouse decay");
    reset();
    put(netLocalPad, 12, 0x100);
    shopDisableMouse[1] = 1;
    shopMouseDecay[1] = 1;
    local_player = 0;
    mainMenu__Fii(0, 0);
    if (!has("loc0;snd") || get(0, 76) != 2) return fail(17, "other slot ignored");
    // Language offsets and the bounce for player 1, frame 123.
    const int lang[5] = {0, 1, 3, 4, 2};
    const int shiftv[5] = {0, 9, 6, 5, 0};
    const int basev[5] = {0, -3, -5, -3, 0};
    for (int k = 0; k < 5; ++k) {
        reset();
        language = static_cast<signed char>(lang[k]);
        drawFrameNm = 123;
        mainMenu__Fii(1, 1);
        const double step = static_cast<double>(__builtin_bit_cast(float, 0x3dccccccu));
        const double angle = 1.0 * 1.5 + static_cast<double>(static_cast<float>(123) * static_cast<float>(step));
        const int d = static_cast<int>((basev[k] + 10) * std::sin(angle));
        const int c = 150 + 150;
        std::string want = "tex:cursor@" + num(c - shiftv[k] - 20 - d - 38) + ",213,1,2155905152;tex:cursor@" +
                           num(c + d + shiftv[k] + 44) + ",213,1,2155905152;";
        if (log_.find("tex:cursor@") == std::string::npos || log_.substr(log_.find("tex:cursor@")) != want) return fail(18 + k, "arrow positions");
    }
    // A zero cursor width and negative frame counts still follow the formula.
    reset();
    static short flat[4] = {0, 0, 0, 0};
    rcursor = flat;
    drawFrameNm = -77;
    mainMenu__Fii(0, 0);
    {
        const double step = static_cast<double>(__builtin_bit_cast(float, 0x3dccccccu));
        const double angle = static_cast<double>(static_cast<float>(-77) * static_cast<float>(step));
        const int d = static_cast<int>(10.0 * std::sin(angle));
        if (!has("tex:cursor@" + num(250 - 0 - 0 - d - 38) + ",206,1,")) return fail(30, "negative frames");
    }
    return 0;
}
