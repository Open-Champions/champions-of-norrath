#include <cstdio>
#include <cstring>
extern "C" {
int nextNmDeferedCalls;
int scriptFrame;
unsigned char deferedCall[10240];
}
#define strcpy norrath_host_strcpy
#include "add_defered_call.cpp"
#undef strcpy
extern "C" char* norrath_host_strcpy(char* d, const char* s) { return std::strcpy(d, s); }
int main() {
    char a[] = "alpha";
    char b[] = "beta";
    nextNmDeferedCalls = 0;
    scriptFrame = 4;  // even frame writes bank 1
    scriptAddDeferedCall__FPc(a);
    if (nextNmDeferedCalls != 1 || std::strcmp((char*)deferedCall + 5120, "alpha") != 0) return 1;
    if (deferedCall[0] != 0) return 2;
    scriptFrame = 7;  // odd frame writes bank 0
    scriptAddDeferedCall__FPc(b);
    if (nextNmDeferedCalls != 2 || std::strcmp((char*)deferedCall + 80, "beta") != 0) return 3;
    // fill to 64 and then drop
    nextNmDeferedCalls = 63;
    scriptAddDeferedCall__FPc(a);
    if (nextNmDeferedCalls != 64 || std::strcmp((char*)deferedCall + 63 * 80, "alpha") != 0) return 4;
    std::memset(deferedCall, 0, sizeof deferedCall);
    scriptAddDeferedCall__FPc(b);
    if (nextNmDeferedCalls != 64) return 5;
    for (unsigned i = 0; i < sizeof deferedCall; ++i) if (deferedCall[i]) return 6;
    // negative counts still index (below 64)
    scriptFrame = -1;  // low bit set -> bank 0
    nextNmDeferedCalls = 2;
    scriptAddDeferedCall__FPc(b);
    if (std::strcmp((char*)deferedCall + 160, "beta") != 0) return 7;
    std::puts("ok");
    return 0;
}
