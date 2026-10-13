// Host contract for doUserYaw__6Camera. Not an EE plan input.
#include "do_user_yaw.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

unsigned levelFlags;
unsigned char pad[88 * 3];
unsigned char player[23984 * 3];
int fadeTimer[7 * 3];
int fadeIconY[7 * 3];
int localPlayersAlive[3];
int localNmPlayersAlive, gameCombatCamera, gameReverseCameraYaw, gameControlPaused;

static double asDouble(Double v) { double d; std::memcpy(&d, &v, 8); return d; }
static Double fromDouble(double d) { Double v; std::memcpy(&v, &d, 8); return v; }
Double fptodp(float v) { return fromDouble(v); }
int dpcmp(Double a, Double b) { double x = asDouble(a), y = asDouble(b); return x < y ? -1 : x > y ? 1 : 0; }
Double dpsub(Double a, Double b) { return fromDouble(asDouble(a) - asDouble(b)); }
float dptofp(Double a) { return static_cast<float>(asDouble(a)); }

static int rotDir, rotAmount, rotCalls, rotFrom, rotTo, hudCalls, localNumber;
int fastestRotDirection__FssPi(short from, short to, int* amount) {
    ++rotCalls; rotFrom = from; rotTo = to; *amount = rotAmount; return rotDir;
}
int netGetLocalPlayerNm__Fi(int code) { return code == 0 ? localNumber : -1; }
int hudGetFadeIconPos__Fii(int a, int b) { ++hudCalls; return 5000 + a + b; }
float yaw_powf(float b, float e) { return std::pow(b, e); }

static int failures = 0;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }

static unsigned char camera[512];
static unsigned char targets[3][64];
static void setw(int off, int v) { std::memcpy(camera + off, &v, 4); }
static void setf(int off, float v) { std::memcpy(camera + off, &v, 4); }
static void seth(int off, int v) { short s = static_cast<short>(v); std::memcpy(camera + off, &s, 2); }
static int getw(int off) { int v; std::memcpy(&v, camera + off, 4); return v; }
static int geth(int off) { short v; std::memcpy(&v, camera + off, 2); return v; }
static void setpad(int k, int off, float v) { std::memcpy(pad + 88 * k + off, &v, 4); }

static void reset(float stickX) {
    std::memset(camera, 0, sizeof camera);
    std::memset(pad, 0, sizeof pad);
    std::memset(player, 0, sizeof player);
    std::memset(fadeTimer, 0, sizeof fadeTimer);
    std::memset(fadeIconY, 0, sizeof fadeIconY);
    levelFlags = 0; localNmPlayersAlive = 0; gameCombatCamera = 0; gameReverseCameraYaw = 0; gameControlPaused = 0;
    rotDir = 0; rotAmount = 0; rotCalls = 0; hudCalls = 0; localNumber = 2;
    for (int k = 0; k < 3; ++k) {
        std::memset(targets[k], 0, 64);
        unsigned short yawv = static_cast<unsigned short>(1000 * (k + 1));
        std::memcpy(targets[k] + 56, &yawv, 2);
        unsigned char* p = targets[k];
        std::memcpy(player + 23984 * k + 21872, &p, sizeof p);
    }
    setw(64, 1);
    setpad(1, 28, stickX);
    seth(104, 100);
}

static void run() { doUserYaw__6Camera(camera); }

