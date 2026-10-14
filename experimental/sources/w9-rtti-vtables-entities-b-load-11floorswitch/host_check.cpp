#include <cstdio>
#include <initializer_list>
#include <cstring>

#include "load.cpp"

int main() {
    int failures = 0;
    for (unsigned value : {0u, 1u, 0x7fu, 0x80u, 0xffu}) {
        alignas(8) unsigned char object[128];
        std::memset(object, 0x11, sizeof object);
        char in[4] = {static_cast<char>(value), 0x22, 0x33, 0x44};
        int used = msg_load__11FloorSwitchPc(object, in);
        if (used != 1) { std::fprintf(stderr, "fail: size %d\n", used); ++failures; }
        if (object[80] != value) { std::fprintf(stderr, "fail: stored %02x\n", object[80]); ++failures; }
        if (object[79] != 0x11 || object[81] != 0x11) { std::fprintf(stderr, "fail: neighbours\n"); ++failures; }
    }
    return failures ? 1 : 0;
}
