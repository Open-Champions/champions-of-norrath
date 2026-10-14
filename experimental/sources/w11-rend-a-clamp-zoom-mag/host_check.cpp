#include <cstring>
extern "C" int clampZoomMag__6Camera(unsigned char* camera);
static float run(float in, int* ret) {
    unsigned char cam[512] = {};
    std::memcpy(cam + 144, &in, 4);
    *ret = clampZoomMag__6Camera(cam);
    float out; std::memcpy(&out, cam + 144, 4);
    return out;
}
int main() {
    int r;
    if (run(0.5f, &r) != 0.5f || r != 0) return 1;
    if (run(0.0f, &r) != 0.0f || r != 0) return 2;
    if (run(1.0f, &r) != 1.0f || r != 0) return 3;
    if (run(1.5f, &r) != 1.0f || r != 1) return 4;
    if (run(-0.25f, &r) != 0.0f || r != 1) return 5;
    return 0;
}
