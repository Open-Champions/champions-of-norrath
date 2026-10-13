// Host contract for doUserZoom__6Camera. Not an EE plan input.
#include "do_user_zoom.cpp"

#include <cstdio>
#include <cmath>
#include <cstring>

unsigned char pad[88 * 3];
int fadeTimer[7 * 3];
int fadeIconY[7 * 3];
unsigned automapButton[3];
int gameControlPaused;

static double asDouble(Double v) { double d; std::memcpy(&d, &v, 8); return d; }
static Double fromDouble(double d) { Double v; std::memcpy(&v, &d, 8); return v; }
Double fptodp(float v) { return fromDouble(v); }
int dpcmp(Double a, Double b) { double x = asDouble(a), y = asDouble(b); return x < y ? -1 : x > y ? 1 : 0; }
Double dpsub(Double a, Double b) { return fromDouble(asDouble(a) - asDouble(b)); }

static int blurCalls, clampResult, hudCalls, hudA, hudB;
static float hermiteArgs[5];
float evalHermite__Ffffff(float t, float a, float b, float c, float d) {
    hermiteArgs[0] = t; hermiteArgs[1] = a; hermiteArgs[2] = b; hermiteArgs[3] = c; hermiteArgs[4] = d;
    return 0.25f;
}
void engineSetBlur__Fi(int v) { ++blurCalls; if (v != 100) std::fprintf(stderr, "blur argument\n"); }
int clampZoomMag__6Camera(void*) { return clampResult; }
int hudGetFadeIconPos__Fii(int a, int b) { ++hudCalls; hudA = a; hudB = b; return 4000 + a; }

static int failures = 0;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }

static unsigned char camera[512];
static void setw(int off, int v) { std::memcpy(camera + off, &v, 4); }
static void setf(int off, float v) { std::memcpy(camera + off, &v, 4); }
static int getw(int off) { int v; std::memcpy(&v, camera + off, 4); return v; }
static float getf(int off) { float v; std::memcpy(&v, camera + off, 4); return v; }

static void reset(float stickY) {
    std::memset(camera, 0, sizeof camera);
    std::memset(pad, 0, sizeof pad);
    std::memset(fadeTimer, 0, sizeof fadeTimer);
    std::memset(fadeIconY, 0, sizeof fadeIconY);
    std::memset(automapButton, 0, sizeof automapButton);
    gameControlPaused = 0; blurCalls = 0; clampResult = 0; hudCalls = 0;
    setw(64, 1);
    std::memcpy(pad + 88 + 32, &stickY, 4);
    setf(176, 100.0f); setf(180, 200.0f); setf(184, 300.0f);
    setf(188, 10.0f); setf(192, 20.0f); setf(196, 30.0f);
}

