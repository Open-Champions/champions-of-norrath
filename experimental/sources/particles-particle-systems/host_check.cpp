// Host contract for PS. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char PS[16 * 312];
int main() {
    if (sizeof PS != 4992u) return 1;
    for (unsigned i = 0; i < sizeof PS; ++i) {
        if (PS[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "PS", i);
            return 1;
        }
    }
    PS[0] = 1;
    if (PS[0] != 1) return 1;
    return 0;
}
