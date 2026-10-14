// Observed: SetOutputMode__13VISoundDevicei at 0x01126718, 104 bytes. Arguments: the device and a mode. Stores the mode in the
// word at +3432, then walks the playback chain (FirstPlayback__13VISoundDevice, NextPlayback__13VISoundDeviceP10VIPlayback) and
// for each record calls SetPan__13VISoundDeviceif(device, word at record+32, float at record+44). The next record is fetched
// after SetPan returns.
// Inferred: the word at record+32 is the playback handle, and the float at +44 the pan it was last given, so changing the output
// mode re-applies every pan under the new mode.
// Unknown: the mode values, and the member names.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
static inline unsigned word_at(const unsigned char* base, int offset) { return *reinterpret_cast<const unsigned*>(base + offset); }
extern "C" {
unsigned FirstPlayback__13VISoundDevice(unsigned char* device);
unsigned NextPlayback__13VISoundDeviceP10VIPlayback(unsigned char* device, unsigned playback);
void SetPan__13VISoundDeviceif(unsigned char* device, int handle, float pan);

void SetOutputMode__13VISoundDevicei(unsigned char* device, int mode) {
    *reinterpret_cast<int*>(device + 3432) = mode;
    for (unsigned p = FirstPlayback__13VISoundDevice(device); p != 0; p = NextPlayback__13VISoundDeviceP10VIPlayback(device, p)) {
        unsigned char* record = bytes_at(p);
        SetPan__13VISoundDeviceif(device, *reinterpret_cast<int*>(record + 32), *reinterpret_cast<float*>(record + 44));
    }
}
}
