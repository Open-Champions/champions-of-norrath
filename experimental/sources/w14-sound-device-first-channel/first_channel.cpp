// Observed: FirstChannel__13VISoundDeviceUl at 0x01126870, 68 bytes. The device pointer is not used. The second argument is a 64-bit
// mask (the image's unsigned long is eight bytes; here it is written as unsigned long long). The routine tests bits 0 to 45 in
// ascending order and returns the index of the first set bit, or -1 when none of those 46 bits is set. Bits 46 and above are ignored.
// Inferred: a mask of this shape is a set of sound channels, 46 of them; ChannelCount__13VISoundDeviceUl at 0x011267a8 counts the same
// 46 bits, and the device keeps such a mask in its first doubleword (AllocateChannels reads it with a 64-bit load).
// Unknown: why 46 and what the channels map to in hardware.
extern "C" {
int FirstChannel__13VISoundDeviceUl(void* device, unsigned long long mask) {
    (void)device;
    for (int i = 0; i < 46; ++i) {
        if (static_cast<int>((mask >> i) & 1)) return i;
    }
    return -1;
}
}
