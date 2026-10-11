// Host contract for invisible. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char invisible[80];
int main() {
    if (sizeof invisible != 80u) return 1;
    for (unsigned i = 0; i < sizeof invisible; ++i) {
        if (invisible[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "invisible", i);
            return 1;
        }
    }
    invisible[0] = 1;
    if (invisible[0] != 1) return 1;
    return 0;
}
