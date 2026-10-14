// Observed: MC_GetLoadName__Fv at 0x0025c008 is 12 bytes. It returns the address of loadName
// (0x008d14a0) and reads nothing.
// Inferred: callers use the result as the level-name string that loadRamSlot__Fv composes there.
// Unknown: the capacity of loadName.

extern "C" char loadName[];

extern "C" char* MC_GetLoadName__Fv() {
    return loadName;
}
