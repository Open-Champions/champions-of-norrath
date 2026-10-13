// Host contract for doNormalCam__6Camera. Not an EE plan input.
#include "do_normal_cam.cpp"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

int localNmPlayersAlive;
int nmPlayersAlive;
int gamePlayersPresent;
signed char netNetworkMode;
int localPlayersAlive[4];
int playersAlive[4];
unsigned char player[23984 * 4];
float worldCameraMatrix[12];
float worldCameraPos[3];
static int object_values[4][400];

static std::string log_events;
static std::vector<float> log_floats;
static int obtain_result, controller_result;
static float ray_result;
static float enforce_scale;
static void* seen_cam;

extern "C" {
int obtainTarget__6CameraP6Point3(void* c, void* p) {
    seen_cam = c; log_events += 'T';
    log_floats.push_back(float((unsigned char*)p - (unsigned char*)c));
    return obtain_result;
}
void playerCountChanged__6Camera(void*) { log_events += 'P'; }
void doUserZoom__6Camera(void*) { log_events += 'Z'; }
void doUserYaw__6Camera(void*) { log_events += 'Y'; }
void doAutoPitch__6Camera(void*) { log_events += 'A'; }
void checkYawReset__6Camera(void*) { log_events += 'R'; }
int determineBestController__6Camera(void*) { log_events += 'D'; return controller_result; }
void enforceMaxSpeed__6CameraP6Point3(void*, float* d) {
    log_events += 'E';
    for (int i = 0; i < 3; ++i) log_floats.push_back(d[i]);
    for (int i = 0; i < 3; ++i) d[i] *= enforce_scale;
}
float rayHit__FP6Point3N30(const void* o, const float* a, const float* b, const float* c) {
    log_events += 'H';
    log_floats.push_back(float((const unsigned char*)o - (const unsigned char*)seen_cam));
    for (int i = 0; i < 3; ++i) log_floats.push_back(a[i]);
    for (int i = 0; i < 3; ++i) log_floats.push_back(b[i]);
    for (int i = 0; i < 3; ++i) log_floats.push_back(c[i]);
    return ray_result;
}
void BuildMatrix__6CamerafffP6Point3(void*, float a, float b, float c, void* p) {
    log_events += 'B';
    log_floats.push_back(a); log_floats.push_back(b); log_floats.push_back(c);
    log_floats.push_back(p ? 1.0f : 0.0f);
}
}

