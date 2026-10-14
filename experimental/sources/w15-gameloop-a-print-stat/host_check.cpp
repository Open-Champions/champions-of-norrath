// Host check for printStat__FiPUsiiii. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "print_stat.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

signed char language;
unsigned smallFont = 0x1234;
static std::vector<std::string> events;
extern "C" {
void usprintf__FPUsT0ffffffff(UString* destination, UString* format, float a, float, float, float, float, float, float, float) {
    events.push_back("label:" + std::to_string(a));
    destination[0] = format[0]; destination[1] = 0;
}
void usprintf__FPUsPcffffffff(UString* destination, const char* format, float a, float b, float, float, float, float, float, float) {
    events.push_back(std::string("value:") + format + ":" + std::to_string(a) + ":" + std::to_string(b));
    destination[0] = 0x39; destination[1] = 0;
}
int fontStringSizeu__FP4FontPUsi(void* font, UString* text, int width) {
    events.push_back("size:" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(font)) + ":" + std::to_string(text[0]) + ":" + std::to_string(width));
    return text[0] == 0x39 ? 7 : 100;
}
void fontPrintu__FiiPUsi(int x, int y, UString* text, int width) {
    events.push_back("print:" + std::to_string(x) + "," + std::to_string(y) + ":" + std::to_string(text[0]) + ":" + std::to_string(width));
}
void fontPrintCenteredu__FiiPUsi(int x, int y, UString* text, int width) {
    events.push_back("center:" + std::to_string(x) + "," + std::to_string(y) + ":" + std::to_string(text[0]) + ":" + std::to_string(width));
}
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n", __LINE__, #c); return 1; } } while (0)
int main() {
    UString label[2] = {0x4c, 0};
    // default layout, slot 0, right aligned: base 128, left = 128 - 48 = 80, right = 128 - 89 = 39, y = 257
    language = 0;
    printStat__FiPUsiiii(0, label, 42, 0, 0, 0);
    CHECK(events.size() == 6);
    CHECK(events[1] == "size:" + std::to_string(0x1234) + ":76:1000000");
    CHECK(events[2] == "print:-20,257:76:1000000");
    CHECK(events[3] == "value:%d:42.000000:0.000000");
    CHECK(events[5] == "print:32,257:57:1000000");   // 39 - width 7
    // language 1 adds 11 to the default left; shrink lowers y by 56; centered with slot 2 subtracts 320:
    // base 768, left 768 + 11 - 48 = 731, right 768 - 89 = 679, y 257 - 56 = 201
    events.clear(); language = 1;
    printStat__FiPUsiiii(2, label, -3, 0, 1, 1);
    CHECK(events[2] == "print:631,201:76:1000000");
    CHECK(events[3] == "value:%d:-3.000000:0.000000");
    CHECK(events[4] == "center:1090,201:57:1000000");
    // centered with slot 0 does not subtract 320; mode 3 at base 128: left 132, right 179, y 377
    events.clear(); language = 2;
    printStat__FiPUsiiii(0, label, 5, 3, 1, 0);
    CHECK(events[2] == "print:32,377:76:1000000");
    CHECK(events[4] == "center:311,377:57:1000000");
    // mode -6 with language 4 at slot 1: base 448, left 448 + 22 - 10 = 460, right 373, y 179
    events.clear(); language = 4;
    printStat__FiPUsiiii(1, label, 5, -6, 0, 0);
    CHECK(events[2] == "print:360,179:76:1000000");
    CHECK(events[5] == "print:366,179:57:1000000");
    // mode -7 uses y 158; mode -2 at base 128: left 85, right 81, y 286
    events.clear();
    printStat__FiPUsiiii(1, label, 5, -7, 0, 0);
    CHECK(events[2] == "print:360,158:76:1000000");
    events.clear(); language = 0;
    printStat__FiPUsiiii(0, label, 5, -2, 0, 0);
    CHECK(events[2] == "print:-15,286:76:1000000" && events[5] == "print:74,286:57:1000000");
    std::puts("ok");
    return 0;
}
