// Observed: jumpGateHubReset__Fi at 0x002846b8 is 64 bytes. If the signed byte
// MC_SavingOrLoading is zero, it copies the string pointed to by hubNames[act]
// into jumpGateEntries with strcpy and then stores 1 in the signed byte
// jumpGateDestCount. The store of the count sits in the call's delay slot, so it
// happens before the copy runs. Otherwise it does nothing. hubNames is read as
// an array of 4-byte pointers indexed by the argument; there is no range check.
// Inferred: nothing.
// Unknown: the number of hub names and the size of jumpGateEntries here.

extern "C" char* strcpy(char* dst, const char* src);
extern "C" signed char MC_SavingOrLoading;
extern "C" signed char jumpGateDestCount;
extern "C" char jumpGateEntries[];
extern "C" const char* hubNames[];

extern "C" void jumpGateHubReset__Fi(int act) {
    if (MC_SavingOrLoading == 0) {
        jumpGateDestCount = 1;
        strcpy(jumpGateEntries, hubNames[act]);
    }
}
