// Observed: NextChannel__13VISoundDeviceUli at 0x011268b8, 76 bytes. The device pointer is not used. Starting at index (third
// argument + 1), it tests bits in ascending order up to bit 45 of the 64-bit mask (second argument) and returns the index of the
// first set bit; it returns -1 when the start index is already 46 or more, or no set bit remains below 46.
// Inferred: this is the iteration step that follows FirstChannel__13VISoundDeviceUl at 0x01126870 over a 46-channel mask; the third
// argument is the previous channel index.
// Unknown: behaviour is as observed for a negative start; the starting index -1 is not known to be used by callers.
extern "C" {
int NextChannel__13VISoundDeviceUli(void* device, unsigned long long mask, int previous) {
    (void)device;
    for (int i = previous + 1; i < 46; ++i) {
        if (static_cast<int>((mask >> i) & 1)) return i;
    }
    return -1;
}
}
