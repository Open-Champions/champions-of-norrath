// Host contract for lineBuffer. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char lineBuffer[128 * 36];
int main() {
    if (sizeof lineBuffer != 128u * 36u) return 1;
    for (unsigned i = 0; i < sizeof lineBuffer; ++i) {
        if (lineBuffer[i] != 0) {
            std::fprintf(stderr, "fail: lineBuffer[%u]\n", i);
            return 1;
        }
    }
    return 0;
}
