// Host contract for Pop__6Camera. Not an EE plan input.
#include "pop.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

static int seq[8], count;
static void* seen_camera[8];
static int arg_i[8];
static float arg_f[8][3];
static void* arg_p[8];
enum { Mode = 1, Yaw, Pitch, Build, Zoom };

extern "C" {
void SetMode__6Camerai(void* c, int m) { seen_camera[count] = c; arg_i[count] = m; seq[count++] = Mode; }
void SetYaw__6Camerai(void* c, int y) { seen_camera[count] = c; arg_i[count] = y; seq[count++] = Yaw; }
void SetPitch__6Camerai(void* c, int p) { seen_camera[count] = c; arg_i[count] = p; seq[count++] = Pitch; }
void BuildMatrix__6CamerafffP6Point3(void* c, float a, float b, float d, void* p) {
    seen_camera[count] = c; arg_f[count][0] = a; arg_f[count][1] = b; arg_f[count][2] = d; arg_p[count] = p; seq[count++] = Build;
}
void SetZoomDistance__6Cameraf(void* c, float z) { seen_camera[count] = c; arg_f[count][0] = z; seq[count++] = Zoom; }
}

int main() {
    unsigned char camera[400];
    std::memset(camera, 0, sizeof camera);
    short yaw = -8192, pitch = -3;
    float zoom = 321.5f, x = 1.5f, y = -2.5f, z = 4.25f;
    std::memcpy(camera + 354, &yaw, 2);
    std::memcpy(camera + 356, &pitch, 2);
    std::memcpy(camera + 360, &zoom, 4);
    std::memcpy(camera + 368, &x, 4);
    std::memcpy(camera + 372, &y, 4);
    std::memcpy(camera + 376, &z, 4);
    unsigned char before[400];
    std::memcpy(before, camera, sizeof camera);
    Pop__6Camera(camera);
    expect(count == 5, "five calls");
    const int order[5] = {Mode, Yaw, Pitch, Build, Zoom};
    for (int i = 0; i < 5 && i < count; ++i) {
        expect(seq[i] == order[i], "call order");
        expect(seen_camera[i] == camera, "camera is passed first");
    }
    expect(arg_i[0] == 1, "mode 1");
    expect(arg_i[1] == -8192, "signed yaw");
    expect(arg_i[2] == -3, "signed pitch");
    expect(arg_f[3][0] == x && arg_f[3][1] == y && arg_f[3][2] == z, "position floats");
    expect(arg_p[3] == nullptr, "null point");
    expect(arg_f[4][0] == zoom, "zoom from +360");
    expect(std::memcmp(before, camera, sizeof camera) == 0, "slot is left in place");
    return failures ? 1 : 0;
}