int main() {
    // Blend after a zoom-index change in the held-button path.
    reset(0.0f);
    setw(80, 0); camera[352] = 1; setw(72, 1); setf(212, 0.35f); setw(92, 0);
    doUserZoom__6Camera(camera);
    const float target = 1.0f - 2.0f * (0.35f - 0.1f);
    expect(getw(84) == 1 && getw(88) == 2, "indices set");
    expect(std::fabs(getf(144) - target) < 1e-5f && getf(148) == getf(144), "held path sets F(144) directly");
    const float m = getf(144);
    const float z = 300.0f + (200.0f - 300.0f) * m;
    expect(getf(140) == z, "blend z");
    expect(getf(216) == 1.0f - (z - 100.0f) / 200.0f, "blend +216");
    expect(getf(252) == 30.0f + (20.0f - 30.0f) * m, "blend +252");
    expect(getw(92) == 1 && hudCalls == 0, "unpressed sets 92 and no HUD");

    // Same path eases when nothing holds it.
    reset(0.0f); camera[352] = 1; setf(212, 0.35f); setf(144, 0.0f);
    doUserZoom__6Camera(camera);
    expect(std::fabs(getf(144) - target * 0.05f) < 1e-5f, "eases by 0.05");
    // Paused with nothing held: returns before touching anything.
    reset(0.0f); camera[352] = 1; gameControlPaused = 1; setw(92, 7);
    doUserZoom__6Camera(camera);
    expect(getw(92) == 7 && getw(84) == 0, "early return keeps 92");
    reset(0.0f); camera[352] = 1; gameControlPaused = 1; camera[365] = 1; setw(92, 7);
    doUserZoom__6Camera(camera);
    expect(getw(92) == 1 && getw(88) == 2, "paused but B(365) set runs");

    // Stick pressed with W(80) set: only the HUD store and 92 = 0.
    reset(-1.0f); setw(80, 1); setw(64, 1); setw(92, 1);
    doUserZoom__6Camera(camera);
    expect(fadeTimer[7] == 120 && fadeIconY[7] == 4001 && hudA == 1 && hudB == 0 && getw(92) == 0, "HUD store");
    reset(0.9f); setw(80, 1); doUserZoom__6Camera(camera);
    expect(hudCalls == 0 && getw(92) == 1, "0.9 is not above the limit");
    reset(0.91f); setw(80, 1); doUserZoom__6Camera(camera);
    expect(hudCalls == 1, "0.91 is");

    // Hermite path.
    reset(0.0f); setf(156, 0.5f); setf(160, 0.25f); setf(144, 0.0f);
    doUserZoom__6Camera(camera);
    expect(blurCalls == 1 && hermiteArgs[0] == 0.75f && hermiteArgs[1] == 0 && hermiteArgs[2] == 1 && hermiteArgs[3] == 0.5f && hermiteArgs[4] == 0.5f, "hermite arguments");
    expect(getf(160) == 0.75f && getf(144) == 0.25f && getf(148) == 0.25f && getf(156) == 0.5f, "hermite stores");
    reset(0.0f); setf(156, 0.5f); setf(144, 0.5f); clampResult = 1;
    doUserZoom__6Camera(camera);
    expect(blurCalls == 0 && getf(156) == 0.0f, "clamp clears F(156)");

    // Step zoom in and out.
    reset(0.95f); setw(96, 1); setw(92, 1); setw(84, 0);
    doUserZoom__6Camera(camera);
    expect(getw(84) == 1 && getw(88) == 0 && getf(156) == 0x1.47ae14p-5f && getf(144) == 0 && getf(160) == 0 && hudCalls == 0 && getw(92) == 0, "zoom in");
    reset(-0.95f); setw(96, 1); setw(92, 1); setw(84, 2);
    doUserZoom__6Camera(camera);
    expect(getw(84) == 1 && getw(88) == 2 && getf(156) == 0x1.47ae14p-5f && hudCalls == 0, "zoom out");
    reset(-0.95f); setw(96, 1); setw(92, 1); setw(84, 0);
    doUserZoom__6Camera(camera);
    expect(getw(84) == 0 && hudCalls == 1, "cannot zoom out at 0: HUD instead");
    reset(0.95f); setw(96, 1); setw(92, 1); setw(84, 2);
    doUserZoom__6Camera(camera);
    expect(getw(84) == 2 && hudCalls == 1, "cannot zoom in at 2 with the stick up: HUD instead");
    reset(0.95f); setw(96, 1); setw(92, 0); setw(84, 0);
    doUserZoom__6Camera(camera);
    expect(getw(84) == 0 && hudCalls == 0 && getw(92) == 0, "92 == 0 blocks stepping");
    reset(0.95f); setw(96, 1); setw(92, 1); setw(84, 0); automapButton[1] = 4; std::memcpy(pad + 88 + 8, &(const int&)4, 4);
    doUserZoom__6Camera(camera);
    expect(getw(84) == 0 && getw(92) == 0, "automap button masks the step");

    // Tween toward F(148) when F(200) is nonzero.
    reset(0.0f); setw(96, 1); setf(148, 0.5f); setf(200, 0.5f); setw(84, 1); setw(88, 2);
    doUserZoom__6Camera(camera);
    expect(getf(144) == 0.5f + (1.0f - 0.5f) * (1.0f - 0.5f) && getw(92) == 1, "tween");
    return failures ? 1 : 0;
}
