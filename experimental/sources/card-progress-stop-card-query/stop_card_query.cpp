// Observed: stopCardQuery calls MCA_Reset, then reads MCA_FileOpen. A nonzero
// file-open word calls MCA_Cmd(1, 0, 0, 0) and MCA_Sync on errorCondition.
// After that it reloads asyncMode and syncState. asyncMode of -1 replaces
// syncState with zero. errorCondition is then stored as zero. The return
// register is the bitwise complement of the reloaded asyncMode.
// Inferred: none.
// Unknown: what MCA_Reset and MCA_Sync change besides the words reloaded here.

extern "C" void MCA_Reset__Fv();
extern "C" void MCA_Cmd__Fiiii(int command, int text, int third, int fourth);
extern "C" void MCA_Sync__FPi(int* error);
extern "C" int MCA_FileOpen;
extern "C" int asyncMode;
extern "C" int syncState;
extern "C" int errorCondition;

extern "C" int stopCardQuery__Fv() {
    MCA_Reset__Fv();
    if (MCA_FileOpen != 0) {
        MCA_Cmd__Fiiii(1, 0, 0, 0);
        MCA_Sync__FPi(&errorCondition);
    }
    int mode = asyncMode;
    int state = syncState;
    if (mode == -1) {
        state = 0;
    }
    errorCondition = 0;
    syncState = state;
    return ~mode;
}
