// Host check for displayResistances__Fi. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_ARENA host_check.cpp
#include <cstdio>
#include <cstdarg>
#include <string>
#include <vector>
#include "../w9-animation-object-c-common/host_arena.hpp"
#include "display_resistances.cpp"

unsigned char statsScreen[232];
unsigned char player[23984 * 3];
unsigned smallFont = 0x4321;
static std::vector<std::string> events;
extern "C" {
void fontStart__FP4FontiUii(void* font, int a, unsigned color, int b) {
    events.push_back("start:" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(font)) + "," + std::to_string(a) + "," + std::to_string(color) + "," + std::to_string(b));
}
void fontColor__FUiii(unsigned color, int a, int b) { events.push_back("color:" + std::to_string(color) + "," + std::to_string(a) + "," + std::to_string(b)); }
void fontEnd__Fv() { events.push_back("end"); }
UString* textGetString__FPc(const char* key) { events.push_back(std::string("key:") + key); static UString heading[2] = {0x48, 0}; return heading; }
void fontPrintCenteredu__FiiPUsi(int x, int y, UString* text, int width) {
    events.push_back("headline:" + std::to_string(x) + "," + std::to_string(y) + ":" + std::to_string(text[0]) + ":" + std::to_string(width));
}
void fontPrintCentered__FiiPci(int x, int y, char* text, int width) {
    events.push_back("row:" + std::to_string(x) + "," + std::to_string(y) + ":" + text + ":" + std::to_string(width));
}
int formatText(char* buffer, const char* format, ...) {
    va_list args; va_start(args, format);
    int v = va_arg(args, int); va_end(args);
    events.push_back(std::string("format:") + format);
    return std::snprintf(buffer, 40, "%d%%", v);
}
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)

int main() {
    using namespace norrath_aoc;
    // slot 1: record at statsScreen + 116, offsets (10, -20), player index 2
    unsigned char* record = statsScreen + 116;
    store_word(record, 10); store_word(record + 4, static_cast<unsigned>(-20)); store_word(record + 52, 2);
    unsigned char* resist = arena_at(0x1000);
    store_pointer(player + 23984 * 2 + 21872, resist);
    const float fractions[6] = {9.0f, 0.255f, 0.5f, 0.0f, 1.0f, 0.505f};   // index 0 is unused here
    for (int k = 0; k < 6; ++k) store_float(resist + 84 + 4 * k, fractions[k]);
    displayResistances__Fi(1);
    const std::vector<std::string> want = {
        "start:17185,6,2150775868,-1", "color:2150775868,-1,0", "key:Resistances", "headline:593,156:72:1000000",
        "format:%d%%", "row:" + std::to_string(10 + 39 + 237) + "," + std::to_string(-20 + 7 + 188) + ":26%:10000",
        "format:%d%%", "row:286,224:50%:10000",
        "format:%d%%", "row:286,275:0%:10000",
        "format:%d%%", "row:286,326:100%:10000",
        "format:%d%%", "row:286,372:51%:10000", "end"};
    if (events != want) {
        std::printf("event list differs\n");
        for (const std::string& e : events) std::printf("  %s\n", e.c_str());
        return 1;
    }
    std::puts("ok");
    return 0;
}
