// Observed: ShareSound__13VISoundDevicei at 0x01126388, 44 bytes. Arguments: the device and an index. Loads the word at
// device + index * 4 + 2584 with no bounds check. If it is null the result is -1. Otherwise the word at +4 of that object is
// incremented and the result is 0.
// Inferred: the object is a sound entry from the table that Sound__13VISoundDevicei reads, and +4 is its use count, so this takes
// one more reference (ReleaseSound__13VISoundDeviceiR12VIDictionary would be the matching release; that is not read here).
// Unknown: the member names, and the table length.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
static inline unsigned word_at(const unsigned char* base, int offset) { return *reinterpret_cast<const unsigned*>(base + offset); }
extern "C" {
int ShareSound__13VISoundDevicei(unsigned char* device, int index) {
    unsigned sound = word_at(device, index * 4 + 2584);
    if (sound == 0) return -1;
    *reinterpret_cast<int*>(bytes_at(sound) + 4) += 1;
    return 0;
}
}