static int failures = 0;
static void expect(int ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static bool near(double a, double b) { return std::fabs(a - b) <= 2e-4 * (1.0 + std::fabs(b)); }

struct Cam { unsigned char b[512]; };
static float gf(const Cam& c, int o) { float v; std::memcpy(&v, c.b + o, 4); return v; }
static void sf(Cam& c, int o, float v) { std::memcpy(c.b + o, &v, 4); }
static int gw(const Cam& c, int o) { int v; std::memcpy(&v, c.b + o, 4); return v; }
static void sw(Cam& c, int o, int v) { std::memcpy(c.b + o, &v, 4); }

int main() {
    for (int i = 0; i < 12; ++i) worldCameraMatrix[i] = 0.3f + 0.17f * float((i * 7) % 11) - (i % 3 == 0 ? 0.6f : 0.0f);
    worldCameraPos[0] = 1.5f; worldCameraPos[1] = -2.5f; worldCameraPos[2] = 8.0f;
    for (int e = 0; e < 4; ++e) {
        unsigned char* rec = player + 23984 * e;
        int* obj = object_values[e];
        std::memcpy(rec + 21872, &obj, sizeof obj);
    }
    int cases = 0;
    for (int lc : {0, 1, 2, 3})
    for (int present : {0, 1})
    for (int b364 : {0, 2, -3})
    for (int obtain : {0, 1})
    for (int w96 : {0, 1})
    for (int net : {0, 1})
    for (int b366 : {1, 3})
    for (int state : {0, 1})
    for (int b365 : {0, 3})
    for (float s156 : {0.0f, 1.0f})
    for (float s200 : {0.0f, 0.5f, 0.02f})
    for (int w100pre : {0, 1, 2, 3})
    for (float s164 : {-1.0f, 5.0f})
    for (float rr : {0.75f, 9.0f}) {
        // w100pre selects the player-value scenario for the camera-sharing check.
        int nAlive = w100pre == 0 ? 1 : (w100pre == 1 ? 3 : 2);
        for (int e = 0; e < 4; ++e) for (int k = 0; k < 400; ++k) object_values[e][k] = 0;
        const int first_value = w100pre == 3 ? 0 : 77;
        object_values[0][1328 / 4] = first_value;
        object_values[1][1328 / 4] = 77;
        object_values[2][1328 / 4] = w100pre == 1 ? 77 : 55;
        object_values[3][1328 / 4] = 77;
        localPlayersAlive[0] = 0; localPlayersAlive[1] = 1;
        playersAlive[0] = 1; playersAlive[1] = 3; playersAlive[2] = 2; playersAlive[3] = 0;
        localNmPlayersAlive = lc; nmPlayersAlive = nAlive;
        gamePlayersPresent = present; netNetworkMode = (signed char)net;
        obtain_result = obtain; controller_result = 9; ray_result = rr; enforce_scale = 0.5f;
        Cam cam;
        std::memset(cam.b, 0, sizeof cam.b);
        cam.b[364] = (unsigned char)b364; cam.b[365] = (unsigned char)b365; cam.b[366] = (unsigned char)b366;
        sw(cam, 72, state); sw(cam, 96, w96); sw(cam, 64, 4); sw(cam, 68, 5); sw(cam, 100, 9);
        sf(cam, 24, 30.0f); sf(cam, 28, -6.0f); sf(cam, 32, 12.0f);
        sf(cam, 36, 2.0f); sf(cam, 40, 1.0f); sf(cam, 44, -3.0f);
        sf(cam, 140, 4.0f); sf(cam, 156, s156); sf(cam, 164, s164);
        sf(cam, 200, s200); sf(cam, 204, 0.2f); sf(cam, 208, 0.1f);
        Cam before = cam;
        log_events.clear(); log_floats.clear();
        doNormalCam__6Camera(cam.b);
        ++cases;

        // Model.
        std::string ev;
        Cam want = before;
        want.b[352] = lc >= 2;
        sw(want, 100, 0);
        if (lc != 0) {
            if (object_values[0][1328 / 4] != 0) {
                bool same = true;
                for (int i = 1; i < nAlive; ++i) if (object_values[playersAlive[i]][1328 / 4] != object_values[playersAlive[0]][1328 / 4]) same = false;
                if (same) sw(want, 100, 1);
            }
        }
        if (!present) {
            sw(want, 72, 0);
            expect(std::memcmp(want.b, cam.b, sizeof want.b) == 0 && log_events.empty(), "no players present: state cleared, nothing else");
            continue;
        }
        if (b364 == 0) {
            ev += 'T';
            if (!obtain) {
                expect(log_events == ev, "obtain failure returns right after the call");
                expect(std::memcmp(want.b, cam.b, sizeof want.b) == 0, "obtain failure leaves state alone");
                continue;
            }
        }
        if ((signed char)want.b[364] > 0) want.b[364]--;
        if (w96 && lc != b366 && net == 0) { ev += 'P'; want.b[366] = (unsigned char)lc; }
        double f200 = s200;
        bool pathA = state != 0;
        if (!pathA) {
            if (f200 != 0.0) {
                f200 = f200 - f200 * 0.05;
                if (f200 < 0.02) f200 = 0.0;
                sf(want, 200, (float)f200);
            }
            pathA = b365 != 0;
        }
        double pos[3] = {2.0, 1.0, -3.0};
        const double M[9] = {worldCameraMatrix[0], worldCameraMatrix[1], worldCameraMatrix[2], worldCameraMatrix[3], worldCameraMatrix[4],
                             worldCameraMatrix[5], worldCameraMatrix[6], worldCameraMatrix[7], worldCameraMatrix[8]};
        if (pathA) {
            for (int i = 0; i < 3; ++i) sf(want, 36 + 4 * i, gf(before, 24 + 4 * i));
            pos[0] = 30.0; pos[1] = -6.0; pos[2] = 12.0;
            ev += 'Z';
        } else {
            double k2 = s156 == 0.0f ? 0.059988 : 0.29994;
            double remain = 1.0 - f200;
            double d[3] = {(30.0 - 2.0) * 0.99998295 * remain, (-6.0 - 1.0) * 0.99998295 * remain, (12.0 + 3.0) * k2 * remain};
            bool big = want.b[352] && gw(want, 100) == 0 && f200 == 0.0;
            if (big) {
                double a = M[0] * d[0] + M[3] * d[1] + M[6] * d[2];
                double b = -M[1] * d[0] - M[4] * d[1] - M[7] * d[2];
                double c = -M[2] * d[0] - M[5] * d[1] - M[8] * d[2];
                double w = std::fmax(0.0, 0.9 - 0.2) * std::fmax(0.0, 0.7 - 0.1);
                a *= w; b *= w; c *= w;
                double n[3] = {M[0] * a - M[1] * b - M[2] * c, M[3] * a - M[4] * b - M[5] * c, M[6] * a - M[7] * b - M[8] * c};
                ev += 'E';
                for (int i = 0; i < 3; ++i) { d[i] = n[i] * 0.5; }
            }
            for (int i = 0; i < 3; ++i) pos[i] += d[i];
            sf(want, 36, (float)pos[0]); sf(want, 40, (float)pos[1]); sf(want, 44, (float)pos[2]);
            sw(want, 68, 4); sw(want, 64, 9);
            ev += 'D';
            if (w96) ev += "RZYA";
        }
        ev += 'B';
        double s = 4.0;
        if (s164 != -1.0f) { ev += 'H'; s = std::fmin(rr, 4.0); }
        ev += 'B';
        if (state != 0) ev += 'A';
        sw(want, 72, 0);
        if ((signed char)want.b[365] != 0) want.b[365]--;

        expect(log_events == ev, "call order");
        // Compare fields with tolerance for floats, exactly for the rest.
        for (int off = 0; off < 400; off += 4) {
            const bool is_float = (off >= 24 && off <= 44) || off == 140 || off == 156 || off == 164 || off == 200 || off == 204 || off == 208;
            if (is_float) expect(near(gf(cam, off), gf(want, off)), "float field");
            else expect(gw(cam, off) == gw(want, off), "word field");
        }
        // Check the build targets in the log.
        if (log_events.find('B') != std::string::npos && log_events == ev) {
            // Locate float groups: B entries carry 4 floats each, in order.
            // The first B is at the world camera position.
            size_t idx = 0;
            for (char ch : log_events) {
                if (ch == 'T') idx += 1;
                else if (ch == 'E') idx += 3;
                else if (ch == 'H') idx += 13;
                else if (ch == 'B') break;
            }
            expect(idx + 3 < log_floats.size() && near(log_floats[idx], 1.5) && near(log_floats[idx + 1], -2.5) && near(log_floats[idx + 2], 8.0) && log_floats[idx + 3] == 0.0f, "first build at the camera position");
            size_t last = log_floats.size() - 4;
            expect(near(log_floats[last], pos[0] - s * M[2]) && near(log_floats[last + 1], pos[1] - s * M[5]) && near(log_floats[last + 2], pos[2] - s * M[8]) && log_floats[last + 3] == 0.0f, "second build backs off along the view axis");
            if (s164 != -1.0f) {
                size_t h = idx + 4;
                expect(near(log_floats[h], 36.0) && near(log_floats[h + 1], -M[2]) && near(log_floats[h + 2], -M[5]) && near(log_floats[h + 3], -M[8]), "ray origin offset and axis");
                expect(log_floats[h + 4] == 0.0f && log_floats[h + 5] == 0.0f && log_floats[h + 6] == 5.0f && log_floats[h + 7] == 0.0f && log_floats[h + 8] == 0.0f && log_floats[h + 9] == -1.0f, "ray vectors");
            }
        }
    }
    expect(cases == 4 * 2 * 3 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 3 * 4 * 2 * 2, "all cases ran");
    return failures ? 1 : 0;
}
