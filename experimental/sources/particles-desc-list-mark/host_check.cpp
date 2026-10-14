// Host contract for descListMark. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char descListMark[128];
int main() {
    if (sizeof descListMark != 128u) return 1;
    for (unsigned i = 0; i < sizeof descListMark; ++i) {
        if (descListMark[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "descListMark", i);
            return 1;
        }
    }
    descListMark[0] = 1;
    if (descListMark[0] != 1) return 1;
    return 0;
}
