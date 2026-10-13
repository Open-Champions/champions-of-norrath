// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
extern "C" { alignas(8) unsigned char GLibData[0x668]; }
#include "lg_kbd_enum_hint.cpp"
int main() {
    int failures = 0;
    std::memset(GLibData, 0xa5, sizeof GLibData);
    int w = 0x1234;
    std::memcpy(GLibData + 100, &w, 4);
    unsigned char before[sizeof GLibData];
    std::memcpy(before, GLibData, sizeof before);
    if (lgKbdEnumHint(0) != static_cast<int>(0x80000004u) || std::memcmp(before, GLibData, sizeof before) != 0) { std::puts("null case wrong"); ++failures; }
    int out = -1;
    int r = lgKbdEnumHint(&out);
    int now;
    std::memcpy(&now, GLibData + 100, 4);
    if (r != 0 || out != 0x1234 || now != 0) { std::puts("delivery wrong"); ++failures; }
    std::memcpy(before + 100, &now, 4);
    if (std::memcmp(before, GLibData, sizeof before) != 0) { std::puts("other bytes changed"); ++failures; }
    r = lgKbdEnumHint(&out);
    if (r != 0 || out != 0) { std::puts("second call wrong"); ++failures; }
    if (failures) return 2;
    std::puts("lgKbdEnumHint cases passed");
    return 0;
}
