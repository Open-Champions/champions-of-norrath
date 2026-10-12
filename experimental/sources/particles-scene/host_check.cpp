// Host contract for scene. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char scene[56];
int main() {
    if (sizeof scene != 56u) return 1;
    for (unsigned i = 0; i < sizeof scene; ++i) {
        if (scene[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "scene", i);
            return 1;
        }
    }
    scene[0] = 1;
    if (scene[0] != 1) return 1;
    return 0;
}
