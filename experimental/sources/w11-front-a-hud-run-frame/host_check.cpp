// Host check for HUDRunFrame__Fv. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin -DW11_FRONT_A_HOST host_check.cpp
// The callees are stubs that append to a trace. Pointer words in the player records are handles
// that hostWordAddress maps to host buffers. Expected traces are written out by hand from the
// reading of the function in hud_run_frame.cpp.
#include "hud_run_frame.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" {
signed char hudNoDraw;
int coreLoading;
unsigned short infoMessageTimer;
int nmPlayers;
signed char netNetworkMode;
unsigned char hudBustCovered;
alignas(8) unsigned char netLocalPad[4 * 88];
signed char interfaceOn;
unsigned char* gameCam;
signed char cutMode;
signed char hudNetWaiting;
alignas(8) unsigned char player[4 * 23984];
signed char recallOn;
int levelUpFade[4];
signed char MC_Debug;
signed char drawBustOn;
signed char drawCompassOn;
alignas(8) unsigned char pad[16];
}
alignas(8) static unsigned char target[4][2048];
alignas(8) static unsigned char camera[64];
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    if (word >= 0x1000u && word < 0x1004u) return reinterpret_cast<__UINTPTR_TYPE__>(target[word - 0x1000u]);
    return 0;
}

static std::string calls;
static int pausedAnswer, jumpAnswer, taskAnswer;
extern "C" {
void runGold__Fv() { calls += "G;"; }
int gameIsPaused__Fv() { calls += "P;"; return pausedAnswer; }
int jumpGateJumping__Fv() { calls += "J;"; return jumpAnswer; }
int engineTaskActive__FPc(char* name) { calls += std::string("T:") + name + ";"; return taskAnswer; }
void inventoryStart__Fii(int i, int m) { calls += "I" + std::to_string(i) + "," + std::to_string(m) + ";"; }
void scriptStart__FPc(char* name) { calls += std::string("S:") + name + ";"; }
void givePlayerExperience__Fii(int i, int a) { calls += "X" + std::to_string(i) + "," + std::to_string(a) + ";"; }
}

static void reset() {
    std::memset(netLocalPad, 0, sizeof netLocalPad);
    std::memset(player, 0, sizeof player);
    std::memset(camera, 0, sizeof camera);
    std::memset(pad, 0, sizeof pad);
    const int one = 1;
    std::memcpy(camera + 52, &one, 4);
    gameCam = camera;
    hudNoDraw = 0; coreLoading = 0; infoMessageTimer = 0; nmPlayers = 1; netNetworkMode = 0; hudBustCovered = 0;
    interfaceOn = 0; cutMode = 0; hudNetWaiting = 0; recallOn = 0; MC_Debug = 0; drawBustOn = 0; drawCompassOn = 0;
    pausedAnswer = jumpAnswer = taskAnswer = 0;
    calls.clear();
    for (int i = 0; i < 4; ++i) {
        std::memset(target[i], 0, sizeof target[i]);
        const unsigned handle = 0x1000u + static_cast<unsigned>(i);
        std::memcpy(player + 23984 * i + 21872, &handle, 4);
        const int bit = 0x100;
        std::memcpy(netLocalPad + 88 * i + 12, &bit, 4);
        levelUpFade[i] = 99;
    }
}
static void setInt(unsigned char* base, int off, int v) { std::memcpy(base + off, &v, 4); }

static int failures;
static void expect(const char* what, const std::string& want) {
    if (calls != want) { std::printf("FAIL %s: got [%s] want [%s]\n", what, calls.c_str(), want.c_str()); ++failures; }
}
static void expectInt(const char* what, long got, long want) {
    if (got != want) { std::printf("FAIL %s: got %ld want %ld\n", what, got, want); ++failures; }
}
static const char* const gates = "P;J;T:jumpgate;";

