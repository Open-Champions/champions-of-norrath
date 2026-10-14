// Host contract for block. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char block[128 * 4096];
int main() {
    if (sizeof block != 524288u) return 1;
    for (unsigned i = 0; i < sizeof block; ++i) {
        if (block[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "block", i);
            return 1;
        }
    }
    block[0] = 1;
    if (block[0] != 1) return 1;
    return 0;
}
