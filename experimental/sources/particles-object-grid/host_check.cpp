// Host contract for objectGrid. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char objectGrid[120 * 120 * 4];
int main() {
    if (sizeof objectGrid != 57600u) return 1;
    for (unsigned i = 0; i < sizeof objectGrid; ++i) {
        if (objectGrid[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "objectGrid", i);
            return 1;
        }
    }
    objectGrid[0] = 1;
    if (objectGrid[0] != 1) return 1;
    return 0;
}
