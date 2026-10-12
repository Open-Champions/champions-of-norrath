// Host contract for gameCameraTask. Not an EE plan input.
#include "camera_task.cpp"

#include <cstdio>

extern "C" unsigned char* gameCam = 0;
extern "C" unsigned char netNetworkMode = 0;

static int failures = 0;
static int updates = 0;
static unsigned char* updated = 0;
static unsigned char camera_bytes[4];

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" void Update__6Camera(unsigned char* camera) {
    ++updates;
    updated = camera;
    gameCam = 0;
    netNetworkMode = 1;
}

int main() {
    gameCam = 0;
    netNetworkMode = 0;
    updates = 0;
    gameCameraTask__Fv();
    expect(updates == 0, "a null camera does not update");

    gameCam = camera_bytes;
    netNetworkMode = 1;
    gameCameraTask__Fv();
    expect(updates == 0, "network mode skips the update");

    netNetworkMode = 0x80;
    gameCameraTask__Fv();
    expect(updates == 0, "any nonzero network byte skips the update");

    netNetworkMode = 0;
    gameCameraTask__Fv();
    expect(updates == 1 && updated == camera_bytes, "offline camera is updated once");
    expect(gameCam == 0, "the call keeps the pointer captured before Update");

    if (failures != 0) return 1;
    std::puts("gameCameraTask contract passed");
    return 0;
}
