#include "save_config.cpp"

#include <cstdio>

extern "C" int disableMC = 0;
extern "C" int ranConfig = 3;
extern "C" int delayCount = 3;

static int pauses;
static int delay_at_pause;
static int ran_at_pause;
static int mode;
static int mode_argument;

extern "C" void pushPause__Fv() {
    ++pauses;
    delay_at_pause = delayCount;
    ran_at_pause = ranConfig;
}
extern "C" int newMode__Fii(int next, int argument) {
    mode = next;
    mode_argument = argument;
    return 42;
}

int main() {
    disableMC = 6;
    if (MC_SaveConfig__Fv() != 6 || pauses != 0) return 1;
    disableMC = 0;
    ranConfig = 3;
    delayCount = 3;
    if (MC_SaveConfig__Fv() != 42) return 2;
    if (pauses != 1 || delay_at_pause != 0 || ran_at_pause != 0) return 3;
    if (mode != 4 || mode_argument != 0 || ranConfig != 0 || delayCount != 0) return 4;
    std::puts("save config passed");
    return 0;
}
