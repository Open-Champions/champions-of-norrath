// Host contract for offTheGrid. Not an EE plan input.
#include <cstdio>
extern "C" unsigned offTheGrid;
int main() {
    if (sizeof offTheGrid != 4u) return 1;
    if (offTheGrid != 0u) {
        std::fprintf(stderr, "fail: offTheGrid\n");
        return 1;
    }
    return 0;
}
