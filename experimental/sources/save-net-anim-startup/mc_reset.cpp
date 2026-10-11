// Observed: MC_Reset calls MCA_ClearInfo, then stores -1 in MCA_Slot and
// MCA_Port and returns -1.
// Inferred: nothing about MCA_ClearInfo.
// Unknown: which later read consumes the two words.

extern "C" void MCA_ClearInfo__Fv();
extern "C" int MCA_Slot;
extern "C" int MCA_Port;

extern "C" int MC_Reset__Fv() {
    MCA_ClearInfo__Fv();
    MCA_Slot = -1;
    MCA_Port = -1;
    return -1;
}
