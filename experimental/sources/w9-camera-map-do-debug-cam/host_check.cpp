// Host contract for doDebugCam__6Camera. Not an EE plan input.
#include "do_debug_cam.cpp"

#include <cmath>
#include <initializer_list>
#include <cstdio>
#include <cstring>

unsigned char pad[352];
float worldCameraMatrix[12];
float worldCameraPos[3];

static int build_n;
static void* build_camera;
static void* build_point;
static float build_args[3];
extern "C" void BuildMatrix__6CamerafffP6Point3(void* c, float a, float b, float d, void* p) {
    ++build_n; build_camera = c; build_point = p;
    build_args[0] = a; build_args[1] = b; build_args[2] = d;
}

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static bool near(double a, double b) { return std::fabs(a - b) <= 1e-3 * (1.0 + std::fabs(b)); }
static void setf(int off, float v) { std::memcpy(pad + off, &v, 4); }
static void setw(int off, int v) { std::memcpy(pad + off, &v, 4); }
static int rnd(double x) { return int(x >= 0 ? x + 0.5 : x - 0.5); }

int main() {
    const float stick[] = {-0.75f, -0.25f, 0.0f, 0.5f, 1.0f};
    const int heldbits[] = {0, 0x400, 0x200, 0x600};
    const int presses[] = {0, 0x200, 0x100};
    const int modes[] = {3, 4, 5, 2, 7};
    const short yaws[] = {0, 1000, -2000};
    int cases = 0;
    for (int mode : modes)
    for (int held : heldbits)
    for (int pressed : presses)
    for (float s20 : stick)
    for (float s24 : stick)
    for (float s28 : {-0.5f, 0.25f, 1.0f})
    for (float s32 : {-1.0f, 0.5f})
    for (short yaw : yaws) {
        std::memset(pad, 0, sizeof pad);
        setw(8, held); setw(12, pressed);
        setf(20, s20); setf(24, s24); setf(28, s28); setf(32, s32);
        for (int i = 0; i < 12; ++i) worldCameraMatrix[i] = 0.5f + 0.25f * float((i * 5) % 7);
        worldCameraPos[0] = 10.0f; worldCameraPos[1] = -20.0f; worldCameraPos[2] = 33.0f;
        unsigned char camera[200];
        std::memset(camera, 0x21, sizeof camera);
        std::memcpy(camera + 52, &mode, 4);
        std::memcpy(camera + 104, &yaw, 2);
        short pitch = 500;
        std::memcpy(camera + 106, &pitch, 2);
        unsigned char before[200];
        std::memcpy(before, camera, sizeof camera);
        build_n = 0;
        doDebugCam__6Camera(camera);
        ++cases;
        if ((pressed & 0x200) && (held & 0x400)) {
            unsigned char want[200];
            std::memcpy(want, before, sizeof want);
            int one = 1, zero = 0;
            std::memcpy(want + 52, &one, 4); std::memcpy(want + 56, &zero, 4);
            std::memcpy(want + 60, &zero, 4); std::memcpy(want + 152, &zero, 4);
            expect(build_n == 0, "reset path builds nothing");
            expect(std::memcmp(want, camera, sizeof want) == 0, "reset path stores mode 1 and clears three words");
            continue;
        }
        expect(build_n == 1 && build_camera == camera && build_point == nullptr, "one matrix build with null point");
        double v[3];
        double ny, np;
        const double M[12] = {worldCameraMatrix[0], worldCameraMatrix[1], worldCameraMatrix[2], worldCameraMatrix[3],
                              worldCameraMatrix[4], worldCameraMatrix[5], worldCameraMatrix[6], worldCameraMatrix[7],
                              worldCameraMatrix[8], worldCameraMatrix[9], worldCameraMatrix[10], worldCameraMatrix[11]};
        if (mode == 3) {
            ny = rnd(yaw + s28 * 400.0); np = rnd(pitch + s32 * 400.0);
            if (held & 0x400) { v[0] = 0; v[1] = 0; v[2] = s24 * -10.0; }
            else {
                const double al = std::sqrt(M[2] * M[2] + M[5] * M[5]), bl = std::sqrt(M[0] * M[0] + M[3] * M[3]);
                v[0] = 10.0 * (M[0] / bl * s20 - M[2] / al * s24);
                v[1] = 10.0 * (M[3] / bl * s20 - M[5] / al * s24);
                v[2] = 0.0;
            }
        } else {
            if (mode == 4) { np = rnd(pitch - s32 * 180.0 * 0.75); ny = rnd(yaw + s28 * 180.0); }
            else { np = rnd(pitch + s32 * 180.0); ny = rnd(yaw + s28 * 180.0 * 1.25); }
            const int c[3] = {2, 5, 8}, d[3] = {0, 3, 6};
            for (int i = 0; i < 3; ++i) v[i] = -M[c[i]] * s24 * 12.0 + M[d[i]] * 0.5 * s20 * 12.0;
        }
        short gy, gp;
        std::memcpy(&gy, camera + 104, 2); std::memcpy(&gp, camera + 106, 2);
        expect(gy == (short)ny, "yaw halfword");
        expect(gp == (short)np, "pitch halfword");
        expect(near(build_args[0], 10.0 + v[0]) && near(build_args[1], -20.0 + v[1]) && near(build_args[2], 33.0 + v[2]), "build target");
        unsigned char want[200];
        std::memcpy(want, before, sizeof want);
        std::memcpy(want + 104, camera + 104, 4);
        expect(std::memcmp(want, camera, sizeof want) == 0, "only the two halfwords change");
    }
    expect(cases == 5 * 4 * 3 * 5 * 5 * 3 * 2 * 3, "all cases ran");
    return failures ? 1 : 0;
}
