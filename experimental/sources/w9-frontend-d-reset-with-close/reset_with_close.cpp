// Observed: resetWithClose calls MCA_Reset, then reads MCA_FileOpen. A nonzero
// word calls MCA_Cmd(1, 0, 0, 0) and MCA_Sync on errorCondition. It returns
// nothing and does not touch asyncMode, syncState or errorCondition itself.
// This is the first half of stopCardQuery.
// Inferred: none.
// Unknown: what MCA_Reset and MCA_Sync change; what command 1 means.

extern "C" void MCA_Reset__Fv();
extern "C" void MCA_Cmd__Fiiii(int command, int text, int third, int fourth);
extern "C" void MCA_Sync__FPi(int* error);
extern "C" int MCA_FileOpen;
extern "C" int errorCondition;

extern "C" void resetWithClose__Fv() {
    MCA_Reset__Fv();
    if (MCA_FileOpen != 0) {
        MCA_Cmd__Fiiii(1, 0, 0, 0);
        MCA_Sync__FPi(&errorCondition);
    }
}
