// Host contract for wigglyTab. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char wigglyTab[544];
int main() {
    if (sizeof wigglyTab != 544u) return 1;
    for (unsigned i = 0; i < sizeof wigglyTab; ++i) {
        if (wigglyTab[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "wigglyTab", i);
            return 1;
        }
    }
    wigglyTab[0] = 1;
    if (wigglyTab[0] != 1) return 1;
    return 0;
}
