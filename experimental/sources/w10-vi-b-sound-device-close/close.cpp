// Observed Close__13VISoundDevice at 0x01126348, 32 bytes. It calls
// Reset__13VISoundDevice with the same object and returns zero whatever the
// callee returned (the callee's result is not read).
extern "C" {
struct VISoundDevice;
void Reset__13VISoundDevice(VISoundDevice* device);

int Close__13VISoundDevice(VISoundDevice* device) {
    Reset__13VISoundDevice(device);
    return 0;
}
}
