// Host contract for trail. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char trail[64 * 1040];
int main() {
    if (sizeof trail != 66560u) return 1;
    for (unsigned i = 0; i < sizeof trail; ++i) {
        if (trail[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "trail", i);
            return 1;
        }
    }
    trail[0] = 1;
    if (trail[0] != 1) return 1;
    return 0;
}
