// Host contract for frameStates. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char frameStates[150 * 3868 + 24];
int main() {
    if (sizeof frameStates != 580224u) return 1;
    for (unsigned i = 0; i < sizeof frameStates; ++i) {
        if (frameStates[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "frameStates", i);
            return 1;
        }
    }
    frameStates[0] = 1;
    if (frameStates[0] != 1) return 1;
    return 0;
}
