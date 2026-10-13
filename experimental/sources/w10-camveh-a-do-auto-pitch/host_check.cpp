// Host contract for doAutoPitch__6Camera. Not an EE plan input.
#include "do_auto_pitch.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

signed char netNetworkMode;
int nmPlayersAlive, localNmPlayersAlive;
int localPlayersAlive[4];
void* world = &world;
float worldCameraPos[3];
unsigned char player[23984 * 4];

static double asDouble(Double v) { double d; std::memcpy(&d, &v, 8); return d; }
static Double fromDouble(double d) { Double v; std::memcpy(&v, &d, 8); return v; }
Double fptodp(float v) { return fromDouble(v); }
Double litodp(int v) { return fromDouble(v); }
Double dpmul(Double a, Double b) { return fromDouble(asDouble(a) * asDouble(b)); }
Double dpadd(Double a, Double b) { return fromDouble(asDouble(a) + asDouble(b)); }
float dptofp(Double a) { return static_cast<float>(asDouble(a)); }

static int atanCalls; static float atanArgs[2]; static int atanResult;
int iatan2__Fff(float a, float b) { ++atanCalls; atanArgs[0] = a; atanArgs[1] = b; return atanResult; }

struct RotCall { int axis; int angle; };
static std::vector<RotCall> rots;
static int idents;
void matIdent__FP8Matrix33(Matrix33*) { ++idents; }
void matRot__FP8Matrix33is(Matrix33*, int axis, short angle) { rots.push_back({axis, angle}); }
void* matUntransform__FP6Point3P8Matrix33T0(float* out, Matrix33*, float* in) {
    for (int i = 0; i < 3; ++i) out[i] = in[i] + 10.0f;
    return out;
}
struct Hit { float origin[3], dir[3], range; int mode; };
static std::vector<Hit> hits;
static float hitResult[8]; static int hitIndex;
float worldHitscan__FP12_worldHeaderG6Point3T1fi(void*, float* o, float* d, float range, int mode) {
    Hit h; std::memcpy(h.origin, o, 12); std::memcpy(h.dir, d, 12); h.range = range; h.mode = mode;
    hits.push_back(h);
    return hitResult[hitIndex++ & 7];
}

static int failures = 0;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }

static unsigned char camera[512];
static unsigned char targets[4][64];
static void setw(int off, int v) { std::memcpy(camera + off, &v, 4); }
static void setf(int off, float v) { std::memcpy(camera + off, &v, 4); }
static void seth(int off, int v) { short s = static_cast<short>(v); std::memcpy(camera + off, &s, 2); }
static int getw(int off) { int v; std::memcpy(&v, camera + off, 4); return v; }
static float getf(int off) { float v; std::memcpy(&v, camera + off, 4); return v; }
static int geth(int off) { short v; std::memcpy(&v, camera + off, 2); return v; }

static void reset() {
    std::memset(camera, 0, sizeof camera);
    std::memset(player, 0, sizeof player);
    netNetworkMode = 0; nmPlayersAlive = 0; localNmPlayersAlive = 1; localPlayersAlive[0] = 2;
    worldCameraPos[0] = 10; worldCameraPos[1] = 20; worldCameraPos[2] = 30;
    atanCalls = 0; atanResult = 1000; rots.clear(); idents = 0; hits.clear(); hitIndex = 0;
    for (int i = 0; i < 8; ++i) hitResult[i] = 0.0f;
    for (int k = 0; k < 4; ++k) {
        std::memset(targets[k], 0, 64);
        const float pos[3] = {1.0f + k, 2.0f + k, 3.0f + k};
        std::memcpy(targets[k] + 16, pos, 12);
        unsigned char* p = targets[k];
        std::memcpy(player + 23984 * k + 21872, &p, sizeof p);
    }
    setf(180, 55.0f); setf(216, 0.5f);
    for (int k = 0; k < 6; ++k) seth(126 + 2 * k, 100 * k);
    seth(108, 8000); seth(110, 10000); seth(112, 13000);
    setf(220, 150.0f); setf(224, 40.0f); setf(228, 20.0f); setf(232, -20.0f); setf(236, -25.0f); setf(240, -25.0f);
    setf(24, 4.0f); setf(28, 6.0f);
}

