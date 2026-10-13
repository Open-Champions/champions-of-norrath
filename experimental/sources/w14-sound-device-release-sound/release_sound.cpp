// Observed: ReleaseSound__13VISoundDeviceiR12VIDictionary at 0x011263b8, 260 bytes. Arguments: the device, a sound index and a
// dictionary reference. The sound pointer is the word at device + 2584 + 4 * index (the slot read by Sound__13VISoundDevicei). It
// returns an int:
//  1. n = word at sound+4, minus one, stored back at once. If n > 0 return 0.
//  2. If the word at sound+0 is nonzero, call Remove__12VIDictionaryR10VIResource(dictionary, sound); if that returns a negative
//     number, return -1.
//  3. Walk the playback records (FirstPlayback__13VISoundDevice, then NextPlayback__13VISoundDeviceP10VIPlayback fetched before the
//     record is used): for each record whose word at +28 equals the index, call Stop__13VISoundDevicei(device, record word at +32).
//  4. Call the virtual member at byte offset 8 of the sound's table (the table is the pointer at sound+12; an entry is a halfword
//     adjustment, a halfword pad and a function pointer), with this = sound + adjustment and second argument 3. Then clear the slot at
//     device + 2584 + 4 * index and return 0.
// Inferred: sound+4 is a reference count, sound+0 a dictionary link, and the entry at offset 8 is the destructor taking a delete flag
// (in the VIWave and VIXm tables it points at _$_6VIWave and _$_4VIXm). Playback record +28 is the owning sound index.
// Unknown: what the first word of the sound really is and the dictionary's removal semantics.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(norrath_host_pointer(address)); }
extern "C" void norrath_host_call_destructor(unsigned self, unsigned function, int flags);
static inline void call_destructor(unsigned self, unsigned function, int flags) { norrath_host_call_destructor(self, function, flags); }
#else
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }
static inline void call_destructor(unsigned self, unsigned function, int flags) {
    typedef void (*Fn)(unsigned long, int);
    reinterpret_cast<Fn>(static_cast<unsigned long>(function))(static_cast<unsigned long>(self), flags);
}
#endif

extern "C" {
unsigned FirstPlayback__13VISoundDevice(void* device);
unsigned NextPlayback__13VISoundDeviceP10VIPlayback(void* device, unsigned playback);
void Stop__13VISoundDevicei(void* device, int voice);
int Remove__12VIDictionaryR10VIResource(void* dictionary, unsigned resource);

int ReleaseSound__13VISoundDeviceiR12VIDictionary(void* device, int index, void* dictionary) {
    unsigned* slot = reinterpret_cast<unsigned*>(static_cast<unsigned char*>(device) + 2584) + index;
    unsigned sound = *slot;
    unsigned* s = words_at(sound);
    int count = static_cast<int>(s[1]) - 1;
    s[1] = static_cast<unsigned>(count);
    if (count > 0) return 0;
    if (s[0] != 0) {
        if (Remove__12VIDictionaryR10VIResource(dictionary, sound) < 0) return -1;
    }
    for (unsigned p = FirstPlayback__13VISoundDevice(device); p;) {
        unsigned next = NextPlayback__13VISoundDeviceP10VIPlayback(device, p);
        const unsigned* record = words_at(p);
        if (static_cast<int>(record[28 / 4]) == index) Stop__13VISoundDevicei(device, static_cast<int>(record[32 / 4]));
        p = next;
    }
    const unsigned* entry = words_at(s[3]) + 2;
    call_destructor(sound + static_cast<short>(entry[0]), entry[1], 3);
    *slot = 0;
    return 0;
}
}