int main() {
    // Early outs.
    reset(1.0f); levelFlags = 0x2000000; setw(60, 77); run();
    expect(getw(60) == 77 && geth(104) == 100, "level flag");
    reset(1.0f); setf(156, 0.5f); setw(60, 77); run();
    expect(getw(60) == 77 && geth(104) == 100, "F(156) blocks");

    // Path 1: spinning down with a W(56) timer.
    reset(0.0f); setw(56, 1); setw(60, 100); rotAmount = 10; seth(138, 5); run();
    expect(rotCalls == 1 && rotFrom == 100 && rotTo == 5, "path 1 rotation arguments");
    expect(getw(60) == 85 && getw(56) == 1, "path 1: 100 * 0.85 rounds to 85 and stays >= 50");
    expect(geth(104) == 100 + 84, "path 1 yaw advance truncates 85 * 0.9999999");
    reset(0.0f); setw(56, 1); setw(60, 55); rotAmount = 10; run();
    expect(getw(60) == 47 && getw(56) == 0, "path 1: below 50 clears W(56)");
    reset(0.0f); setw(56, 1); setw(60, -100); rotAmount = 10; run();
    expect(getw(60) == -85 && getw(56) == 0, "path 1 rounds away from zero for negatives; negative clears");
    reset(0.0f); setw(56, 2); setw(60, 10); rotAmount = 1000; run();
    expect(getw(60) == 150 && getw(56) == 2, "path 1: amount >= 1000 adds 70*W(56)");

    // Path 2: a local player drives the yaw.
    reset(0.0f); setw(100, 1); rotDir = 1; rotAmount = 5000; seth(104, 30); run();
    expect(rotTo == static_cast<short>(16383 - 3000) && rotFrom == 30, "path 2 target yaw from local player");
    expect(getw(60) == 70, "path 2: 70 * min(1, amount * 0.00025)");
    reset(0.0f); setw(100, 1); rotDir = 1; rotAmount = 2000; run();
    expect(getw(60) == 34, "path 2: half scale truncates 34.99997");
    reset(0.0f); setw(100, 1); rotDir = -1; rotAmount = 400; run();
    expect(getw(60) == -6, "path 2 scales by amount and truncates toward zero");
    reset(0.9f); setw(100, 1); rotDir = 1; rotAmount = 5000; run();
    expect(getw(60) == 70 && hudCalls == 1 && fadeTimer[7] == 120 && fadeIconY[7] == 5001, "path 2 clamp is skipped, HUD runs");
    reset(0.4f); setw(100, 1); rotDir = 1; rotAmount = 4000; run();
    expect(hudCalls == 0, "HUD needs x > 0.5");

    // Path 3: stick.
    reset(0.5f); setw(60, 0); run();
    expect(getw(60) == 0 + static_cast<int>(0.125f * (std::pow(0.5f, 0.95f) * 560.0f)), "path 3 eases toward the stick");
    reset(-0.5f); gameReverseCameraYaw = 1; setw(60, 0); run();
    expect(getw(60) > 0, "negative stick and reversed camera give a positive turn");
    reset(-0.5f); setw(60, 0); run();
    expect(getw(60) < 0, "negative stick turns negative");
    reset(1.0f); setw(60, 0); run();
    expect(getw(60) == 70, "path 3 limited to 153 after ease (560/8 = 70)");
    reset(0.6f); localNmPlayersAlive = 3; setw(60, 40); run();
    expect(getw(60) == 40 && hudCalls == 1, "three or more players: only HUD");
    reset(0.6f); localNmPlayersAlive = 2; camera[352] = 1; setf(212, 0.8f); setw(60, 100); run();
    expect(getw(60) == 95 && hudCalls == 1, "camera mode: 0.95 damping");
    reset(0.6f); localNmPlayersAlive = 2; camera[352] = 1; setf(212, 0.6f); setw(60, 0); run();
    expect(getw(60) > 0 && hudCalls == 0, "mode 352 with F(212) not above 0.7 uses the stick path");
    reset(0.6f); localNmPlayersAlive = 1; setw(60, 200); run();
    expect(getw(60) <= 153, "stick path is clamped");

    // Path 4: idle stick.
    reset(0.0f); setw(60, 100); run();
    expect(getw(60) == 75 && geth(104) == 100 + 74, "idle damps by a quarter (100 - 25)");
    reset(0.0f); setw(60, 22); run();
    expect(getw(60) == 0, "idle 22 damps to 17, which is within 19 and becomes zero");
    reset(0.0f); setw(60, 25); run();
    expect(getw(60) == 0, "idle values within 19 become zero");
    reset(0.0f); setw(60, 1000); run();
    expect(getw(60) == 153 - 38, "clamp to 153 precedes the damping");

    // Combat camera follows the first listed player.
    reset(0.0f); gameCombatCamera = 1; setf(144, 1.0f); localNmPlayersAlive = 1; localPlayersAlive[0] = 0;
    rotDir = 1; rotAmount = 16000; setw(60, 10);
    setpad(1, 20, 3.0f); setpad(1, 24, 4.0f);
    run();
    expect(rotTo == static_cast<short>(16383 - 1000), "combat target from first listed player");
    {
        const float q = std::pow(16000.0f * 0x1.0624dcp-14f, 2.0f);
        const int s = static_cast<short>(static_cast<int>(1.0f * 124.0f * q));
        const int r = static_cast<short>(static_cast<int>(static_cast<float>(s) * 5.0f));
        const int want = static_cast<short>(static_cast<int>(10.0f + (static_cast<float>(r) - 10.0f) * 0.1f));
        expect(getw(60) == want, "combat follow value");
    }
    reset(0.0f); gameCombatCamera = 1; setf(144, 0.5f); localNmPlayersAlive = 1; setw(60, 100); run();
    expect(rotCalls == 0 && getw(60) == 75, "F(144) != 1.0 means plain damping");
    reset(0.0f); gameCombatCamera = 1; setf(144, 1.0f); localNmPlayersAlive = 1; gameControlPaused = 1; setw(60, 100); run();
    expect(rotCalls == 0 && getw(60) == 75, "paused means plain damping");
    reset(0.0f); gameCombatCamera = 1; setf(144, 1.0f); localNmPlayersAlive = 0; setw(60, 100); run();
    expect(rotCalls == 0 && getw(60) == 75, "no players alive means plain damping");
    return failures ? 1 : 0;
}
