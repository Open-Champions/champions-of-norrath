// Host contract for drawParticlesTask. Not an EE plan input.
#include "draw_particles.cpp"

extern "C" {
unsigned char worldCameraMatrix[48];
}

static int calls = 0;
static unsigned char* seen = nullptr;

extern "C" void P_DrawParticles__FP8Matrix34(unsigned char* matrix) {
    ++calls;
    seen = matrix;
}

int main() {
    drawParticlesTask__Fv();
    if (calls != 1 || seen != worldCameraMatrix) return 1;
    drawParticlesTask__Fv();
    if (calls != 2) return 2;
    return 0;
}
