// Host contract for Push__6Camera. Not an EE plan input.
#include "push.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static int order[8];
static int steps = 0;
static int mode_seen = -1;
static void* mode_camera;
static int zoom_before_mode = 0;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

unsigned char worldCameraPos[12];

extern "C" short GetYaw__6Camera(void*) { order[steps++] = 1; return static_cast<short>(-12345); }
extern "C" short GetPitch__6Camera(void*) { order[steps++] = 2; return static_cast<short>(777); }
extern "C" float GetZoomDistance__6Camera(void*) { order[steps++] = 3; return 6.25f; }
extern "C" void SetMode__6Camerai(void* camera, int mode) {
    order[steps++] = 4;
    mode_seen = mode;
    mode_camera = camera;
}

int main() {
    static unsigned char camera[512];
    static unsigned char before[512];
    std::memset(camera, 0x5a, sizeof camera);
    const float pos[3] = {1.0f, -2.0f, 3.5f};
    std::memcpy(worldCameraPos, pos, 12);
    std::memcpy(before, camera, sizeof camera);
    Push__6Camera(camera);
    short yaw, pitch;
    float zoom, saved[3];
    std::memcpy(&yaw, camera + 354, 2);
    std::memcpy(&pitch, camera + 356, 2);
    std::memcpy(&zoom, camera + 360, 4);
    std::memcpy(saved, camera + 368, 12);
    expect(yaw == -12345, "yaw at +354");
    expect(pitch == 777, "pitch at +356");
    expect(zoom == 6.25f, "zoom distance at +360");
    expect(saved[0] == 1.0f && saved[1] == -2.0f && saved[2] == 3.5f, "camera position at +368..+376");
    expect(mode_seen == 0 && mode_camera == camera, "mode 0 set on this camera");
    expect(steps == 4 && order[3] == 4, "SetMode is the last call");
    std::memcpy(before + 354, camera + 354, 4);
    std::memcpy(before + 360, camera + 360, 4);
    std::memcpy(before + 368, camera + 368, 12);
    expect(std::memcmp(before, camera, sizeof camera) == 0, "no other camera byte changes");
    return failures ? 1 : 0;
}
