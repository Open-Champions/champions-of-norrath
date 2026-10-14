// Host contract for obtainTarget__6CameraP6Point3. Not an EE plan input.
#include "obtain_target.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>

int nmPlayers, nmPlayersAlive, localNmPlayersAlive;
int localPlayersAlive[4];
int playersAlive[4];
signed char netNetworkMode, MC_SavingOrLoading, hudDisplayNetCamToggle;
short gameDeathTimerSingle[4];
float worldCameraMatrix[12];
float worldCameraPos[3];
unsigned char player[23984 * 4];

static double asDouble(Double v) { double d; std::memcpy(&d, &v, 8); return d; }
static Double fromDouble(double d) { Double v; std::memcpy(&v, &d, 8); return v; }
Double fptodp(float v) { return fromDouble(v); }
int dpcmp(Double a, Double b) { double x = asDouble(a), y = asDouble(b); return x < y ? -1 : x > y ? 1 : 0; }
Double dpsub(Double a, Double b) { return fromDouble(asDouble(a) - asDouble(b)); }
float dptofp(Double a) { return static_cast<float>(asDouble(a)); }

static int isLocal[4];
int netPlayerIsLocal__Fi(int i) { return isLocal[i]; }
// "Transform": x' = x * 2, y' = y * 3, z' = z (kept positive by the fixtures).
void* matTransform__FP6Point3P8Matrix34T0(float* out, float* matrix, float* in) {
    if (matrix != worldCameraMatrix) std::fprintf(stderr, "matrix argument\n");
    out[0] = in[0] * 2.0f; out[1] = in[1] * 3.0f; out[2] = in[2];
    return out;
}
static float untIn[3];
void* matUntransform34Vec__FP6Point3P8Matrix34T0(float* out, float* matrix, float* in) {
    if (matrix != worldCameraMatrix) std::fprintf(stderr, "matrix argument\n");
    std::memcpy(untIn, in, 12);
    out[0] = in[0] + 1.0f; out[1] = in[1] + 2.0f; out[2] = in[2] + 3.0f;
    return out;
}

static int failures = 0;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }

static unsigned char camera[512];
static unsigned char holders[4][1400];
static unsigned char vehicles[4][64];
static void setw(int off, int v) { std::memcpy(camera + off, &v, 4); }
static void setf(int off, float v) { std::memcpy(camera + off, &v, 4); }
static float getf(int off) { float v; std::memcpy(&v, camera + off, 4); return v; }

static void setHolder(int k, float x, float y, float z) {
    const float v[3] = {x, y, z};
    std::memcpy(holders[k] + 16, v, 12);
}

static void reset() {
    std::memset(camera, 0, sizeof camera);
    std::memset(player, 0, sizeof player);
    nmPlayers = 2; nmPlayersAlive = 0; localNmPlayersAlive = 0;
    netNetworkMode = 0; MC_SavingOrLoading = 0; hudDisplayNetCamToggle = 0;
    std::memset(gameDeathTimerSingle, 0, sizeof gameDeathTimerSingle);
    for (int k = 0; k < 4; ++k) {
        isLocal[k] = 0;
        localPlayersAlive[k] = k; playersAlive[k] = k;
        std::memset(holders[k], 0, sizeof holders[k]);
        setHolder(k, 1.0f + k, 10.0f + k, 100.0f + k);
        unsigned char* h = holders[k];
        std::memcpy(player + 23984 * k + 21872, &h, sizeof h);
        unsigned char* v = vehicles[k];
        std::memcpy(holders[k] + 1328, &v, sizeof v);
        std::memset(vehicles[k], 0, 64);
        const float vp[3] = {7.0f + k, 8.0f + k, 9.0f + k};
        std::memcpy(vehicles[k] + 16, vp, 12);
        const float fl = 1.0f; std::memcpy(player + 23984 * k + 172, &fl, 4);
    }
    for (int i = 0; i < 12; ++i) worldCameraMatrix[i] = static_cast<float>(i);
    worldCameraPos[0] = 100; worldCameraPos[1] = 200; worldCameraPos[2] = 300;
    setf(252, 0.5f);
}

