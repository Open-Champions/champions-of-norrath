// Host contract for nmLines. Not an EE plan input.
#include <cstdio>
extern "C" unsigned nmLines;
int main() {
    if (sizeof nmLines != 4u) return 1;
    if (nmLines != 0u) {
        std::fprintf(stderr, "fail: nmLines\n");
        return 1;
    }
    return 0;
}
