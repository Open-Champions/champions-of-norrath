#include "mc_set_new_act.cpp"

#include <cstdio>
#include <cstring>

signed char MC_SavingOrLoading;
int curAct = -1;
static int active, queries, resets, gates, gate_act, gate_cur;
static char name[16];
extern "C" int engineTaskActive__FPc(char* n) { ++queries; std::strcpy(name, n); return active; }
extern "C" void MC_ResetRamSlot__Fv() { ++resets; }
extern "C" void jumpGateHubReset__Fi(int a) { ++gates; gate_act = a; gate_cur = curAct; }

int main() {
    MC_SavingOrLoading = 1;
    MC_SetNewAct__Fi(3);
    if (queries != 0 || curAct != 3) return 1;
    MC_SavingOrLoading = 0;
    active = 1;
    MC_SetNewAct__Fi(4);
    if (queries != 1 || std::strcmp(name, "deathTask") != 0) return 2;
    if (resets != 0 || gates != 0 || curAct != 4) return 3;
    active = 0;
    MC_SetNewAct__Fi(2);
    if (resets != 1 || gates != 1 || gate_act != 2) return 4;
    if (gate_cur != 4 || curAct != 2) return 5;  // curAct is stored after the reset
    std::puts("mc set new act passed");
    return 0;
}
