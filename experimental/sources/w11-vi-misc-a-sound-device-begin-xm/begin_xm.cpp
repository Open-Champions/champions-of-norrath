// Observed: BeginXmTransfer__13VISoundDevicePPvUiUiiPi at 0x01126f60 is 52 bytes.
// Arguments after the device: a pointer to a pointer P, unsigned A, unsigned B, an
// int I and an int pointer Q. With old = *P it stores, in device byte offsets:
// 3384 = I, 3380 = old, 3388 = -1, 3396 = A, 3400 = B, 3416 = Q, 3420 = 0,
// 3392 = A and 3412 = 0. It returns 0 and, unlike BeginAdpcmTransfer, leaves *P
// unchanged. Unknown: the meaning of the fields.
extern "C" {
int BeginXmTransfer__13VISoundDevicePPvUiUiiPi(void* device, void** source, unsigned first, unsigned second, int arg, int* status) {
    char* bytes = static_cast<char*>(device);
    void* old = *source;
    *reinterpret_cast<int*>(bytes + 3384) = arg;
    *reinterpret_cast<void**>(bytes + 3380) = old;
    *reinterpret_cast<int*>(bytes + 3388) = -1;
    *reinterpret_cast<unsigned*>(bytes + 3396) = first;
    *reinterpret_cast<unsigned*>(bytes + 3400) = second;
    *reinterpret_cast<int**>(bytes + 3416) = status;
    *reinterpret_cast<unsigned*>(bytes + 3420) = 0;
    *reinterpret_cast<unsigned*>(bytes + 3392) = first;
    *reinterpret_cast<unsigned*>(bytes + 3412) = 0;
    return 0;
}
}
