// Observed: MC_Busy__Fv at 0x0025acd8 is 44 bytes. A nonzero disableMC
// returns 0. Otherwise an asyncMode other than -1 returns 1. Otherwise it
// returns 1 when errorCondition is nonzero and 0 when it is zero.
// Inferred: nothing.
// Unknown: which path a caller treats as busy.

extern "C" int disableMC;
extern "C" int asyncMode;
extern "C" int errorCondition;

extern "C" int MC_Busy__Fv() {
    if (disableMC != 0) return 0;
    if (asyncMode != -1) return 1;
    return errorCondition != 0 ? 1 : 0;
}
