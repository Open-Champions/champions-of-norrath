#include <cstring>
struct Point3 { float x, y, z; };
extern "C" Point3* GetLastTarget__6Camera(Point3* result, const unsigned char* camera);
int main() {
    unsigned char cam[512] = {};
    const float v[3] = {1.5f, -2.0f, 300.25f};
    std::memcpy(cam + 24, v, 12);
    float w = 77.0f; std::memcpy(cam + 36, &w, 4);
    Point3 out = {9, 9, 9};
    Point3* r = GetLastTarget__6Camera(&out, cam);
    if (r != &out) return 1;
    if (out.x != 1.5f || out.y != -2.0f || out.z != 300.25f) return 2;
    return 0;
}
