// Host contract for noiseTable. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char noiseTable[512];
int main() {
    if (sizeof noiseTable != 512u) return 1;
    for (unsigned i = 0; i < sizeof noiseTable; ++i) {
        if (noiseTable[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "noiseTable", i);
            return 1;
        }
    }
    noiseTable[0] = 1;
    if (noiseTable[0] != 1) return 1;
    return 0;
}
