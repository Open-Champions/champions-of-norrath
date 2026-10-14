// Host contract for identityResult. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char identityResult[1600];
int main() {
    if (sizeof identityResult != 1600u) return 1;
    for (unsigned i = 0; i < sizeof identityResult; ++i) {
        if (identityResult[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "identityResult", i);
            return 1;
        }
    }
    identityResult[0] = 1;
    if (identityResult[0] != 1) return 1;
    return 0;
}
