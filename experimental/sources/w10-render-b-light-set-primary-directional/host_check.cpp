// Host contract for lightSetPrimaryDirectional__FG6Point3Ps. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct Point3 {
    float x, y, z;
};
extern "C" {
unsigned char charDirect[8];
unsigned char charDirectDir[16];
unsigned char worldCameraMatrix[48];
static const unsigned char* seenMatrix;
static Point3 seenIn;
static unsigned char* seenOut;
void matTransform34Vec__FP6Point3P8Matrix34T0(unsigned char* out, unsigned char* matrix, unsigned char* in) {
    seenOut = out; seenMatrix = matrix; std::memcpy(&seenIn, in, 12);
    float v[3] = {1.0f, 2.0f, 3.0f};
    std::memcpy(out, v, 12);
}
void lightSetPrimaryDirectional__FG6Point3Ps(Point3*, short*);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Point3 dir = {0.25f, -0.5f, 0.75f};
    short color[4] = {11, -22, 33, 44};
    lightSetPrimaryDirectional__FG6Point3Ps(&dir, color);
    short got[4];
    std::memcpy(got, charDirect, 8);
    expect(std::memcmp(got, color, 8) == 0, "four halfwords copied");
    expect(seenOut == charDirectDir && seenMatrix == worldCameraMatrix, "call arguments");
    expect(seenIn.x == 0.25f && seenIn.y == -0.5f && seenIn.z == 0.75f, "direction copied");
    float out[3];
    std::memcpy(out, charDirectDir, 12);
    expect(out[0] == 1.0f && out[1] == 2.0f && out[2] == -3.0f, "z negated after the call");
    if (failures) return 1;
    std::printf("lightSetPrimaryDirectional contract passed\n");
    return 0;
}
