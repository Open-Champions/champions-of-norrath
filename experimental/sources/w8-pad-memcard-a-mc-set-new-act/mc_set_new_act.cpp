// Observed: MC_SetNewAct__Fi at 0x0025b248 is 80 bytes. If the signed byte
// MC_SavingOrLoading is zero, it calls engineTaskActive__FPc with a pointer to a
// string at 0x00474cc8 (the text "deathTask"). A zero result calls
// MC_ResetRamSlot__Fv and then jumpGateHubReset__Fi(act). In every case it
// finally stores the argument in curAct.
// Inferred: nothing.
// Unknown: what the argument means beyond the act number used by curAct.

extern "C" int engineTaskActive__FPc(char* name);
extern "C" void MC_ResetRamSlot__Fv();
extern "C" void jumpGateHubReset__Fi(int act);
extern "C" signed char MC_SavingOrLoading;
extern "C" int curAct;

extern "C" void MC_SetNewAct__Fi(int act) {
    if (MC_SavingOrLoading == 0) {
        if (engineTaskActive__FPc(const_cast<char*>("deathTask")) == 0) {
            MC_ResetRamSlot__Fv();
            jumpGateHubReset__Fi(act);
        }
    }
    curAct = act;
}
