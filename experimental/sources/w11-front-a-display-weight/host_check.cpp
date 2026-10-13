// Host check for displayWeight__FP12playerStructP12screenStruct. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -Wno-deprecated-declarations -ffp-contract=off -fno-builtin host_check.cpp
// The callees are stubs that append to a trace; the check compares the whole trace, including
// the bar index (percentage / 14, limited to 6), the colours, offsets and the formatted text.
#include "display_weight.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
void* scaleTex[7];
void* smallFont;
}
static std::string trace;
static int fontToken;
static char tex[7];

extern "C" {
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color) {
    int idx = -1;
    for (int i = 0; i < 7; ++i) if (texture == &tex[i]) idx = i;
    trace += "tex" + std::to_string(idx) + "," + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(mode) + "," +
             std::to_string(color) + ";";
}
void fontStart__FP4FontiUii(void* font, int size, unsigned color, int flags) {
    trace += std::string("fs") + (font == &fontToken ? "ok" : "?") + "," + std::to_string(size) + "," + std::to_string(color) + "," +
             std::to_string(flags) + ";";
}
unsigned long long fptodp(float value) {
    double d = value;
    unsigned long long bits;
    std::memcpy(&bits, &d, 8);
    return bits;
}
int sprintf(char* out, const char* format, ...) {
    // only the one format the function uses: "%.1f/%d" with a double passed as 64-bit bits and an int
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    unsigned long long bits = __builtin_va_arg(ap, unsigned long long);
    int n = __builtin_va_arg(ap, int);
    __builtin_va_end(ap);
    double d;
    std::memcpy(&d, &bits, 8);
    if (std::strcmp(format, "%.1f/%d") != 0) { trace += "badformat;"; return 0; }
    return std::snprintf(out, 32, "%.1f/%d", d, n);
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

static std::string expect(int idx, int x, int y, const char* text) {
    return "tex" + std::to_string(idx) + "," + std::to_string(x + 8) + "," + std::to_string(y + 4) + ",1,2155905152;fsok,1,2150775868,-1;size" +
           "ok," + text + ",100000;print" + std::to_string(x - (static_cast<int>(std::strlen(text)) * 7 + 3) + 127) + "," +
           std::to_string(y + 17) + "," + text + ",100000;end;";
}

static int run(float cur, float max, int x, int y, int idx, const char* text) {
    alignas(8) unsigned char player[400] = {};
    std::memcpy(player + 308, &cur, 4);
    std::memcpy(player + 312, &max, 4);
    int screen[2] = {x, y};
    trace.clear();
    displayWeight__FP12playerStructP12screenStruct(player, screen);
    std::string want = expect(idx, x, y, text);
    if (trace != want) { std::printf("cur %g max %g\n got  %s\n want %s\n", cur, max, trace.c_str(), want.c_str()); return 1; }
    return 0;
}

int main() {
    for (int i = 0; i < 7; ++i) scaleTex[i] = &tex[i];
    smallFont = &fontToken;
    int rc = 0;
    rc |= run(0.f, 80.f, 100, 200, 0, "0.0/80");
    rc |= run(12.5f, 80.f, 100, 200, 1, "12.5/80");   // 15.6 percent
    rc |= run(11.f, 100.f, 0, 0, 0, "11.0/100");      // 11 percent: below the first step
    rc |= run(13.99f, 100.f, 0, 0, 0, "14.0/100");    // 13.99 truncates to 13
    rc |= run(14.f, 100.f, 0, 0, 1, "14.0/100");
    rc |= run(27.9f, 100.f, 5, 6, 1, "27.9/100");
    rc |= run(28.f, 100.f, 5, 6, 2, "28.0/100");
    rc |= run(83.f, 100.f, 5, 6, 5, "83.0/100");
    rc |= run(84.f, 100.f, 5, 6, 6, "84.0/100");      // 84 / 14 = 6
    rc |= run(120.f, 80.f, 640, 480, 6, "120.0/80");  // above 100 percent: limited to 6
    rc |= run(1000.f, 100.f, -20, 5, 6, "1000.0/100");
    rc |= run(5.f, 80.75f, 100, 100, 0, "5.0/80");    // max is truncated for the %d
    rc |= run(6.25f, 12.f, 3, 4, 3, "6.2/12");        // 52 percent -> 3; %.1f of 6.25
    if (rc) return rc;
    std::puts("display weight passed");
    return 0;
}
