// Host contract for descList. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char descList[32 * 64];
int main() {
    if (sizeof descList != 2048u) return 1;
    for (unsigned i = 0; i < sizeof descList; ++i) {
        if (descList[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "descList", i);
            return 1;
        }
    }
    descList[0] = 1;
    if (descList[0] != 1) return 1;
    return 0;
}
