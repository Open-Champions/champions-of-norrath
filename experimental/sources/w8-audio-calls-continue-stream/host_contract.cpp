#include "continue_stream.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char stream[3 * 452] = {};
static void setw(int r, int off, unsigned v) { std::memcpy(stream + r * 452 + off, &v, 4); }
static unsigned getw(int r, int off) { unsigned v; std::memcpy(&v, stream + r * 452 + off, 4); return v; }
int main() {
    setw(1, 448, 4);
    setw(1, 108, 0x7); setw(1, 108 + 48, 0xf); setw(1, 108 + 96, 0xff); setw(1, 108 + 144, 0x8);
    soundContinueStream__Fi(1);
    if (getw(1, 108) != 0x7 || getw(1, 108 + 48) != 0x7) return 1;     // only the first flagged block
    if (getw(1, 108 + 96) != 0xff || getw(1, 108 + 144) != 0x8) return 2;
    soundContinueStream__Fi(1);
    if (getw(1, 108 + 96) != 0xf7) return 3;
    soundContinueStream__Fi(1);
    if (getw(1, 108 + 144) != 0) return 4;
    soundContinueStream__Fi(1);                                        // nothing left to clear
    setw(2, 448, 2); setw(2, 108 + 96, 0x8);                           // flagged block beyond the count
    soundContinueStream__Fi(2);
    if (getw(2, 108 + 96) != 0x8) return 5;
    setw(0, 448, 0); setw(0, 108, 0x8);
    soundContinueStream__Fi(0);
    if (getw(0, 108) != 0x8) return 6;
    setw(0, 448, static_cast<unsigned>(-3));
    soundContinueStream__Fi(0);
    if (getw(0, 108) != 0x8) return 7;
    std::puts("continue_stream passed");
    return 0;
}
