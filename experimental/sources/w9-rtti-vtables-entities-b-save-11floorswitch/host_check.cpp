#include <cstdio>
#include <cstring>
#include <initializer_list>

#include "save.cpp"

int main() {
    int failures = 0;
    for (unsigned value : {0u, 1u, 0x7fu, 0x80u, 0xffu}) {
        alignas(8) unsigned char object[128];
        std::memset(object, 0x11, sizeof object);
        object[80] = static_cast<unsigned char>(value);
        char out[8];
        std::memset(out, 0x77, sizeof out);
        int used = msg_save__11FloorSwitchPc(object, out);
        if (used != 1) { std::fprintf(stderr, "fail: size %d\n", used); ++failures; }
        if (static_cast<unsigned char>(out[0]) != value) { std::fprintf(stderr, "fail: byte %02x\n", value); ++failures; }
        if (out[1] != 0x77) { std::fprintf(stderr, "fail: wrote past the byte\n"); ++failures; }
    }
    return failures ? 1 : 0;
}
