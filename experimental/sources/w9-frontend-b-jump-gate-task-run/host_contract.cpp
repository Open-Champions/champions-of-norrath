#include "jump_gate_task_run.cpp"

#include <cstdio>
#include <cstring>
#include <string>

unsigned char jumpGateState;
signed char jumpGateWho;
char* jumpGateHurtDest;
float jumpGateTime, cursorTime;
int thisLevel, jumping, flareAlpha;
signed char curDest, lastDest, jumpGateDestCount;
char jumpGateEntries[480];
char levelName[96];
unsigned char pad[4 * 88];
int padBackButton = 0x40, padAcceptButton = 0x20;
int gameControlPaused, gameControlDontResetPause, gamePause;
signed char particlesPaused, netNetworkMode, hudNoDraw;
static std::string events;
static int local = 1, connected = 1, debug_menu;
static void putw(int who, int off, int v) { std::memcpy(pad + 88 * who + off, &v, 4); }

extern "C" {
void gameGetFullLevelName__FPc(char* n) { std::strcpy(n, "beta"); }
int netPlayerIsLocal__Fi(int) { return local; }
int netPlayerIsConnected__Fi(int) { return connected; }
int debugStartMenuIsOn__Fv() { return debug_menu; }
void debugClearStartMenu__Fv() { events += "clear;"; }
void netBlankController__Fii(int, int m) { events += "blank" + std::to_string(m) + ";"; }
void* lumpFindResource__FPcT0(const char* l, const char* n) {
    events += std::string(l) + "/" + n + ";";
    return 0;
}
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void*, void*, float v) {
    events += "sound" + std::to_string(static_cast<int>(v)) + ";";
}
void initJumpEffect__Fv() { events += "fx;"; }
void engineRemoveTask__FPFv_v(void (*f)()) { events += f == jumpGateTaskRun__Fv ? "rmrun;" : "rmdraw;"; }
void jumpGateTaskDraw__Fv() {}
}

static void step(int state) { jumpGateState = static_cast<unsigned char>(state); events.clear(); jumpGateTaskRun__Fv(); }

int main() {
    std::strcpy(jumpGateEntries, "alpha");
    std::strcpy(jumpGateEntries + 32, "beta");
    std::strcpy(jumpGateEntries + 64, "gamma");
    jumpGateDestCount = 3;
    step(0);
    if (thisLevel != 1 || jumpGateState != 1 || !gamePause || !particlesPaused || gameControlPaused != 1) return 1;
    jumpGateHurtDest = jumpGateEntries + 64;
    step(0);
    if (jumpGateState != 4) return 2;
    step(4);
    if (events != "fx;" || jumpGateState != 5) return 3;
    jumping = 181;
    step(5);
    if (std::strcmp(levelName, "gamma") != 0 || levelName[80] != -1 || jumpGateState != 6) return 4;
    jumpGateHurtDest = 0;
    curDest = 2; jumping = 200;
    step(5);
    if (std::strcmp(levelName, "gamma") != 0) return 5;
    jumping = 10; jumpGateTime = 0.98f;
    step(5);
    if (jumping != 11 || jumpGateTime != 1.0f) return 6;
    flareAlpha = 120;
    step(1);
    if (flareAlpha != 127 || jumpGateState != 2) return 7;
    // state 2: selection moves down with the down bits, then up with the up bits
    curDest = 0; thisLevel = 2; cursorTime = 0.95f;
    putw(0, 12, 0); putw(0, 16, 0x4000);
    step(2);
    if (curDest != 1 || lastDest != 0 || cursorTime != 0.0f || events != "fx.lmp/menu_main_toggle.anm;sound1;")
        return 8;
    putw(0, 16, 0x1000);
    step(2);
    if (curDest != 0 || lastDest != 1) return 9;
    step(2);  // up at the top does nothing; down bit absent
    if (curDest != 0 || !events.empty() ) return 10;
    putw(0, 12, 0x20);  // accept on a different level than the current one
    step(2);
    if (jumping != 1 || jumpGateState != 3 || events != "fx.lmp/menu_main_accept.anm;sound1;") return 11;
    putw(0, 12, 0x40);  // back
    step(2);
    if (jumpGateState != 3 || jumping != 1) return 12;
    debug_menu = 1;
    step(2);
    if (events != "blank4;" || jumpGateState != 2) return 13;
    debug_menu = 0;
    // state 3: fade out; jumping set advances, otherwise the tasks are removed
    flareAlpha = 5;
    jumping = 1;
    step(3);
    if (flareAlpha != 0 || jumpGateState != 4 || events != "clear;") return 14;
    jumping = 0;
    flareAlpha = 5;
    step(3);
    if (events != "blank5;rmrun;rmdraw;" || gameControlPaused != 0) return 15;
    step(7);
    if (gameControlPaused != 10 || events != "rmrun;rmdraw;") return 16;
    step(9);
    if (!events.empty() || jumpGateState != 9) return 17;
    std::puts("jump gate task run passed");
    return 0;
}
