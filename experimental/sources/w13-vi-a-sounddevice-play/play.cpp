// Observed: Play__13VISoundDeviceii12VISOUNDCLASS at 0x01124e50 is 1104 bytes. Arguments: device
// (a0), sound index (a1), an int arg2 (a2) and a class value cls (a3, an enum). The result is an
// int: a playback id, or a negative status.
//   1. cls >= 2 returns -1 (signed compare); sound index -1 returns -1; the sound is the word at
//      device + 2584 + 4 * index; a null sound returns -1.
//   2. A virtual call on the sound subobject (entry at (word at sound+12) + 48, this = sound +
//      the signed 16-bit value at entry+0, function = word at entry+4, int result) returning 0
//      makes the function return -2.
//   3. channels = ChannelCount__C6VIWave(AsWave__7VISound(sound)) when Type__C7VISound(sound) ==
//      0, else ChannelCount__C4VIXm(AsXm__7VISound(sound)). mask = AllocateChannels__13VISoundDeviceii(
//      device, channels, arg2) (64-bit); a zero mask returns -1.
//   4. The first free slot i (word == 0) of the 46 words at device + 3100 is used: id = i |
//      ((counter << 8) & 0x7fffff00) with counter the int at device+3308, which is incremented;
//      pb = NewPlayback__13VISoundDevice(device) is stored in the slot. With no free slot it
//      returns -1.
//   5. pb fields: +24 = arg2, +28 = sound index, +32 = id, +16 = mask (64-bit), +52 = cls and
//      +48 = the int at device+3096.
//   6. If Type__C7VISound(sound) == 0 (wave): first = FirstChannel__13VISoundDeviceUl(device,
//      mask), next = NextChannel__13VISoundDeviceUli(device, mask, first); pb+36 = 4096, pb+40 =
//      1.0f; f = a virtual float call on the wave (entry at (word at wave+12) + 32, as above)
//      stored at pb+44; data (sIopFunctionData) is filled: [0] = first, [1] = the int at wave+60,
//      [2] = next, [3] = next == -1 ? 0 : wave[60] + (wave[56] >> 1), [4] = (int)((long long)
//      wave[32] * 4096 / 48000), [5] = (unsigned)(pb[40] * 16383.0f * (g * base)) with base a
//      second virtual float call (entry + 16) and g the float at device + 3424 + 4 * cls, [6] =
//      the int at device+3432 != 0 ? (int)(f * 16383.0f) : 0; then SetIopFunction__FUii(2, 7).
//   7. Otherwise, if Type__C7VISound(sound) is not 1 it returns -1; for 1 (XM): f = a virtual float
//      call on xm (entry + 32) stored at pb+44, pb+36 = 4096, pb+40 = 1.0f; data[0] = low 24 bits
//      of mask, [1] = bits 24..55 (arithmetic shift of mask << 8), [2] = the int at xm+24, [3] =
//      the int at xm+52, [4] = pb[36], [5] = (unsigned)(pb[40] * 255.0f * (g * base)) with base a
//      second virtual float call (entry + 16), [6] = flag ? (int)((f * 0.5f + 0.5f) * 255.0f) : 0
//      with flag = the int at device+3432; then SetIopFunction__FUii(8, 7).
//   8. In both successful cases it returns the id.
// Inferred: starts playing a loaded sound: claims channels, registers a playback record in the
// 46-entry table keyed by id, and forwards the start request (channels, volume, pan, rate) to the
// IOP; waves go to IOP function 2 and XM modules to function 8. cls selects a per-class volume (g)
// and the 4096 at pb+36 looks like a 1.0 pitch in 12-bit fixed point (see SetPitch).
// Unknown: the enum values of VISOUNDCLASS (only cls < 2 is accepted), the names of the vtable
// methods (+16 returns a base volume, +32 a pan, +48 a readiness flag), the meaning of arg2 and of
// pb+48, wave[32/56/60] and xm[24/52], and the names of IOP functions 2 and 8. The playback
// record leaks in step 7 when the sound has an unexpected type.
extern "C" {
int Type__C7VISound(const void* sound);
void* AsWave__7VISound(void* sound);
void* AsXm__7VISound(void* sound);
int ChannelCount__C6VIWave(const void* wave);
int ChannelCount__C4VIXm(const void* xm);
unsigned long long AllocateChannels__13VISoundDeviceii(void* device, int channels, int arg);
void* NewPlayback__13VISoundDevice(void* device);
int FirstChannel__13VISoundDeviceUl(void* device, unsigned long long mask);
int NextChannel__13VISoundDeviceUli(void* device, unsigned long long mask, int channel);
void SetIopFunction__FUii(unsigned function, int count);
extern int norrath_overlay_sIopFunctionData_11edf08[];
}

// Words that hold 32-bit target addresses: a host check defines NORRATH_HOST_ARENA, so they hold
// arena offsets / function ids and the helpers below are provided by the check.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
extern "C" unsigned norrath_host_word(const void* pointer);
#define WORD_POINTER(word) norrath_host_pointer(word)
#define WORD_FUNCTION(word) norrath_host_function(word)
#define POINTER_WORD(pointer) norrath_host_word(pointer)
#else
#define WORD_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define WORD_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#define POINTER_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#endif

typedef float (*FloatMethod)(void* self);
typedef int (*IntMethod)(void* self);

