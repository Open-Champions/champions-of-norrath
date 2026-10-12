// Host contract for Update__6Camera. Not an EE plan input.
#include "update.cpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
static int log_calls[16];
static int log_count = 0;
static int state_after_normal = -1;
static int paused_v, dlg_v, busy_v;
static int camera_state_during_normal;

static void expect(int ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

signed char hudDisplayNetCamToggle;
signed char hudNetWaiting;
void* world;
int gameNoDraw;
int coreLoading;
int gameControlPaused;

enum { Paused = 1, Dlg = 2, Busy = 3, Normal = 4, Debug = 5, Innoruk = 6 };
static unsigned char* current_camera;

extern "C" int gameIsPaused__Fv() { log_calls[log_count++] = Paused; return paused_v; }
extern "C" int DLG_Active__Fv() { log_calls[log_count++] = Dlg; return dlg_v; }
extern "C" int playerLocalPlayerBusy__Fv() { log_calls[log_count++] = Busy; return busy_v; }
extern "C" void doNormalCam__6Camera(void* camera) {
    log_calls[log_count++] = Normal;
    std::memcpy(&camera_state_during_normal, static_cast<unsigned char*>(camera) + 72, 4);
    if (state_after_normal >= 0) std::memcpy(static_cast<unsigned char*>(camera) + 72, &state_after_normal, 4);
}
extern "C" void doDebugCam__6Camera(void*) { log_calls[log_count++] = Debug; }
extern "C" void doInnorukCam__6Camera(void*) { log_calls[log_count++] = Innoruk; }

static int word(const unsigned char* p, int off) {
    int v;
    std::memcpy(&v, p + off, 4);
    return v;
}

int main() {
    static unsigned char camera[512];
    int dummy_world;
    // Gate: every condition in turn, with earlier ones clear.
    for (int blocker = 0; blocker <= 7; ++blocker) {
        std::memset(camera, 0, sizeof camera);
        std::memset(camera + 96, 0x55, 4);
        hudDisplayNetCamToggle = 7;
        paused_v = blocker == 0 ? 3 : 0;
        world = blocker == 1 ? nullptr : &dummy_world;
        gameNoDraw = blocker == 2 ? 1 : 0;
        coreLoading = blocker == 3 ? 5 : 0;
        hudNetWaiting = blocker == 4 ? -1 : 0;
        dlg_v = blocker == 5 ? 1 : 0;
        gameControlPaused = blocker == 6 ? 1 : 0;
        busy_v = blocker == 7 ? 4 : 0;
        log_count = 0;
        state_after_normal = -1;
        Update__6Camera(camera);
        expect(hudDisplayNetCamToggle == 0, "toggle byte cleared");
        expect(word(camera, 96) == 0, "a blocker zeroes camera+96");
        if (blocker == 0) expect(log_count == 1 && log_calls[0] == Paused, "paused stops before DLG and busy");
        if (blocker >= 1 && blocker <= 4) expect(log_count == 1 && log_calls[0] == Paused, "plain flags stop before DLG");
        if (blocker == 5) expect(log_count == 2 && log_calls[1] == Dlg, "DLG blocks before busy");
        if (blocker == 6) expect(log_count == 2 && log_calls[1] == Dlg, "control pause blocks before busy");
        if (blocker == 7) expect(log_count == 3 && log_calls[2] == Busy, "busy is asked last");
    }
    // No blocker: result is one exactly when busy is zero.
    for (int busy = 0; busy < 3; ++busy) {
        std::memset(camera, 0, sizeof camera);
        paused_v = 0; world = &dummy_world; gameNoDraw = coreLoading = 0; hudNetWaiting = 0; dlg_v = 0; gameControlPaused = 0;
        busy_v = busy;
        log_count = 0;
        Update__6Camera(camera);
        expect(word(camera, 96) == (busy == 0 ? 1 : 0), "active word is not-busy");
    }
    // Camera selection.
    for (int mode = -1; mode <= 7; ++mode) {
        for (int state = 0; state <= 3; ++state) {
            for (int after = -1; after <= 3; ++after) {
                std::memset(camera, 0, sizeof camera);
                std::memcpy(camera + 52, &mode, 4);
                std::memcpy(camera + 72, &state, 4);
                paused_v = 1;
                log_count = 0;
                state_after_normal = after;
                Update__6Camera(camera);
                int want_calls[2] = {0, 0};
                int want_count = 0;
                int want_state = state;
                if (mode == 1 || state != 0) {
                    want_calls[want_count++] = Normal;
                    if (after >= 0) want_state = after;
                    if (want_state == 1 && mode != 1) want_state = 2;
                } else if (mode >= 3 && mode <= 5) {
                    want_calls[want_count++] = Debug;
                } else if (mode == 2) {
                    want_calls[want_count++] = Innoruk;
                }
                expect(log_count == 1 + want_count, "camera call count");
                for (int i = 0; i < want_count && 1 + i < log_count; ++i) expect(log_calls[1 + i] == want_calls[i], "camera routine chosen");
                expect(word(camera, 72) == want_state, "state word after update");
                expect(word(camera, 52) == mode, "mode unchanged");
            }
        }
    }
    return failures ? 1 : 0;
}
