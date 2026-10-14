// Host contract for objectIgnoreFlags. Not an EE plan input.
#include <cstdio>
extern "C" unsigned objectIgnoreFlags;
int main() {
    if (sizeof objectIgnoreFlags != 4u) return 1;
    if (objectIgnoreFlags != 0u) {
        std::fprintf(stderr, "fail: objectIgnoreFlags\n");
        return 1;
    }
    return 0;
}