int main() {
    float out[3];
    // Nobody alive and no network: returns 0 and does not touch out.
    reset(); out[0] = out[1] = out[2] = -1.0f;
    expect(obtainTarget__6CameraP6Point3(camera, out) == 0 && out[0] == -1.0f, "no players, no network");

    // Network spectating with nobody alive.
    reset(); netNetworkMode = 1; camera[353] = 2; gameDeathTimerSingle[2] = 0;
    expect(obtainTarget__6CameraP6Point3(camera, out) == 1 && hudDisplayNetCamToggle == 0, "dead-timer zero gives nothing");
    reset(); netNetworkMode = 1; camera[353] = 2; gameDeathTimerSingle[2] = 9;
    out[0] = out[1] = out[2] = -1.0f;
    expect(obtainTarget__6CameraP6Point3(camera, out) == 1 && hudDisplayNetCamToggle == 1 && out[0] == 3 && out[1] == 12 && out[2] == 102.5f, "network spectate position plus F(252)");
    reset(); netNetworkMode = 1; nmPlayersAlive = 2; camera[353] = 1; gameDeathTimerSingle[1] = 0;
    expect(obtainTarget__6CameraP6Point3(camera, out) == 1 && hudDisplayNetCamToggle == 1 && out[0] == 2, "nmPlayersAlive skips the timer test");
    reset(); netNetworkMode = 1; nmPlayersAlive = 2; camera[353] = 1;
    { unsigned char* none = nullptr; std::memcpy(player + 23984 + 21872, &none, sizeof none); }
    hudDisplayNetCamToggle = 0;
    expect(obtainTarget__6CameraP6Point3(camera, out) == 1 && hudDisplayNetCamToggle == 0, "null holder writes nothing");

    // Averaging path.
    reset(); localNmPlayersAlive = 2; localPlayersAlive[0] = 1; localPlayersAlive[1] = 3; nmPlayers = 0;
    expect(obtainTarget__6CameraP6Point3(camera, out) == 1, "average returns 1");
    expect(out[0] == (2.0f + 4.0f) / 2.0f && out[1] == (11.0f + 13.0f) / 2.0f && out[2] == (101.0f + 103.0f) / 2.0f + 0.5f, "average of listed players plus F(252)");
    expect(camera[353] == 0, "found index stored (nmPlayers 0 gives 0)");

    // Found index: first local player with nonzero float and holder.
    reset(); localNmPlayersAlive = 1; localPlayersAlive[0] = 0; nmPlayers = 3;
    isLocal[0] = 0; isLocal[1] = 1; isLocal[2] = 1;
    { const float zero = 0.0f; std::memcpy(player + 23984 * 1 + 172, &zero, 4); }
    obtainTarget__6CameraP6Point3(camera, out);
    expect(camera[353] == 2, "player 1 skipped for zero float, player 2 found");
    reset(); localNmPlayersAlive = 1; localPlayersAlive[0] = 3; nmPlayers = 3; isLocal[0] = isLocal[1] = isLocal[2] = 1;
    { unsigned char* none = nullptr; std::memcpy(player + 21872, &none, sizeof none); }
    obtainTarget__6CameraP6Point3(camera, out);
    expect(camera[353] == 1, "player 0 skipped for null holder");
    reset(); localNmPlayersAlive = 1; nmPlayers = 3;
    obtainTarget__6CameraP6Point3(camera, out);
    expect(camera[353] == 3, "none local gives nmPlayers");

    // W(100): look through the first alive player's vehicle pointer.
    reset(); localNmPlayersAlive = 1; setw(100, 1); setf(140, 8.0f); playersAlive[0] = 2; nmPlayers = 0;
    obtainTarget__6CameraP6Point3(camera, out);
    expect(out[0] == 9.0f - 1.0f && out[1] == 10.0f - 4.0f && out[2] == 11.0f - 7.0f, "W(100): vehicle position minus matrix terms * F(140)/8");

    // Framing path with two players.
    reset(); localNmPlayersAlive = 2; localPlayersAlive[0] = 0; localPlayersAlive[1] = 1; nmPlayers = 1; setw(72, 0);
    setHolder(0, 1.0f, 2.0f, 10.0f); setHolder(1, 5.0f, 1.0f, 20.0f);
    obtainTarget__6CameraP6Point3(camera, out);
    // transformed: P0 = (2, 6, 10) -> (0.2, 0.6); P1 = (10, 3, 20) -> (0.5, 0.15)
    const float mid = (20.0f + 10.0f) * 0.5f;
    expect(std::fabs(untIn[2] - mid) < 1e-6f && std::fabs(untIn[0] - (0.5f + 0.2f) * 0.5f * mid) < 1e-5f &&
           std::fabs(untIn[1] - (0.6f + 0.15f) * 0.5f * mid) < 1e-5f, "framing centre");
    expect(std::fabs(getf(204) - 0.3f) < 1e-6f && std::fabs(getf(208) - 0.45f) < 1e-6f, "F(204), F(208) are the spans");
    expect(std::fabs(getf(212) - std::sqrt(0.3f * 0.3f + 0.45f * 0.45f)) < 1e-6f, "F(212) is their length");
    expect(std::fabs(out[0] - (untIn[0] + 1.0f + 100.0f)) < 1e-4f && std::fabs(out[2] - (untIn[2] + 3.0f + 300.0f + 0.5f)) < 1e-4f, "framing output adds camera position and F(252) to z");
    // Spans clamp at 1.0.
    reset(); localNmPlayersAlive = 2; localPlayersAlive[0] = 0; localPlayersAlive[1] = 1; nmPlayers = 1;
    setHolder(0, -50.0f, -50.0f, 10.0f); setHolder(1, 50.0f, 50.0f, 10.0f);
    obtainTarget__6CameraP6Point3(camera, out);
    expect(getf(204) == 1.0f && getf(208) == 1.0f && getf(212) == 1.0f, "spans clamp at 1.0");
    // W(72) set blocks the framing unless MC_SavingOrLoading.
    reset(); localNmPlayersAlive = 1; localPlayersAlive[0] = 0; nmPlayers = 1; setw(72, 1);
    obtainTarget__6CameraP6Point3(camera, out);
    expect(out[0] == 1.0f, "W(72) selects averaging");
    reset(); localNmPlayersAlive = 1; localPlayersAlive[0] = 0; nmPlayers = 1; setw(72, 1); MC_SavingOrLoading = 1;
    obtainTarget__6CameraP6Point3(camera, out);
    expect(out[0] != 1.0f, "MC_SavingOrLoading re-enables framing");
    return failures ? 1 : 0;
}