int main() {
    // Empty lists return before any work.
    reset(); localNmPlayersAlive = 0; setw(76, 5); doAutoPitch__6Camera(camera);
    expect(atanCalls == 0 && getw(76) == 5, "empty list returns");
    reset(); localNmPlayersAlive = 0; netNetworkMode = 1; nmPlayersAlive = 2; camera[353] = 0xff; doAutoPitch__6Camera(camera);
    expect(atanCalls == 0, "network single slot -1 returns");
    reset(); localNmPlayersAlive = 0; netNetworkMode = 1; nmPlayersAlive = 2; camera[353] = 3; doAutoPitch__6Camera(camera);
    expect(atanCalls == 1 && hits.size() == 5, "network single slot 3 runs");
    reset(); { unsigned char* none = nullptr; std::memcpy(player + 23984 * 2 + 21872, &none, sizeof none); }
    doAutoPitch__6Camera(camera);
    expect(atanCalls == 0, "null target returns");

    // One sweep pass, free-camera mode (B(352) == 0): the camera's own xy is the origin of the aim vector.
    reset(); setw(76, 6);              // sub = 2, slot = (6 % 4) / 4 = 1... period is 4, so slot 0
    hitResult[1] = 3.0f; hitResult[3] = 0.1f;
    doAutoPitch__6Camera(camera);
    expect(getw(76) == 7, "counter advances once");
    expect(atanCalls == 1, "one atan");
    const float dx = 10.0f - 4.0f, dy = 20.0f - 6.0f, inv = 1.0f / std::sqrt(dx * dx + dy * dy);
    expect(std::fabs(atanArgs[0] - dx * inv) < 1e-6f && std::fabs(atanArgs[1] - dy * inv) < 1e-6f, "normalised aim vector");
    expect(idents == 5 && rots.size() == 10 && hits.size() == 5, "five rays");
    const float lim = 0.5f * 6000.0f + 4000.0f;
    const int base = static_cast<short>(1000 + static_cast<int>(lim * 0.5f) - static_cast<int>(2.0f * (lim * 0.25f)));
    expect(rots[0].axis == 0 && rots[0].angle == 100 + 16339 && rots[1].axis == 2 && rots[1].angle == base, "rotations");
    expect(rots[8].angle == 500 + 16339, "fifth ray uses the fifth table halfword");
    expect(hits[0].origin[0] == 3.0f && hits[0].origin[2] == 5.0f + 40.0f && hits[0].dir[1] == 9.0f && hits[0].range == 55.0f && hits[0].mode == 0, "ray arguments");
    expect(camera[256 + 24 * 2 + 12 + 1] == 0 && camera[256 + 24 * 2 + 12 + 2] == 1 && camera[256 + 24 * 2 + 12 + 4] == 1, "hit bytes at 24*player + 6*sub + k");

    // Close mode picks the target position.
    reset(); camera[352] = 1; setw(76, 0); doAutoPitch__6Camera(camera);
    const float cdx = 10.0f - 3.0f, cdy = 20.0f - 4.0f, cinv = 1.0f / std::sqrt(cdx * cdx + cdy * cdy);
    expect(std::fabs(atanArgs[0] - cdx * cinv) < 1e-6f, "close mode aims from the target position");

    // First blocked index selects the table angle (free mode: index first - 1).
    reset(); camera[256 + 24 * 2 + 6 + 3] = 1; setf(156, 0); seth(106, 0);
    doAutoPitch__6Camera(camera);
    expect(geth(114) == 16383 - 200, "H(114) from the table: first = 3 gives idx 2");
    reset(); camera[352] = 1; camera[256 + 24 * 2 + 6 + 3] = 1; doAutoPitch__6Camera(camera);
    expect(geth(114) == 16383 - 100, "close mode takes two off");
    reset(); doAutoPitch__6Camera(camera);
    expect(geth(114) == 16383 - 500, "nothing blocked: first = 6 gives idx 5");

    // Locked follow.
    reset(); setf(156, 1.0f); seth(116, 100); seth(114, 300); seth(108, 50); setw(84, 0); setf(144, 0.5f); setf(152, 9.0f);
    doAutoPitch__6Camera(camera);
    expect(atanCalls == 1 && getf(152) == 0.0f && geth(106) == 100 + 100, "F(156) != 0: H(106) eases toward the larger target");
    expect(geth(116) == 100 && geth(114) == 300, "locked path leaves H(116) and H(114)");

    // Direct set when W(72) != 0.
    reset(); setw(72, 1); doAutoPitch__6Camera(camera);
    expect(getf(152) == 0.0f && geth(116) == geth(106) && geth(106) == 16383 - 500 && getw(76) >= 4, "W(72) forces H(106) to the target; sweeps all four slots");

    // Easing. Free mode, target above current.
    reset(); seth(106, 5000); setf(152, 0.0f); setw(84, 1); seth(110, 0);   // target = max(16383-500, 0)
    doAutoPitch__6Camera(camera);
    expect(geth(106) > 5000 && geth(106) == geth(116), "free mode moves toward the target");
    reset(); seth(106, 15883); doAutoPitch__6Camera(camera);
    expect(geth(106) == 15883 && getf(152) == 0.0f, "no movement at the target");
    reset(); seth(106, 15883 + 8000); setw(84, 0); doAutoPitch__6Camera(camera);
    expect(geth(106) < 15883 + 8000 && getf(152) < 0.0f, "moves down when above");
    // Limit window of F(152).
    reset(); seth(106, 0); setf(152, 1000.0f); setf(216, 0.0f); doAutoPitch__6Camera(camera);
    expect(getf(152) <= 150.0f && getf(152) >= -20.0f, "free mode caps F(152) to the window at 220/232");
    reset(); camera[352] = 1; seth(106, 0); setf(152, 1000.0f); setf(212, 0.0f); doAutoPitch__6Camera(camera);
    expect(getf(152) <= 35.0f && getf(152) > 0.0f, "close mode caps F(152) at 35.0");
    reset(); camera[352] = 1; seth(106, 0); setf(152, 1000.0f); setf(212, 1.0f); doAutoPitch__6Camera(camera);
    expect(getf(152) == 0.0f, "close mode: F(212)^3 == 1 kills the speed");

    // Do-while: with B(365) set the sweep continues until the counter reaches 4 * count.
    reset(); camera[365] = 1; setw(76, 1); doAutoPitch__6Camera(camera);
    expect(getw(76) == 4 && atanCalls == 3, "B(365) repeats passes until 4 * count");
    reset(); localNmPlayersAlive = 2; localPlayersAlive[1] = 0; camera[365] = 1; doAutoPitch__6Camera(camera);
    expect(getw(76) == 8 && atanCalls == 8, "two players need eight passes");
    reset(); doAutoPitch__6Camera(camera);
    expect(getw(76) == 1 && atanCalls == 1, "single pass otherwise");
    return failures ? 1 : 0;
}
