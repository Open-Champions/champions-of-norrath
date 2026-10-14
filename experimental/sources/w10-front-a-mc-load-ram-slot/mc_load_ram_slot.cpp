// Observed: MC_LoadRamSlot__Fv at 0x0025bfe8 is 28 bytes. It calls loadRamSlot__Fv once with
// no arguments and returns nothing.
// Inferred: nothing.
// Unknown: nothing.

extern "C" void loadRamSlot__Fv();

extern "C" void MC_LoadRamSlot__Fv() {
    loadRamSlot__Fv();
}
