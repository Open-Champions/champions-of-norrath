// Host contract for bolt. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char bolt[20 * 1844];
int main() {
    if (sizeof bolt != 36880u) return 1;
    for (unsigned i = 0; i < sizeof bolt; ++i) {
        if (bolt[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "bolt", i);
            return 1;
        }
    }
    bolt[0] = 1;
    if (bolt[0] != 1) return 1;
    return 0;
}
