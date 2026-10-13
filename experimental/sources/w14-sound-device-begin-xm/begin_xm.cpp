// Observed: BeginXmTransfer__13VISoundDevicePPvUiUiiPi at 0x01126f60, 52 bytes. Arguments: the device, a pointer to a pointer
// (source), unsigned a, unsigned b, an int, and a pointer to an int (the sixth argument). It reads *source once (call it p), then
// stores on the device: +3380 = p, +3384 = the int, +3388 = -1, +3392 = a, +3396 = a, +3400 = b, +3412 = 0, +3416 = the int pointer,
// +3420 = 0. *source is not written. It returns 0. It calls nothing.
// Inferred: this is the XM counterpart of BeginAdpcmTransfer__13VISoundDevicePPvUiiPi at 0x01126f30 and arms a transfer that
// IdleTransfer__13VISoundDevice at 0x01125f70 advances; unlike that sibling it leaves the caller's pointer in place.
// Unknown: meaning of the individual fields and what a and b are (size, offset or count), since IdleTransfer is not authored here.
extern "C" {
int BeginXmTransfer__13VISoundDevicePPvUiUiiPi(void* device, void** source, unsigned a, unsigned b, int value, int* result) {
    unsigned char* base = static_cast<unsigned char*>(device);
    unsigned p = *reinterpret_cast<unsigned*>(source);
    *reinterpret_cast<int*>(base + 3384) = value;
    *reinterpret_cast<unsigned*>(base + 3380) = p;
    *reinterpret_cast<int*>(base + 3388) = -1;
    *reinterpret_cast<unsigned*>(base + 3396) = a;
    *reinterpret_cast<unsigned*>(base + 3400) = b;
    *reinterpret_cast<unsigned*>(base + 3416) = static_cast<unsigned>(reinterpret_cast<unsigned long>(result));
    *reinterpret_cast<unsigned*>(base + 3420) = 0;
    *reinterpret_cast<unsigned*>(base + 3392) = a;
    *reinterpret_cast<int*>(base + 3412) = 0;
    return 0;
}
}
