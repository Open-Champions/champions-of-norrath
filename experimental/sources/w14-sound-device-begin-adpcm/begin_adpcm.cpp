// Observed: BeginAdpcmTransfer__13VISoundDevicePPvUiiPi at 0x01126f30, 48 bytes. Arguments: the device, a pointer to a pointer
// (source), an unsigned size, an int, and a pointer to an int (fifth argument, read from the fifth argument register). It reads the
// pointer *source once (call it p), then stores on the device: +3380 = p, +3384 = fourth argument, +3388 = third argument, +3392 = -1,
// +3412 = 0, +3416 = fifth argument, +3420 = p. Finally *source = 0. It returns 0. It calls nothing.
// Inferred: this arms a transfer of the buffer p to sound memory that IdleTransfer__13VISoundDevice at 0x01125f70 later advances,
// and clears the caller's pointer to show the device took the buffer; +3392 = -1 reads as an unset position. The sibling
// BeginXmTransfer__13VISoundDevicePPvUiUiiPi at 0x01126f60 writes the same area with different slots.
// Unknown: the meaning of the individual fields (not examined here, since IdleTransfer is not yet authored) and the units of the sizes.
extern "C" {
int BeginAdpcmTransfer__13VISoundDevicePPvUiiPi(void* device, void** source, unsigned size, int argument, int* result) {
    unsigned char* base = static_cast<unsigned char*>(device);
    unsigned* slot = reinterpret_cast<unsigned*>(source);
    unsigned p = *slot;
    *reinterpret_cast<int*>(base + 3384) = argument;
    *reinterpret_cast<unsigned*>(base + 3420) = p;
    *reinterpret_cast<unsigned*>(base + 3388) = size;
    *reinterpret_cast<int*>(base + 3392) = -1;
    *reinterpret_cast<unsigned*>(base + 3416) = static_cast<unsigned>(reinterpret_cast<unsigned long>(result));
    *slot = 0;
    *reinterpret_cast<unsigned*>(base + 3380) = p;
    *reinterpret_cast<int*>(base + 3412) = 0;
    return 0;
}
}
