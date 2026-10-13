// Observed: SetPan__13VISoundDeviceif at 0x011257a0 is 660 bytes. It is the counterpart of
// SetVolume__13VISoundDeviceif (same file layout, same helpers) with the roles of the two float
// fields exchanged. Arguments: device (a0), playback id (a1) and a float pan (f12). The pan is
// limited to [-1.0f, 1.0f] (max with -1.0f, then min with 1.0f). pb =
// Playback__13VISoundDevicei(device, id); a null pb ends the call. Otherwise the limited pan is
// stored as a float at pb+44, and the sound is the word at device + 2584 + 4 * (the int at
// pb+28), or none when that int is -1; no sound ends the call.
// Let base be the float returned by a virtual call on a sound subobject (see below), volume the
// float stored at pb+40, g the float at device + 3424 + 4 * (the int at pb+52), pan the float at
// pb+44 (re-read), mask the 64-bit word at pb+16 and flag the int at device+3432.
//   - If Type__C7VISound(sound) == 0: wave = AsWave__7VISound(sound); base = the method found at
//     vtable entry 1 of wave; panValue = (int)(pan * 16383.0f);
//     data[0] = FirstChannel__13VISoundDeviceUl(device, mask); data[1] =
//     NextChannel__13VISoundDeviceUli(device, mask, data[0]); data[2] = (unsigned)(base * volume * g
//     * 16383.0f); data[3] = flag ? panValue : 0 (data is sIopFunctionData); then
//     SetIopFunction__FUii(6, 4).
//   - Then Type__C7VISound(sound) is called again; when it returns 1: xm = AsXm__7VISound(sound);
//     base from the same virtual call on xm; data[0] = the low 24 bits of mask, data[1] = bits
//     24..55 (arithmetic shift of mask << 8), data[2] = (unsigned)(base * volume * g * 255.0f),
//     data[3] = flag ? (int)((pan * 0.5f + 0.5f) * 255.0f) : 0; then SetIopFunction__FUii(10, 4).
// The virtual call: entry = (the word at object+12) + 16; it calls the function whose address is
// the word at entry+4 with this = object + the signed 16-bit value at entry+0, and the function
// returns a float. The float to unsigned conversions use the usual two-way conversion
// (values >= 2^31 are converted after subtracting 2^31 and the top bit is or-ed back in).
// Inferred: the stored volume and pan are forwarded to the sound back end as in SetVolume: the
// pan in -1..1 maps to a signed 14-bit value for waves and to 0..255 for XM; the 16383 and 255
// scales are observed constants. device+3424 is a table of per-group float volumes indexed by
// pb+52 and device+3432 selects whether a pan value is sent.
// Unknown: the name of the virtual method, the names of IOP functions 6 and 10, and the meaning
// of pb+52.
extern "C" {
void* Playback__13VISoundDevicei(void* device, int id);
int Type__C7VISound(const void* sound);
void* AsWave__7VISound(void* sound);
void* AsXm__7VISound(void* sound);
int FirstChannel__13VISoundDeviceUl(void* device, unsigned long long mask);
int NextChannel__13VISoundDeviceUli(void* device, unsigned long long mask, int channel);
void SetIopFunction__FUii(unsigned function, int count);
extern int norrath_overlay_sIopFunctionData_11edf08[];
}

// Words that hold 32-bit target addresses: a host check defines NORRATH_HOST_ARENA, so they hold
// arena offsets / function ids and the two helpers below are provided by the check.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
#define WORD_POINTER(word) norrath_host_pointer(word)
#define WORD_FUNCTION(word) norrath_host_function(word)
#else
#define WORD_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define WORD_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

typedef float (*FloatMethod)(void* self);

static inline float callVirtualFloat(unsigned char* object) {
    unsigned char* entry = WORD_POINTER(*reinterpret_cast<unsigned*>(object + 12)) + 16;
    FloatMethod method = reinterpret_cast<FloatMethod>(WORD_FUNCTION(*reinterpret_cast<unsigned*>(entry + 4)));
    return method(object + *reinterpret_cast<short*>(entry));
}

extern "C" {
void SetPan__13VISoundDeviceif(unsigned char* device, int id, float pan) {
    pan = pan > -1.0f ? pan : -1.0f;
    pan = pan < 1.0f ? pan : 1.0f;
    unsigned char* pb = static_cast<unsigned char*>(Playback__13VISoundDevicei(device, id));
    if (pb == 0) {
        return;
    }
    int slot = *reinterpret_cast<int*>(pb + 28);
    *reinterpret_cast<float*>(pb + 44) = pan;
    unsigned char* sound = 0;
    if (slot != -1) {
        sound = WORD_POINTER(*reinterpret_cast<unsigned*>(device + 2584 + 4 * slot));
    }
    if (sound == 0) {
        return;
    }
    int* data = norrath_overlay_sIopFunctionData_11edf08;
    if (Type__C7VISound(sound) == 0) {
        unsigned char* wave = static_cast<unsigned char*>(AsWave__7VISound(sound));
        float scaled = callVirtualFloat(wave) * *reinterpret_cast<float*>(pb + 40);
        pan = *reinterpret_cast<float*>(pb + 44);
        float group = *reinterpret_cast<float*>(device + 3424 + 4 * *reinterpret_cast<int*>(pb + 52));
        scaled = scaled * group;
        int panValue = static_cast<int>(pan * 16383.0f);
        unsigned long long mask = *reinterpret_cast<unsigned long long*>(pb + 16);
        data[0] = FirstChannel__13VISoundDeviceUl(device, mask);
        int next = NextChannel__13VISoundDeviceUli(device, mask, data[0]);
        data[1] = next;
        data[2] = static_cast<int>(static_cast<unsigned>(scaled * 16383.0f));
        data[3] = *reinterpret_cast<int*>(device + 3432) != 0 ? panValue : 0;
        SetIopFunction__FUii(6, 4);
    }
    if (Type__C7VISound(sound) == 1) {
        unsigned char* xm = static_cast<unsigned char*>(AsXm__7VISound(sound));
        float scaled = callVirtualFloat(xm) * *reinterpret_cast<float*>(pb + 40);
        pan = *reinterpret_cast<float*>(pb + 44);
        float group = *reinterpret_cast<float*>(device + 3424 + 4 * *reinterpret_cast<int*>(pb + 52));
        long long mask = *reinterpret_cast<long long*>(pb + 16);
        float panned = pan * 0.5f + 0.5f;
        scaled = scaled * group;
        data[0] = static_cast<int>(mask & 0xffffff);
        data[1] = static_cast<int>(mask << 8 >> 32);
        scaled = scaled * 255.0f;
        panned = panned * 255.0f;
        int panValue = static_cast<int>(panned);
        data[2] = static_cast<int>(static_cast<unsigned>(scaled));
        data[3] = *reinterpret_cast<int*>(device + 3432) != 0 ? panValue : 0;
        SetIopFunction__FUii(10, 4);
    }
}
}
