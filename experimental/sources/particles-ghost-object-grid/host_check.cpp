// Host contract for ghostObjectGrid. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char ghostObjectGrid[120 * 120 * 4];
int main() {
    if (sizeof ghostObjectGrid != 57600u) return 1;
    for (unsigned i = 0; i < sizeof ghostObjectGrid; ++i) {
        if (ghostObjectGrid[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "ghostObjectGrid", i);
            return 1;
        }
    }
    ghostObjectGrid[0] = 1;
    if (ghostObjectGrid[0] != 1) return 1;
    return 0;
}
