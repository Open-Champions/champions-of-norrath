// Host contract for StartDialogSwoop__6CameraP10GameObjecti. Not an EE plan input.
// icos/isin use a 65536-step turn. Float results are compared with a small tolerance.
#define NORRATH_HOST_CONTRACT 1
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
static unsigned char arena[1024] __attribute__((aligned(16)));
unsigned char* norrath_host_base = reinterpret_cast<unsigned char*>(reinterpret_cast<std::uintptr_t>(arena) & ~std::uintptr_t(0xffffffffu));
#include "start_swoop.cpp"
extern "C" {
unsigned char player[4 * 23984 + 64] __attribute__((aligned(16)));
unsigned char worldCameraPos[12];
int nmPlayersAlive;
int playersAlive[4];
}
static const char* tagText; static int tagCalls; static std::string tagName; static int tagIndex; static void* tagObject;
static float planeResult; static int planeCalls; static void *planePoint, *planeNormal, *planeOrigin;
extern "C" {
char* findTagString__10GameObjectPci(void* o, char* n, int i) { ++tagCalls; tagObject = o; tagName = n; tagIndex = i; return const_cast<char*>(tagText); }
float icos__Fi(int a) { return static_cast<float>(std::cos(static_cast<short>(a) * 6.283185307179586 / 65536.0)); }
float isin__Fi(int a) { return static_cast<float>(std::sin(static_cast<short>(a) * 6.283185307179586 / 65536.0)); }
float distanceToPlane__FP6Point3N20(void* p, void* n, void* o) { ++planeCalls; planePoint = p; planeNormal = n; planeOrigin = o; return planeResult; }
}
alignas(8) static unsigned char cam[256];
alignas(8) static unsigned char obj[64];
alignas(8) static unsigned char creatures[4][64];
static float F(const unsigned char* b, int off) { float v; std::memcpy(&v, b + off, 4); return v; }
static void setF(unsigned char* b, int off, float v) { std::memcpy(b + off, &v, 4); }
static short H(const unsigned char* b, int off) { short v; std::memcpy(&v, b + off, 2); return v; }
static void setH(unsigned char* b, int off, short v) { std::memcpy(b + off, &v, 2); }
static bool near(float a, float b, float tol = 1e-3f) { return std::fabs(a - b) <= tol; }
static void reset(short yaw) {
    std::memset(cam, 0, sizeof cam); std::memset(obj, 0, sizeof obj); std::memset(creatures, 0, sizeof creatures);
    std::memset(player, 0, sizeof player);
    setF(obj, 16, 100.0f); setF(obj, 20, 200.0f); setF(obj, 24, 300.0f); setH(obj, 56, yaw);
    setF(worldCameraPos, 0, 150.0f); setF(worldCameraPos, 4, 190.0f); setF(worldCameraPos, 8, 310.0f);
    tagText = nullptr; tagCalls = planeCalls = 0; nmPlayersAlive = 0; planeResult = 1.0f;
    setH(cam, 104, 0x1000); setH(cam, 106, 0x2000);
    setF(cam, 168, 0.75f); setF(cam, 172, 5.0f);
}
static void addPlayer(int slot, int index, float dx, float dy, float dz) {
    unsigned char* c = creatures[slot];
    setF(c, 16, 100.0f + dx); setF(c, 20, 200.0f + dy); setF(c, 24, 300.0f + dz);
    unsigned w = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(c));
    std::memcpy(player + index * 23984 + 21872, &w, 4);
    playersAlive[slot] = index;
    if (nmPlayersAlive < slot + 1) nmPlayersAlive = slot + 1;
}
#define CHECK(c) do { if (!(c)) { std::fprintf(stderr, "line %d\n", __LINE__); return __LINE__; } } while (0)
int main() {
    // Mode 0, no tag, yaw 0: dir = (1, 0, 0); end = pos + (80, 0, 70).
    reset(0);
    StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 0);
    CHECK(tagCalls == 1 && tagObject == obj && tagName == "dialogOffset" && tagIndex == 0);
    CHECK(near(F(cam, 12), 180.0f) && near(F(cam, 16), 200.0f) && near(F(cam, 20), 370.0f));
    CHECK(H(cam, 122) == 4000 && H(cam, 120) == -16384);
    CHECK(near(F(cam, 0), 150.0f) && near(F(cam, 4), 190.0f) && near(F(cam, 8), 310.0f));
    CHECK(H(cam, 124) == 0x2000 && H(cam, 118) == 0x1000);
    CHECK(F(cam, 168) == 0.0f);
    // rate = 1 - (cos(0x1000 - h120) + 1)/2 with h120 = -16384, h118 = 4096: angle diff 20480 = 112.5 deg.
    { double ang = (4096.0 - (-16384.0)) * 6.283185307179586 / 65536.0; CHECK(near(F(cam, 172), static_cast<float>(1.0 - (std::cos(ang) + 1.0) * 0.5))); }
    CHECK(planeCalls == 0);
    // Tag "5,6,7,8": a=5, b=6, dz=7, e=8. Yaw 0: w = (0, 1, 0).
    reset(0); tagText = "5,6,7,8";
    StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 0);
    CHECK(near(F(cam, 12), 185.0f) && near(F(cam, 16), 207.0f) && near(F(cam, 20), 376.0f));
    CHECK(H(cam, 122) == 4008);
    // Yaw 16384 (quarter turn): dir = (0, 1, 0), w = (-1, 0, 0); h120 = -(16384+16384) wraps.
    reset(16384); tagText = "0,0,10,0";
    StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 0);
    CHECK(near(F(cam, 12), 90.0f) && near(F(cam, 16), 280.0f) && near(F(cam, 20), 370.0f));
    CHECK(H(cam, 120) == static_cast<short>(-(16384 + 16384)));
    // Far distance override: start (worldCameraPos) is farther than 354 from the end -> 0.85.
    reset(0); setF(worldCameraPos, 0, 1000.0f);
    StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 0);
    CHECK(F(cam, 172) == bits(0x3f599999));
    // Mode 1, nobody nearby: lane 1, side 0 -> same as mode 0.
    reset(0);
    StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 1);
    CHECK(near(F(cam, 12), 180.0f) && H(cam, 120) == -16384 && planeCalls == 0);
    // Facing for mode 1 at yaw 0: d0 = (cos 16384, sin 16834, 0) ~ +y. Players: ahead (+y), middle (+x), behind (-y).
    struct Case { int ahead, middle, behind; int side; };
    const Case cases[] = {{0, 0, 0, 0}, {0, 0, 1, 6000}, {1, 0, 0, -6000}, {0, 1, 1, 6000}, {1, 1, 0, -6000},
                          {1, 0, 1, 0}, {1, 1, 1, 0}};
    for (const Case& c : cases) {
        reset(0); int n = 0;
        if (c.ahead) addPlayer(n++, 0, 0.0f, 20.0f, 0.0f);
        if (c.middle) addPlayer(n++, 1, 20.0f, 0.0f, 0.0f);
        if (c.behind) addPlayer(n++, 2, 0.0f, -20.0f, 0.0f);
        StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 1);
        CHECK(planeCalls == 0);
        CHECK(H(cam, 120) == static_cast<short>(-(0 + c.side + 16384)));
    }
    // Middle only: the plane test decides. Non-negative -> lane 0 (+6000); negative -> lane 2 (-6000).
    reset(0); addPlayer(0, 1, 20.0f, 0.0f, 0.0f); planeResult = 0.0f;
    StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 1);
    CHECK(planeCalls == 1 && planePoint == obj + 16 && planeNormal != nullptr && planeOrigin == worldCameraPos);
    CHECK(H(cam, 120) == static_cast<short>(-(6000 + 16384)));
    reset(0); addPlayer(0, 1, 20.0f, 0.0f, 0.0f); planeResult = -0.5f;
    StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 1);
    CHECK(planeCalls == 1 && H(cam, 120) == static_cast<short>(-(-6000 + 16384)));
    // Players at or beyond 80 units are ignored; 3-D distance counts.
    reset(0); addPlayer(0, 0, 0.0f, 79.0f, 40.0f);
    StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 1);
    CHECK(H(cam, 120) == -16384);
    reset(0); addPlayer(0, 0, 0.0f, 79.0f, 0.0f);   // under 80: ahead only -> -6000 side
    StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 1);
    CHECK(H(cam, 120) == static_cast<short>(-(-6000 + 16384)));
    // Dot thresholds: a direction at about 0.3 against d0 is ahead; about 0.1 is middle.
    reset(0); addPlayer(0, 0, 20.0f * std::sqrt(1 - 0.3f * 0.3f), 20.0f * 0.3f, 0.0f);
    StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 1);
    CHECK(H(cam, 120) == static_cast<short>(-(-6000 + 16384)));
    reset(0); addPlayer(0, 0, 20.0f * std::sqrt(1 - 0.1f * 0.1f), 20.0f * 0.1f, 0.0f); planeResult = 1.0f;
    StartDialogSwoop__6CameraP10GameObjecti(cam, obj, 1);
    CHECK(planeCalls == 1 && H(cam, 120) == static_cast<short>(-(6000 + 16384)));
    return 0;
}