static inline unsigned char* vtableEntry(unsigned char* object, int offset) {
    return WORD_POINTER(*reinterpret_cast<unsigned*>(object + 12)) + offset;
}
static inline float callVirtualFloat(unsigned char* object, int offset) {
    unsigned char* entry = vtableEntry(object, offset);
    FloatMethod method = reinterpret_cast<FloatMethod>(WORD_FUNCTION(*reinterpret_cast<unsigned*>(entry + 4)));
    return method(object + *reinterpret_cast<short*>(entry));
}
static inline int callVirtualInt(unsigned char* object, int offset) {
    unsigned char* entry = vtableEntry(object, offset);
    IntMethod method = reinterpret_cast<IntMethod>(WORD_FUNCTION(*reinterpret_cast<unsigned*>(entry + 4)));
    return method(object + *reinterpret_cast<short*>(entry));
}

extern "C" {
int Play__13VISoundDeviceii12VISOUNDCLASS(unsigned char* device, int index, int arg2, int cls) {
    if (cls >= 2) {
        return -1;
    }
    if (index == -1) {
        return -1;
    }
    unsigned char* sound = WORD_POINTER(*reinterpret_cast<unsigned*>(device + 2584 + 4 * index));
    if (sound == 0) {
        return -1;
    }
    if (callVirtualInt(sound, 48) == 0) {
        return -2;
    }
    int channels;
    if (Type__C7VISound(sound) == 0) {
        channels = ChannelCount__C6VIWave(AsWave__7VISound(sound));
    } else {
        channels = ChannelCount__C4VIXm(AsXm__7VISound(sound));
    }
    unsigned long long mask = AllocateChannels__13VISoundDeviceii(device, channels, arg2);
    if (mask == 0) {
        return -1;
    }
    int id = -1;
    unsigned char* pb = 0;
    unsigned char* slot = device + 3100;
    for (int i = 0; i < 46; ++i, slot += 4) {
        if (*reinterpret_cast<unsigned*>(slot) == 0) {
            int counter = *reinterpret_cast<int*>(device + 3308);
            *reinterpret_cast<int*>(device + 3308) = counter + 1;
            id = i | ((counter << 8) & 0x7fffff00);
            pb = static_cast<unsigned char*>(NewPlayback__13VISoundDevice(device));
            *reinterpret_cast<unsigned*>(slot) = POINTER_WORD(pb);
            break;
        }
    }
    if (id == -1) {
        return -1;
    }
    *reinterpret_cast<int*>(pb + 24) = arg2;
    *reinterpret_cast<int*>(pb + 28) = index;
    *reinterpret_cast<int*>(pb + 32) = id;
    *reinterpret_cast<unsigned long long*>(pb + 16) = mask;
    int word3096 = *reinterpret_cast<int*>(device + 3096);
    *reinterpret_cast<int*>(pb + 52) = cls;
    *reinterpret_cast<int*>(pb + 48) = word3096;
    int* data = norrath_overlay_sIopFunctionData_11edf08;
    if (Type__C7VISound(sound) == 0) {
        unsigned char* wave = static_cast<unsigned char*>(AsWave__7VISound(sound));
        int first = FirstChannel__13VISoundDeviceUl(device, mask);
        int next = NextChannel__13VISoundDeviceUli(device, mask, first);
        *reinterpret_cast<int*>(pb + 36) = 4096;
        *reinterpret_cast<float*>(pb + 40) = 1.0f;
        float f = callVirtualFloat(wave, 32);
        *reinterpret_cast<float*>(pb + 44) = f;
        data[0] = first;
        float scaledPan = f * 16383.0f;
        data[2] = next;
        data[1] = *reinterpret_cast<int*>(wave + 60);
        int panValue = static_cast<int>(scaledPan);
        data[3] = next == -1 ? 0 : *reinterpret_cast<int*>(wave + 60) + (*reinterpret_cast<int*>(wave + 56) >> 1);
        data[4] = static_cast<int>((static_cast<long long>(*reinterpret_cast<int*>(wave + 32)) << 12) / 48000);
        float base = callVirtualFloat(wave, 16);
        float volume = *reinterpret_cast<float*>(pb + 40) * 16383.0f;
        float group = *reinterpret_cast<float*>(device + 3424 + 4 * cls) * base;
        data[5] = static_cast<int>(static_cast<unsigned>(volume * group));
        data[6] = *reinterpret_cast<int*>(device + 3432) != 0 ? panValue : 0;
        SetIopFunction__FUii(2, 7);
    } else {
        if (Type__C7VISound(sound) != 1) {
            return -1;
        }
        unsigned char* xm = static_cast<unsigned char*>(AsXm__7VISound(sound));
        *reinterpret_cast<int*>(pb + 36) = 4096;
        *reinterpret_cast<float*>(pb + 40) = 1.0f;
        float f = callVirtualFloat(xm, 32);
        long long mask64 = static_cast<long long>(mask);
        data[1] = static_cast<int>(mask64 << 8 >> 32);
        data[0] = static_cast<int>(mask64 & 0xffffff);
        float panned = f * 0.5f;
        data[2] = *reinterpret_cast<int*>(xm + 24);
        panned = panned + 0.5f;
        data[3] = *reinterpret_cast<int*>(xm + 52);
        panned = panned * 255.0f;
        *reinterpret_cast<float*>(pb + 44) = f;
        int panValue = static_cast<int>(panned);
        data[4] = *reinterpret_cast<int*>(pb + 36);
        float base = callVirtualFloat(xm, 16);
        float volume = *reinterpret_cast<float*>(pb + 40) * 255.0f;
        float group = *reinterpret_cast<float*>(device + 3424 + 4 * cls) * base;
        data[5] = static_cast<int>(static_cast<unsigned>(volume * group));
        data[6] = *reinterpret_cast<int*>(device + 3432) != 0 ? panValue : 0;
        SetIopFunction__FUii(8, 7);
    }
    return id;
}
}
