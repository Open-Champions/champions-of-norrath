#include <cstdio>
#include <initializer_list>
#include <cstring>

#include "save.cpp"

int main() {
    int failures = 0;
    for (unsigned value : {0u, 1u, 2u, 0x80000000u, 0xdeadbeefu}) {
        alignas(8) unsigned char object[512];
        std::memset(object, 0x11, sizeof object);
        std::memcpy(object + 316, &value, 4);
        alignas(8) char out[16];
        std::memset(out, 0x77, sizeof out);
        int used = msg_save__13AutoContainerPc(object, out);
        unsigned got;
        std::memcpy(&got, out, 4);
        if (used != 4) { std::fprintf(stderr, "fail: size %d\n", used); ++failures; }
        if (got != value) { std::fprintf(stderr, "fail: word %08x\n", got); ++failures; }
        if (out[4] != 0x77) { std::fprintf(stderr, "fail: wrote past the word\n"); ++failures; }
    }
    return failures ? 1 : 0;
}
