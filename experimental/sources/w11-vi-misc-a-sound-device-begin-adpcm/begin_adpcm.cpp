// Observed: BeginAdpcmTransfer__13VISoundDevicePPvUiiPi at 0x01126f30 is 48 bytes.
// Arguments after the device: a pointer to a pointer P, an unsigned U, an int I and
// an int pointer Q. With old = *P it stores, in device byte offsets: 3384 = I,
// 3420 = old, 3388 = U, 3392 = -1, 3416 = Q, then *P = 0, then 3380 = old and
// 3412 = 0. It returns 0. Inferred: the source pointer is handed over to the device
// (and cleared in the caller) for a later IdleTransfer__13VISoundDevice step.
// Unknown: the field meanings; word 3420 is also written as a gain-like zero by
// Init__13VISoundDevice, so the same word may be reused.
extern "C" {
int BeginAdpcmTransfer__13VISoundDevicePPvUiiPi(void* device, void** source, unsigned size, int arg, int* status) {
    char* bytes = static_cast<char*>(device);
    void* old = *source;
    *reinterpret_cast<int*>(bytes + 3384) = arg;
    *reinterpret_cast<void**>(bytes + 3420) = old;
    *reinterpret_cast<unsigned*>(bytes + 3388) = size;
    *reinterpret_cast<int*>(bytes + 3392) = -1;
    *reinterpret_cast<int**>(bytes + 3416) = status;
    *source = nullptr;
    *reinterpret_cast<void**>(bytes + 3380) = old;
    *reinterpret_cast<unsigned*>(bytes + 3412) = 0;
    return 0;
}
}
