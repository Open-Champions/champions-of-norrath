// Host contract for drawParticlesTask__Fv. Not an EE plan input.
#include <cstdio>

extern "C" void drawParticlesTask__Fv();
extern "C" unsigned char worldCameraMatrix[48] = {};

namespace {
int calls = 0;
void* seen = nullptr;
}

extern "C" void P_DrawParticles__FP8Matrix34(void* matrix) {
    ++calls;
    seen = matrix;
}

int main() {
    drawParticlesTask__Fv();
    if (calls != 1 || seen != worldCameraMatrix) {
        std::fprintf(stderr, "fail: draw call\n");
        return 1;
    }
    return 0;
}
