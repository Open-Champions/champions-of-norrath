// Observed Process__13VISoundDevice at 0x01124b40, 8 bytes. It returns zero
// without reading or writing anything.
extern "C" {
struct VISoundDevice;
int Process__13VISoundDevice(VISoundDevice* device) {
    (void)device;
    return 0;
}
}
