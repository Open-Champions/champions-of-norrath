// Observed: Process__13VIBattleMusic at 0x0106da90, 436 bytes. It returns 0, except -1 when the sound index at +24 is -1 in
// step 2. Words are indexed by byte offset from the object (music); floats are at +12 and +16. In order:
//  1. If VIGetFeatureFlags__Fv() has bit 0x100, return. scene = word at +0 (null: return); device = word at scene+19360 (null: return).
//     If the word at +4 is 0, return.
//  2. State is the word at +8. State 1 (start requested):
//     - if the playback id at +28 is -1: let index = word at +24; if that is -1, return -1. Look up the sound with
//       Sound__13VISoundDevicei(device, index) and call the virtual member at byte offset 48 of its table (the table is the pointer at
//       sound+12; each table entry is a halfword this-adjustment, a halfword pad and a function pointer; the adjusted this is
//       sound + adjustment). In both VIWave and VIXm tables that entry is the one for IsCached__6VIWave and IsCached__4VIXm.
//       If the call returns 0, return. Otherwise id = Play__13VISoundDeviceii12VISOUNDCLASS(device, index, 0, 1), store id at +28
//       and call SetVolume__13VISoundDeviceif(device, id, float at +12).
//     - volume (+12) += step (+16), stored back. If 1.0 <= volume, store 1.0 there and 3 at +8. Then call SetVolume(device, id, volume)
//       with the id read before the possible Play (the id from +28 when it already existed, else the newly stored one).
//     State 2 (fading out): s = float(+16) * 0.1 (bit pattern 0x3dcccccc); if the word at +20 is nonzero, s *= 10.0; volume = float(+12) - s,
//     stored at +12. If volume <= 0.0, call StopNow__13VIBattleMusic(music) and store 0 at +8; else call SetVolume(device, id at +28, volume).
//     Any other state (0 and 3 upward) does nothing.
// Inferred: +4 track, +8 state, +12 volume, +16 per-step fade amount, +20 stop-immediately bool, +24 sound index, +28 playback id, as in
// Play__13VIBattleMusici and Stop__13VIBattleMusicb. The fade-out step is scaled by 0.1 per call, or by 1.0 when the bool is set.
// Unknown: the time base of a "step", what IsCached means to this caller, and the VISOUNDCLASS value 1.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(norrath_host_pointer(address)); }
#else
static inline unsigned* words_at(unsigned address) { return reinterpret_cast<unsigned*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
unsigned VIGetFeatureFlags__Fv();
unsigned Sound__13VISoundDevicei(unsigned device, int index);
int Play__13VISoundDeviceii12VISOUNDCLASS(unsigned device, int index, int unused, int soundClass);
void SetVolume__13VISoundDeviceif(unsigned device, int id, float volume);
void StopNow__13VIBattleMusic(void* music);
}

static inline float float_at(const unsigned* words, int index) {
    return __builtin_bit_cast(float, words[index]);
}
static inline void store_float(unsigned* words, int index, float value) {
    words[index] = __builtin_bit_cast(unsigned, value);
}

// One table entry of the sound's virtual table: halfword adjustment, halfword pad, function pointer.
struct SoundTableEntry {
    short adjustment;
    short pad;
    unsigned function;
};

#ifdef NORRATH_HOST_ARENA
extern "C" int norrath_host_call_is_cached(unsigned sound, unsigned function);
static inline int call_entry(unsigned sound, const SoundTableEntry* entry) {
    return norrath_host_call_is_cached(sound + entry->adjustment, entry->function);
}
#else
static inline int call_entry(unsigned sound, const SoundTableEntry* entry) {
    typedef int (*Fn)(unsigned long);
    return reinterpret_cast<Fn>(static_cast<unsigned long>(entry->function))(static_cast<unsigned long>(sound + entry->adjustment));
}
#endif

extern "C" {
int Process__13VIBattleMusic(void* music) {
    unsigned* m = static_cast<unsigned*>(music);
    if (VIGetFeatureFlags__Fv() & 0x100) return 0;
    unsigned scene = m[0];
    if (!scene) return 0;
    unsigned device = words_at(scene)[19360 / 4];
    if (!device) return 0;
    if (m[1] == 0) return 0;
    unsigned state = m[2];
    if (state == 1) {
        int id = static_cast<int>(m[7]);
        if (id == -1) {
            int index = static_cast<int>(m[6]);
            if (index == -1) return -1;
            unsigned sound = Sound__13VISoundDevicei(device, index);
            const SoundTableEntry* entry =
                reinterpret_cast<const SoundTableEntry*>(words_at(words_at(sound)[3]) + 12);
            if (!call_entry(sound, entry)) return 0;
            id = Play__13VISoundDeviceii12VISOUNDCLASS(device, static_cast<int>(m[6]), 0, 1);
            m[7] = static_cast<unsigned>(id);
            SetVolume__13VISoundDeviceif(device, id, float_at(m, 3));
            id = static_cast<int>(m[7]);
        }
        float volume = float_at(m, 3) + float_at(m, 4);
        store_float(m, 3, volume);
        if (1.0f <= volume) {
            store_float(m, 3, 1.0f);
            m[2] = 3;
        }
        SetVolume__13VISoundDeviceif(device, id, float_at(m, 3));
        return 0;
    }
    if (state == 2) {
        float step = float_at(m, 4) * __builtin_bit_cast(float, 0x3dccccccu);
        float volume;
        if (m[5]) volume = float_at(m, 3) - step * 10.0f;
        else volume = float_at(m, 3) - step;
        store_float(m, 3, volume);
        if (volume <= 0.0f) {
            StopNow__13VIBattleMusic(music);
            m[2] = 0;
        } else {
            SetVolume__13VISoundDeviceif(device, static_cast<int>(m[7]), volume);
        }
    }
    return 0;
}
}
