// Observed: MC_GetRamSlot__FPi at 0x0025b398 is 32 bytes. When the argument is not null it
// stores the int 731592 (0x000b29c8) through it. It always returns the address of ramSlot
// (0x0081e240).
// Inferred: 731592 is the size of the ramSlot buffer that callers are allowed to use.
// Unknown: how that size was chosen and what lies after the buffer.

extern "C" unsigned char ramSlot[];

extern "C" unsigned char* MC_GetRamSlot__FPi(int* size) {
    if (size) *size = 731592;
    return ramSlot;
}
