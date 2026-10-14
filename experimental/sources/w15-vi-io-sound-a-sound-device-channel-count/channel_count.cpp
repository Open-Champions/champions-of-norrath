// Observed: ChannelCount__13VISoundDeviceUl at 0x011267a8, 56 bytes. Arguments: the device (unused) and a 64-bit mask. The result
// is the number of set bits among bits 0 to 45 of the mask; bits 46 to 63 are ignored. It reads and writes no memory.
// Inferred: the mask is a set of the 46 hardware channels (AllocateChannels__13VISoundDeviceii uses the same limit).
// Unknown: nothing beyond the count.
extern "C" {
int ChannelCount__13VISoundDeviceUl(unsigned char*, unsigned long long mask) {
    int count = 0;
    for (int bit = 0; bit < 46; ++bit) count += static_cast<int>((mask >> bit) & 1);
    return count;
}
}
