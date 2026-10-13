// Observed: Unload__13VIBattleMusic at 0x0106dd38 is 200 bytes. Reads, from the object: the
// owner pointer word at +0, +24 (an int sound id), +28 (an int playback id). Steps:
//   1. If VIGetFeatureFlags__Fv() & 0x100 is non-zero, return. If the owner (+0) is null,
//      return. device = *(owner + 19360); if null, return.
//   2. Calls VIGetFeatureFlags__Fv() a second time. If bit 0x100 is set it calls
//      StopBgm__13VISoundDevicei(device, 0); otherwise, if +28 is not -1, it calls
//      Stop__13VISoundDevicei(device, +28) and stores -1 at +28. (The first step returned
//      already when that bit was set, so the StopBgm path is only reachable if the flag word
//      changes between the two calls.)
//   3. It stores zero at +8. If +24 is non-negative it calls
//      ReleaseSound__13VISoundDeviceiR12VIDictionary(device, +24, *(owner + 19352)) and stores
//      -1 at +24.
//   4. It stores -1 at +4 and returns nothing. The early returns of step 1 leave the object
//      untouched.
// Inferred: this unloads a battle-music object: the owner holds the sound device at +19360 and
// the sound dictionary at +19352; +24 is a loaded sound, +28 a playing instance, +4 a state
// that is reset to -1.
// Unknown: the true field names, what +8 and +4 mean, and what feature bit 0x100 selects.
extern "C" {
int VIGetFeatureFlags__Fv();
void StopBgm__13VISoundDevicei(void* device, int slot);
void Stop__13VISoundDevicei(void* device, int id);
void ReleaseSound__13VISoundDeviceiR12VIDictionary(void* device, int sound, void* dictionary);
}

// A host check defines NORRATH_HOST_ARENA because a 64-bit host cannot keep target pointers in
// 4-byte words; the words then hold arena offsets.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* pointerAt(const unsigned char* p) {
    unsigned word = *reinterpret_cast<const unsigned*>(p);
    return word == 0 ? nullptr : norrath_host_pointer(word);
}
#else
static inline unsigned char* pointerAt(const unsigned char* p) {
    return *reinterpret_cast<unsigned char* const*>(p);
}
#endif

extern "C" {
void Unload__13VIBattleMusic(unsigned char* music) {
    if ((VIGetFeatureFlags__Fv() & 0x100) != 0) {
        return;
    }
    unsigned char* owner = pointerAt(music);
    if (owner == 0) {
        return;
    }
    unsigned char* device = pointerAt(owner + 19360);
    if (device == 0) {
        return;
    }
    if ((VIGetFeatureFlags__Fv() & 0x100) != 0) {
        StopBgm__13VISoundDevicei(device, 0);
    } else if (*reinterpret_cast<int*>(music + 28) != -1) {
        Stop__13VISoundDevicei(device, *reinterpret_cast<int*>(music + 28));
        *reinterpret_cast<int*>(music + 28) = -1;
    }
    *reinterpret_cast<int*>(music + 8) = 0;
    if (*reinterpret_cast<int*>(music + 24) >= 0) {
        ReleaseSound__13VISoundDeviceiR12VIDictionary(device, *reinterpret_cast<int*>(music + 24), pointerAt(owner + 19352));
        *reinterpret_cast<int*>(music + 24) = -1;
    }
    *reinterpret_cast<int*>(music + 4) = -1;
}
}
