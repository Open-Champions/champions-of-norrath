// Host contract for alwaysDrawList. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char alwaysDrawList[8];
int main() {
    if (sizeof alwaysDrawList != 8u) return 1;
    for (unsigned i = 0; i < sizeof alwaysDrawList; ++i) {
        if (alwaysDrawList[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "alwaysDrawList", i);
            return 1;
        }
    }
    alwaysDrawList[0] = 1;
    if (alwaysDrawList[0] != 1) return 1;
    return 0;
}
