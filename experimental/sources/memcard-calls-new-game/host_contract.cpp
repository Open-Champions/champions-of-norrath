#include "new_game.cpp"

#include <cstdio>

extern "C" int delayCount = 4;
extern "C" int errorCondition = 4;
extern "C" int actionCoolOff = 4;
extern "C" int curPlaySeconds = 4;

static int quick;
static int set_act;
static int set_act_value;
static int reset_slot;
static int seconds_at_quick;

extern "C" void resetQuickLoad__Fv() {
    ++quick;
    seconds_at_quick = curPlaySeconds;
}
extern "C" void MC_SetNewAct__Fi(int act) {
    ++set_act;
    set_act_value = act;
}
extern "C" void MC_ResetRamSlot__Fv() { ++reset_slot; }

int main() {
    MC_NewGame__Fi(1);
    if (delayCount != -1 || errorCondition != 0 || actionCoolOff != 0 || curPlaySeconds != 0) return 1;
    if (quick != 1 || seconds_at_quick != 0 || set_act != 0 || reset_slot != 1) return 2;
    delayCount = 3;
    MC_NewGame__Fi(0);
    if (set_act != 1 || set_act_value != 0 || reset_slot != 2 || quick != 2) return 3;
    if (delayCount != -1) return 4;
    std::puts("new game passed");
    return 0;
}
