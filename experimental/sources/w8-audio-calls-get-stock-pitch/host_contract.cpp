#include "get_stock_pitch.cpp"
#include <cstdio>
#include <cstring>
static int seen = 0;
extern "C" int convertEnd__Fi(int value) { seen = value; return value + 1; }
int main() {
    alignas(8) unsigned char header[64] = {};
    int word = 47999;
    std::memcpy(header + 16, &word, 4);
    if (getStockPitch__FP10_vagHeader(header) != (48000 << 12) / 48000) return 1;
    if (seen != 47999) return 2;
    word = 24000 - 1;
    std::memcpy(header + 16, &word, 4);
    if (getStockPitch__FP10_vagHeader(header) != 2048) return 3;
    word = -48001;  // negative quotient truncates toward zero
    std::memcpy(header + 16, &word, 4);
    if (getStockPitch__FP10_vagHeader(header) != (-48000 * 4096) / 48000) return 4;
    word = 11;
    std::memcpy(header + 16, &word, 4);
    if (getStockPitch__FP10_vagHeader(header) != (12 * 4096) / 48000) return 5;
    std::puts("get_stock_pitch passed");
    return 0;
}
