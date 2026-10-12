// Host contract for objectDrawEnable. Not an EE plan input.
#include <cstdio>
extern "C" signed char objectDrawEnable;
int main() {
    if (sizeof objectDrawEnable != 1u) return 1;
    if (objectDrawEnable != 1) {
        std::fprintf(stderr, "fail: objectDrawEnable\n");
        return 1;
    }
    return 0;
}
