// Host contract for animResults. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char animResults[100 * 1568];
int main() {
    if (sizeof animResults != 156800u) return 1;
    for (unsigned i = 0; i < sizeof animResults; ++i) {
        if (animResults[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "animResults", i);
            return 1;
        }
    }
    animResults[0] = 1;
    if (animResults[0] != 1) return 1;
    return 0;
}
