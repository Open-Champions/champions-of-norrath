// Observed: MC_NewGame__Fi at 0x0025ac68 is 80 bytes. It stores -1 in
// delayCount and 0 in errorCondition, actionCoolOff, and curPlaySeconds,
// then calls resetQuickLoad. A zero argument then calls MC_SetNewAct(0).
// It always calls MC_ResetRamSlot after that.
// Inferred: nothing about the three callees.
// Unknown: the result left in the return register by MC_ResetRamSlot.

extern "C" void resetQuickLoad__Fv();
extern "C" void MC_SetNewAct__Fi(int act);
extern "C" void MC_ResetRamSlot__Fv();
extern "C" int delayCount;
extern "C" int errorCondition;
extern "C" int actionCoolOff;
extern "C" int curPlaySeconds;

extern "C" void MC_NewGame__Fi(int argument) {
    delayCount = -1;
    errorCondition = 0;
    actionCoolOff = 0;
    curPlaySeconds = 0;
    resetQuickLoad__Fv();
    if (argument == 0) MC_SetNewAct__Fi(0);
    MC_ResetRamSlot__Fv();
}
