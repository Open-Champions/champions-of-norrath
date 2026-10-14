// Host check only; not an EE build input. Checks the extent and that entry i is 8 minus the bit length of i.
#include <cstdio>
extern "C" const unsigned char bitoff[256];
int main() {
    if (sizeof(bitoff) != 256) return 1;
    for (int i = 0; i < 256; ++i) {
        int bits = 0;
        for (int v = i; v; v >>= 1) ++bits;
        if (bitoff[i] != 8 - bits) return 2;
    }
    std::puts("bitoff ok");
    return 0;
}