int main() {
    // Timer and bust cover.
    reset(); infoMessageTimer = 5; nmPlayers = 4; calls.clear();
    for (int i = 0; i < 4; ++i) setInt(netLocalPad + 88 * i, 12, 0);
    HUDRunFrame__Fv();
    expectInt("timer", infoMessageTimer, 4); expectInt("cover", hudBustCovered, 1);
    reset(); infoMessageTimer = 5; nmPlayers = 4; netNetworkMode = 1; setInt(netLocalPad, 12, 0);
    HUDRunFrame__Fv(); expectInt("net mode keeps cover clear", hudBustCovered, 0);
    reset(); infoMessageTimer = 0x8000; nmPlayers = 0; HUDRunFrame__Fv();
    expectInt("negative timer untouched", infoMessageTimer, 0x8000);
    reset(); infoMessageTimer = 5; nmPlayers = 0; hudNoDraw = 1; HUDRunFrame__Fv(); expectInt("hudNoDraw", infoMessageTimer, 5);
    reset(); infoMessageTimer = 5; nmPlayers = 0; coreLoading = 1; HUDRunFrame__Fv(); expectInt("coreLoading", infoMessageTimer, 5);
    reset(); nmPlayers = 3; drawBustOn = 1; drawCompassOn = 7; HUDRunFrame__Fv();
    expectInt("bust", drawBustOn, 0); expectInt("compass", drawCompassOn, 6);

    // Plain open: no level-up pending, nothing in the inventory-notify slots.
    reset(); setInt(target[0], 12, 0); { const float f = 2.0f; std::memcpy(player + 172, &f, 4); }
    HUDRunFrame__Fv();
    expect("open", std::string("G;") + gates + "I0,0;");
    // Level-up pending (record + 88 < record + 89): mode 1 and the fade cleared.
    reset(); { const float f = 0.5f; std::memcpy(player + 172, &f, 4); } player[88] = 0xff; player[89] = 1;
    HUDRunFrame__Fv();
    expect("levelup", std::string("G;") + gates + "I0,1;");
    expectInt("fade cleared", levelUpFade[0], 0); expectInt("other fade", levelUpFade[1], 99);
    // Equal bytes: mode 0.
    reset(); { const float f = 0.5f; std::memcpy(player + 172, &f, 4); } player[88] = 3; player[89] = 3;
    HUDRunFrame__Fv(); expect("equal", std::string("G;") + gates + "I0,0;"); expectInt("fade kept", levelUpFade[0], 99);
    // Health of zero or bit 8 set: no open, but the notify scan still runs.
    reset(); HUDRunFrame__Fv(); expect("no health", std::string("G;") + gates);
    reset(); setInt(target[0], 12, 8); { const float f = 9.0f; std::memcpy(player + 172, &f, 4); } HUDRunFrame__Fv();
    expect("flag 8", std::string("G;") + gates);
    // Notify scripts: one per positive record, for every pass of the outer loop.
    reset(); nmPlayers = 2; setInt(player + 23984, 4936, 3); setInt(player, 4936, -1);
    setInt(netLocalPad + 88, 12, 0);
    HUDRunFrame__Fv(); expect("notify", std::string("G;") + gates + "S:*inventoryOnNotify;");
    // Each blocker, tested alone.
    struct Blocker { const char* name; void (*set)(); const char* want; };
    const Blocker blockers[] = {
        {"pad bit", [] { setInt(netLocalPad, 12, 0xff); }, "G;"},
        {"pad 0x80", [] { setInt(netLocalPad, 8, 0x80); }, "G;"},
        {"interface", [] { interfaceOn = 1; }, "G;"},
        {"paused", [] { pausedAnswer = 1; }, "G;P;"},
        {"camera", [] { setInt(camera, 52, 2); }, "G;P;"},
        {"cut", [] { cutMode = 1; }, "G;P;"},
        {"net waiting", [] { hudNetWaiting = 1; }, "G;P;"},
        {"busy target", [] { setInt(target[0], 1328, 1); }, "G;P;"},
        {"jump", [] { jumpAnswer = 1; }, "G;P;J;"},
        {"task", [] { taskAnswer = 1; }, "G;P;J;T:jumpgate;"},
        {"recall", [] { recallOn = 1; }, "G;P;J;T:jumpgate;"},
        {"dead flag", [] { setInt(target[0], 12, 0x400000); }, "G;P;J;T:jumpgate;"},
        {"no target", [] { const unsigned z = 0; std::memcpy(player + 21872, &z, 4); }, "G;P;J;T:jumpgate;"},
    };
    for (const Blocker& b : blockers) {
        reset(); const float f = 2.0f; std::memcpy(player + 172, &f, 4);
        b.set(); HUDRunFrame__Fv();
        expect(b.name, b.want);
    }

    // MC_Debug cheat.
    reset(); nmPlayers = 2; MC_Debug = 1; setInt(pad, 12, 0x4000); setInt(pad, 8, 8);
    setInt(netLocalPad, 12, 0); setInt(netLocalPad + 88, 12, 0);
    HUDRunFrame__Fv();
    expect("debug", "G;X0,20000;X1,20000;"); expectInt("gold", *reinterpret_cast<int*>(player + 232), 0x16e360);
    reset(); nmPlayers = 0; MC_Debug = 1; setInt(pad, 12, 0x4000); setInt(pad, 8, 8); HUDRunFrame__Fv();
    expect("debug no players", "G;"); expectInt("gold no players", *reinterpret_cast<int*>(player + 232), 0x16e360);
    reset(); nmPlayers = 0; MC_Debug = 1; setInt(pad, 12, 0x4000); setInt(pad, 8, 0); HUDRunFrame__Fv();
    expectInt("debug needs button", *reinterpret_cast<int*>(player + 232), 0);
    reset(); nmPlayers = 0; MC_Debug = 0; setInt(pad, 12, 0x4000); setInt(pad, 8, 8); HUDRunFrame__Fv();
    expectInt("debug needs flag", *reinterpret_cast<int*>(player + 232), 0);
    reset(); nmPlayers = 0; MC_Debug = 1; setInt(pad, 12, 0xbfff); setInt(pad, 8, 8); HUDRunFrame__Fv();
    expectInt("debug needs bit 0x4000", *reinterpret_cast<int*>(player + 232), 0);

    if (failures == 0) std::puts("hud_run_frame host check passed");
    return failures != 0;
}
