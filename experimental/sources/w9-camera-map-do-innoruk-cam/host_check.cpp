// Host contract for doInnorukCam__6Camera. Not an EE plan input.
#include "do_innoruk_cam.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>

float worldCameraPos[3];
float worldCameraMatrix[12];
int frameNm;

static unsigned char object[64];
static int find_calls, find_count_arg;
static std::string find_name;
static bool find_result;
static int icos_arg[4], isin_arg[4], icos_n, isin_n;
static int build_n;
static void* build_camera[4];
static float build_args[4][3];
static void* build_point[4];
static std::string order;

extern "C" {
void objectFindObject__FPcPP10GameObjecti(const char* name, void** out, int count) {
    ++find_calls; find_name = name; find_count_arg = count;
    *out = find_result ? object : nullptr;
}
float icos__Fi(int a) { icos_arg[icos_n++] = a; order += 'c'; return float(a % 1000) / 7.0f; }
float isin__Fi(int a) { isin_arg[isin_n++] = a; order += 's'; return float(a % 777) / 5.0f - 20.0f; }
void BuildMatrix__6CamerafffP6Point3(void* c, float a, float b, float d, void* p) {
    build_camera[build_n] = c; build_args[build_n][0] = a; build_args[build_n][1] = b; build_args[build_n][2] = d;
    build_point[build_n] = p; ++build_n; order += 'B';
}
}

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}

int main() {
    unsigned char camera[200];
    const int frames[] = {0, 1, 100, 819, 820, 65535, 65536, 70000, 1000000, 26843546};
    const short yaws[] = {0, 100, 32760, -5};
    for (int frame : frames) {
        for (short yaw : yaws) {
            std::memset(camera, 0x5a, sizeof camera);
            std::memcpy(camera + 104, &yaw, 2);
            worldCameraPos[0] = 100.0f; worldCameraPos[1] = -50.0f; worldCameraPos[2] = 12.5f;
            for (int i = 0; i < 12; ++i) worldCameraMatrix[i] = 0.25f * float(i + 1);
            const float ox = 130.0f, oy = -20.0f, oz = 40.0f;
            std::memcpy(object + 16, &ox, 4); std::memcpy(object + 20, &oy, 4); std::memcpy(object + 24, &oz, 4);
            frameNm = frame;
            find_calls = icos_n = isin_n = build_n = 0; order.clear(); find_result = true;
            unsigned char before[200];
            std::memcpy(before, camera, sizeof camera);
            doInnorukCam__6Camera(camera);
            expect(find_calls == 1 && find_name == "InnoruukB" && find_count_arg == 10, "object lookup");
            expect(order == "cBsB", "call order icos, build, isin, build");
            expect(icos_arg[0] == (frame * 80) % 65536 && isin_arg[0] == (frame * 81) % 65536, "angles");
            short got_yaw, got_pitch;
            std::memcpy(&got_yaw, camera + 104, 2);
            std::memcpy(&got_pitch, camera + 106, 2);
            expect(got_yaw == (short)(yaw + 33), "yaw advances by 33");
            const float pitch = float(icos_arg[0] % 1000) / 7.0f * 1447.0f + 8725.0f;
            expect(got_pitch == (short)(int)pitch, "pitch from icos");
            expect(build_camera[0] == camera && build_camera[1] == camera && !build_point[0] && !build_point[1], "camera first, null point");
            const float x = 100.0f + (ox - 100.0f), y = -50.0f + (oy + 50.0f), z = 12.5f + ((oz + 131.0f) - 12.5f);
            expect(build_args[0][0] == x && build_args[0][1] == y && build_args[0][2] == z, "first target");
            const float d = (float(isin_arg[0] % 777) / 5.0f - 20.0f) * 94.0f + 279.0f;
            expect(build_args[1][0] == x - d * worldCameraMatrix[2], "second x uses m[2]");
            expect(build_args[1][1] == y - d * worldCameraMatrix[5], "second y uses m[5]");
            expect(build_args[1][2] == z - d * worldCameraMatrix[8], "second z uses m[8]");
            unsigned char want[200];
            std::memcpy(want, before, sizeof want);
            std::memcpy(want + 104, camera + 104, 4);
            expect(std::memcmp(want, camera, sizeof want) == 0, "only +104 and +106 change");
        }
    }
    // Object missing: only the lookup happens.
    find_calls = icos_n = isin_n = build_n = 0; order.clear(); find_result = false;
    std::memset(camera, 0, sizeof camera);
    doInnorukCam__6Camera(camera);
    expect(find_calls == 1 && order.empty() && camera[104] == 0, "missing object does nothing");
    return failures ? 1 : 0;
}
