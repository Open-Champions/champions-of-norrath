#include <cstdio>
extern "C" {
signed char debugPauseMenuOn, netNetworkMode;
unsigned char startMenuState, exitToMenuMode, particlesPaused;
int gamePause;
int log_[8], n;
signed char busy_player, busy_value;
void (*removed)();
signed char netGetLocalPlayerNm__Fi(int i) { log_[n++] = 100 + i; return 3; }
void msgHudSetPlayerBusy__Fcc(signed char p, signed char b) { log_[n++] = 200; busy_player = p; busy_value = b; }
void pauseMenuFrameRun__Fv() {}
void engineRemoveTask__FPFv_v(void (*t)()) { log_[n++] = 300; removed = t; }
}
#include "clear_start_menu.cpp"
static void prime(signed char dbg, signed char net) {
    n = 0; removed = 0; busy_player = busy_value = -9;
    debugPauseMenuOn = dbg; netNetworkMode = net;
    startMenuState = exitToMenuMode = particlesPaused = 5; gamePause = 6;
}
int main() {
    prime(0, 0); debugClearStartMenu__Fv();
    if (n || startMenuState != 5 || gamePause != 6 || debugPauseMenuOn != 0) return 1;
    prime(1, 0); debugClearStartMenu__Fv();
    if (startMenuState || exitToMenuMode || gamePause || particlesPaused || debugPauseMenuOn) return 2;
    if (n != 3 || log_[0] != 100 || log_[1] != 200 || log_[2] != 300) return 3;
    if (busy_player != 3 || busy_value != 0 || removed != pauseMenuFrameRun__Fv) return 4;
    prime(1, 1); debugClearStartMenu__Fv();
    if (startMenuState || exitToMenuMode || debugPauseMenuOn) return 5;
    if (gamePause != 6 || particlesPaused != 5) return 6;
    prime(-1, 0); debugClearStartMenu__Fv();
    if (debugPauseMenuOn != 0 || n != 3) return 7;
    std::puts("debugClearStartMenu__Fv check passed");
    return 0;
}
