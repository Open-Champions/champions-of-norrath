// Host contract for lightSetSceneLights__FG6Point3iiiiiiiii. Not an EE plan input.
// Build with -ffp-contract=off.
#include <cmath>
#include <cstdio>
#include <cstring>
struct Point3 {
    float x, y, z;
};
extern "C" {
unsigned char scene[0x80];
static int order;
void transformSceneLightTask__Fv() { order = order * 10 + 1; }
void lightResetOptionalParams__Fv() { order = order * 10 + 2; }
void lightSetSceneLights__FG6Point3iiiiiiiii(Point3*, int, int, int, int, int, int, int, int, int);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Point3 d = {3.0f, 4.0f, 12.0f};
    lightSetSceneLights__FG6Point3iiiiiiiii(&d, 0x101, 0x202, 0x303, 0x404, 0x505, 0x606, 0x707, 0x808, 0x12345678);
    float v[3];
    std::memcpy(v, scene, 12);
    const float scale = 1.0f / std::sqrt((d.x * d.x + d.y * d.y) + d.z * d.z);
    expect(std::fabs(v[0] - d.x * scale) < 1e-6f && std::fabs(v[1] - d.y * scale) < 1e-6f &&
               std::fabs(v[2] - d.z * scale) < 1e-6f,
           "normalised direction");
    int w;
    std::memcpy(&w, scene + 52, 4);
    expect(w == 0x12345678, "word at 52");
    for (int i = 0; i < 7; ++i) {
        expect(scene[36 + i] == (i + 1) && scene[44 + i] == (i + 1), "seven bytes twice");
    }
    expect(scene[43] == 8 && scene[51] == 8, "eighth byte twice");
    expect(order == 12, "transform then reset");
    if (failures) return 1;
    std::printf("lightSetSceneLights contract passed\n");
    return 0;
}
