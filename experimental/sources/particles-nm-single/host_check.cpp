// Host contract for nmSingle. Not an EE plan input.
#include <cstdio>
extern "C" unsigned nmSingle;
int main() {
    if (sizeof nmSingle != 4u) return 1;
    if (nmSingle != 0u) {
        std::fprintf(stderr, "fail: nmSingle\n");
        return 1;
    }
    return 0;
}
