// Observed: MC_ResetRamSlot__Fv at 0x0025bb70 is 52 bytes. When the signed byte
// MC_ProtectRamSlot is zero it calls memset on ramSlot with value 0 and length
// 0xb29c8 (727,496 bytes). A nonzero byte does nothing.
// Inferred: nothing.
// Unknown: the record layout inside ramSlot.

extern "C" void* memset(void* dest, int value, unsigned long count);
extern "C" signed char MC_ProtectRamSlot;
extern "C" char ramSlot[];

extern "C" void MC_ResetRamSlot__Fv() {
    if (MC_ProtectRamSlot == 0) {
        memset(ramSlot, 0, 0xb29c8);
    }
}
