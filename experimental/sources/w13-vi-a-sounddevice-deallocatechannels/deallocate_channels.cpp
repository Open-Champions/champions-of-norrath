// Observed: DeallocateChannels__13VISoundDeviceUl at 0x01126790 is 20 bytes. It loads the
// 64-bit word at offset 0 of the device, clears the bits that are set in the unsigned long
// argument (a1, complemented and ANDed) and stores the result back. Nothing is returned.
// The original leaves the new mask in v0 as an incidental value.
// Inferred: offset 0 is a 64-bit mask of allocated sound channels, one bit per channel; the
// argument is a mask of channels to release. Unknown: the channel count and how the bits
// are allocated (not part of this function).
extern "C" {
void DeallocateChannels__13VISoundDeviceUl(unsigned long long* device, unsigned long long mask) {
    device[0] &= ~mask;
}
}
