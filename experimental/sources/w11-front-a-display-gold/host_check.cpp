// Host check for displayGold__FP12playerStructP12screenStruct. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -Wno-deprecated-declarations -fno-builtin host_check.cpp
// The callees are stubs that append to a trace. The expected text is built here with a plain
// comma-grouping routine for the lengths that occur after the clamp (1 to 8 digits).
#include "display_gold.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
void* moneyTex[7];
void* smallFont;
}
static std::string trace;
static int fontToken;
alignas(8) static unsigned char tex[7][16];

extern "C" {
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color) {
    int idx = -1;
    for (int i = 0; i < 7; ++i) if (texture == tex[i]) idx = i;
    trace += "tex" + std::to_string(idx) + "," + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(mode) + "," +
             std::to_string(color) + ";";
}
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags) {
    trace += std::string("fs") + (font == &fontToken ? "ok" : "?") + "," + std::to_string(size) + "," + std::to_string(color) + "," +
             std::to_string(flags) + ";";
}
int sprintf(char* out, const char* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    int v = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    if (std::strcmp(format, "%d") != 0) { trace += "badformat;"; return 0; }
    return std::snprintf(out, 48, "%d", v);
}
int fontStringSize__FP4FontPci(void* font, char* text, int limit) {
    trace += std::string("size") + (font == &fontToken ? "ok," : "?,") + text + "," + std::to_string(limit) + ";";
    return static_cast<int>(std::strlen(text)) * 7 + 3;
}
void fontPrint__FiiPci(int x, int y, char* text, int limit) {
    trace += "print" + std::to_string(x) + "," + std::to_string(y) + "," + text + "," + std::to_string(limit) + ";";
}
void fontEnd__Fv() { trace += "end;"; }
}

static std::string group(long long v) {
    std::string digits = std::to_string(v < 0 ? -v : v), out;
    for (size_t i = 0; i < digits.size(); ++i) {
        if (i && (digits.size() - i) % 3 == 0) out += ',';
        out += digits[i];
    }
    return (v < 0 ? "-" : "") + out;
}

static int run(int gold, int x, int y, int idx, const char* text, int shortTop, unsigned flagBit, int wantGoldAfter) {
    alignas(8) unsigned char player[300] = {};
    std::memcpy(player + 232, &gold, 4);
    int screen[2] = {x, y};
    std::memset(tex, 0, sizeof tex);
    for (int i = 0; i < 7; ++i) moneyTex[i] = tex[i];
    const short top = static_cast<short>(shortTop);
    const unsigned short fl = static_cast<unsigned short>(flagBit | 0x123);
    std::memcpy(tex[idx] + 2, &top, 2);
    std::memcpy(tex[idx] + 8, &fl, 2);
    trace.clear();
    displayGold__FP12playerStructP12screenStruct(player, screen);
    const int width = static_cast<int>(std::strlen(text)) * 7 + 3;
    std::string want = "tex" + std::to_string(idx) + "," + std::to_string(x + 278) + "," +
                       std::to_string(y + 38 + ((fl & 0x800u) != 0) - shortTop) + ",1,2155905152;fsok,1,2150775868,-1;sizeok," + text +
                       ",100000;print" + std::to_string(x - width + 275) + "," + std::to_string(y + 16) + "," + text + ",100000;end;";
    int after;
    std::memcpy(&after, player + 232, 4);
    if (trace != want || after != wantGoldAfter) {
        std::printf("gold %d\n got  %s (gold %d)\n want %s\n", gold, trace.c_str(), after, want.c_str());
        return 1;
    }
    return 0;
}

int main() {
    smallFont = &fontToken;
    int rc = 0;
    rc |= run(0, 10, 20, 0, "0", 5, 0, 0);
    rc |= run(99, 10, 20, 0, "99", -5, 0x800, 99);
    rc |= run(100, 0, 0, 1, "100", 12, 0x800, 100);
    rc |= run(1000, 0, 0, 6, "1,000", 0, 0, 1000);  // 1000 / 100 = 10 is limited to 6
    rc |= run(250, 7, 9, 2, "250", 3, 0, 250);
    rc |= run(599, 7, 9, 5, "599", 3, 0x800, 599);
    rc |= run(600, 7, 9, 6, "600", 3, 0, 600);
    rc |= run(1234, 5, 5, 6, "1,234", 1, 0, 1234);
    rc |= run(12345, 5, 5, 6, "12,345", 1, 0x800, 12345);
    rc |= run(123456, 5, 5, 6, "123,456", 1, 0, 123456);
    rc |= run(1234567, 5, 5, 6, "1,234,567", 1, 0, 1234567);
    rc |= run(12345678, 5, 5, 6, "12,345,678", 1, 0, 12345678);
    rc |= run(99999999, 5, 5, 6, "99,999,999", 1, 0, 99999999);
    rc |= run(100000000, 5, 5, 6, "99,999,999", 1, 0, 99999999);     // clamped and stored back
    rc |= run(2147483647, 5, 5, 6, "99,999,999", 1, 0, 99999999);
    rc |= run(-5, 5, 5, 0, "-5", 1, 0, -5);       // division truncates toward zero: index 0
    rc |= run(-99, 5, 5, 0, "-99", 1, 0, -99);
    // gold below -99 gives a negative table index, which the original also reads out of range;
    // that is not exercised on the host.
    if (rc) return rc;
    // comma grouping for every length that occurs after the clamp
    for (long long v : {1LL, 12LL, 123LL, 1234LL, 12345LL, 123456LL, 1234567LL, 12345678LL, 99999999LL}) {
        std::string t = group(v);
        if (run(static_cast<int>(v), 0, 0, v / 100 > 6 ? 6 : static_cast<int>(v / 100), t.c_str(), 0, 0, static_cast<int>(v))) return 2;
    }
    std::puts("display gold passed");
    return 0;
}
