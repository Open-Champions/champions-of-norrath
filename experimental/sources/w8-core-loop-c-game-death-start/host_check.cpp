// Host contract for gameDeathStart. Not an EE plan input.
#include <cstring>
#include "game_death_start.cpp"

extern "C" {
unsigned char deathNetHack = 0xEE;
unsigned char deathState = 0xEE;
}

static int seq = 0, fadeSeq = 0, taskSeq = 0;
static int fadeArg = -1, fadeStateSeen = -1, fadeNetSeen = -1;
static void (*taskFn)() = nullptr;
static int taskPri = -1, taskFlag = -1;
static char taskName[32];
extern "C" void gameDeath__Fv() {}
extern "C" void hudResetFadeScreen__Fi(int value) {
    fadeSeq = ++seq;
    fadeArg = value;
    fadeStateSeen = deathState;
    fadeNetSeen = deathNetHack;
}
extern "C" void engineAddTask__FPFv_viPci(void (*callback)(), int priority, char* name, int flag) {
    taskSeq = ++seq;
    taskFn = callback;
    taskPri = priority;
    taskFlag = flag;
    std::strncpy(taskName, name, sizeof taskName - 1);
}

int main() {
    gameDeathStart__Fi(0x1A5);  // only the low byte is kept
    if (fadeSeq != 1 || taskSeq != 2) return 1;
    if (deathNetHack != 0xA5 || deathState != 0) return 2;
    if (fadeArg != 0 || fadeStateSeen != 0 || fadeNetSeen != 0xA5) return 3;  // stores come first
    if (taskFn != gameDeath__Fv || taskPri != 20 || taskFlag != 0) return 4;
    if (std::strcmp(taskName, "deathTask") != 0) return 5;
    deathState = 7;
    seq = 0;
    gameDeathStart__Fi(0);
    if (deathNetHack != 0 || deathState != 0 || seq != 2) return 6;
    return 0;
}
