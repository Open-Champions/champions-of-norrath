// Host contract for worldCameraMatrix. Not an EE plan input.
#include <cstdio>
extern "C" unsigned char worldCameraMatrix[48];
int main() {
    if (sizeof worldCameraMatrix != 48u) return 1;
    for (unsigned i = 0; i < sizeof worldCameraMatrix; ++i) {
        if (worldCameraMatrix[i] != 0) {
            std::fprintf(stderr, "fail: %s byte %u\n", "worldCameraMatrix", i);
            return 1;
        }
    }
    worldCameraMatrix[0] = 1;
    if (worldCameraMatrix[0] != 1) return 1;
    return 0;
}
