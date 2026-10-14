#include <cstdio>
#include "zbuf_addr.cpp"
static unsigned long long param_word = 1;
extern "C" const unsigned long long* sceGsGetGParam() { return &param_word; }
int main() {
    // Mode word exactly 1 uses shift 16; the high halves of each word's upper bits are ignored.
    param_word = 1;
    if (sceGszbufaddr(0, 640, 224) != 10 * 7) return 1;
    param_word = 0xabcd0000abcd0001ull;
    if (sceGszbufaddr(0, 640, 224) != 10 * 7) return 2;
    param_word = 0;
    if (sceGszbufaddr(0, 640, 224) != 10 * 7 * 2) return 3;
    param_word = 0x0000000100000001ull;
    if (sceGszbufaddr(0, 640, 224) != 10 * 7 * 2) return 4;
    param_word = 1;
    // Bit 1 of the index selects 64-row height blocks.
    if (sceGszbufaddr(2, 640, 224) != 10 * 4) return 5;
    if (sceGszbufaddr(3, 640, 224) != 10 * 4) return 6;
    if (sceGszbufaddr(1, 640, 224) != 10 * 7) return 7;
    // Rounding up for odd sizes.
    if (sceGszbufaddr(0, 641, 225) != 11 * 8) return 8;
    if (sceGszbufaddr(2, 1, 1) != 1) return 9;
    // Arguments are truncated to 16-bit signed values: 0x10000 + 64 acts as 64.
    if (sceGszbufaddr(0x10000, 0x10040, 0x10020) != 1) return 10;
    // A negative width divides toward zero: (-100 + 63) / 64 == 0.
    if (sceGszbufaddr(0, -100, 64) != 0) return 11;
    // The shifted product is sign-extended from bit 15.
    param_word = 1;
    if (sceGszbufaddr(0, 640, 224) < 0) return 12;
    if (sceGszbufaddr(0, 4096, 8192) != static_cast<short>(64 * 256)) return 13;   // 16384 fits
    if (sceGszbufaddr(0, 8192, 8192) != static_cast<short>(128 * 256)) return 14;  // 32768 wraps negative
    param_word = 0;
    if (sceGszbufaddr(0, 4096, 4096) != static_cast<short>(64 * 128 * 2)) return 15;
    std::puts("sceGszbufaddr contract passed");
    return 0;
}
