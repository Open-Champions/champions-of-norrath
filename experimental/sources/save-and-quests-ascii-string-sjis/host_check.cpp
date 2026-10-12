#include <cstdio>
#include <cstring>
extern "C" unsigned Ascii2Sjis__FUc(unsigned char c) {
    // stand-in with a distinct byte in each half, and garbage above 16 bits
    return 0xabcd0000u | (static_cast<unsigned>(c) << 8) | (c ^ 0x5au);
}
#include "ascii_string_sjis.cpp"
int main() {
    unsigned short out[8];
    std::memset(out, 0x7e, sizeof out);
    unsigned char text[] = "Ab";
    AsciiString2Sjis__FPUcPUs(text, out);
    if (out[0] != (((unsigned)'A' ^ 0x5au) << 8 | 'A')) return 1;
    if (out[1] != (((unsigned)'b' ^ 0x5au) << 8 | 'b')) return 2;
    if (out[2] != 0 || out[3] != 0x7e7e) return 3;
    unsigned char empty[] = "";
    std::memset(out, 0x7e, sizeof out);
    AsciiString2Sjis__FPUcPUs(empty, out);
    if (out[0] != 0 || out[1] != 0x7e7e) return 4;
    std::puts("ok");
    return 0;
}
