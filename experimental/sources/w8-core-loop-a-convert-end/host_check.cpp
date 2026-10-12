// Host check for convertEnd__Fi. Not an EE plan input.
#include "convert_end.cpp"

static unsigned ref(unsigned v) {
    unsigned char b[4] = {static_cast<unsigned char>(v), static_cast<unsigned char>(v >> 8),
                          static_cast<unsigned char>(v >> 16), static_cast<unsigned char>(v >> 24)};
    return (unsigned)b[0] << 24 | (unsigned)b[1] << 16 | (unsigned)b[2] << 8 | b[3];
}

int main() {
    if (convertEnd__Fi(0x11223344) != 0x44332211) return 1;
    if (convertEnd__Fi(0) != 0 || convertEnd__Fi(-1) != -1) return 2;
    if (convertEnd__Fi(1) != 0x01000000) return 3;
    if (static_cast<unsigned>(convertEnd__Fi(static_cast<int>(0x80000000u))) != 0x80u) return 4;
    unsigned x = 0x9e3779b9u;
    for (int i = 0; i < 1000; ++i) {
        x = x * 1664525u + 1013904223u;
        if (static_cast<unsigned>(convertEnd__Fi(static_cast<int>(x))) != ref(x)) return 5;
        if (convertEnd__Fi(convertEnd__Fi(static_cast<int>(x))) != static_cast<int>(x)) return 6;
    }
    return 0;
}
