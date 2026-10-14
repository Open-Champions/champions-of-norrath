// Host contract for worldCameraPos. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char worldCameraPos[12];
int main() {
    if (sizeof worldCameraPos != 12u) return 1;
    for (unsigned i = 0; i < sizeof worldCameraPos; ++i) {
        if (worldCameraPos[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "worldCameraPos", i);
            return 1;
        }
    }
    worldCameraPos[0] = 1;
    if (worldCameraPos[0] != 1) return 1;
    return 0;
}
