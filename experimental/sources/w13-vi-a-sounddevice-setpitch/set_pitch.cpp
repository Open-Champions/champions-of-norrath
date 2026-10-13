// Observed: SetPitch__13VISoundDeviceii at 0x011253a8 is 360 bytes. Arguments: device (a0),
// playback id (a1) and pitch (a2). pb = Playback__13VISoundDevicei(device, id); a null pb ends the
// call. Otherwise it stores the pitch at pb+36, and selects a sound: the word at device + 2584 +
// 4 * (the int at pb+28), or none when that int is -1; no sound ends the call.
//   - If Type__C7VISound(sound) == 0: wave = AsWave__7VISound(sound), rate =
//     SampleRate__C6VIWave(wave); it reads the pitch back from pb+36 and computes
//     value = ((rate << 12) / 48000) * pitch, then value / 4096 (rounded toward zero). With
//     mask = the 64-bit word at pb+16 it stores into sIopFunctionData: [0] =
//     FirstChannel__13VISoundDeviceUl(device, mask), [1] = NextChannel__13VISoundDeviceUli(
//     device, mask, [0]), [2] = value / 4096, then calls SetIopFunction__FUii(5, 3).
//   - Then (in every case that reaches here) it calls Type__C7VISound(sound) a second time; when
//     that result is 1 it calls AsXm__7VISound(sound) (the result is unused), stores [0] = the
//     low 24 bits of mask, [1] = bits 24..55 of mask (arithmetic shift of mask << 8) and [2] =
//     the pitch argument, and calls SetIopFunction__FUii(11, 3).
//   Nothing is returned.
// Inferred: pitch is a fixed-point multiplier with 12 fraction bits; for a wave sound the
// requested playback rate is rate * pitch / 48000 (relative to a 48 kHz base), forwarded with the
// channel range to IOP function 5; for a type-1 sound (an XM module) the channel mask and pitch
// go to IOP function 11. The second Type call is as observed (the same double call is seen in
// Stop__13VISoundDevicei).
// Unknown: the names of IOP functions 5 and 11, the sound type numbers, and why Type is queried
// twice.
extern "C" {
void* Playback__13VISoundDevicei(void* device, int id);
int Type__C7VISound(const void* sound);
void* AsWave__7VISound(void* sound);
int SampleRate__C6VIWave(const void* wave);
void* AsXm__7VISound(void* sound);
int FirstChannel__13VISoundDeviceUl(void* device, unsigned long long mask);
int NextChannel__13VISoundDeviceUli(void* device, unsigned long long mask, int channel);
void SetIopFunction__FUii(unsigned function, int count);
extern int norrath_overlay_sIopFunctionData_11edf08[];
}

// The sound word in the device table is a 32-bit target address; a host check redefines this.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
void SetPitch__13VISoundDeviceii(unsigned char* device, int id, int pitch) {
    unsigned char* pb = static_cast<unsigned char*>(Playback__13VISoundDevicei(device, id));
    if (pb == 0) {
        return;
    }
    void* sound = 0;
    int slot = *reinterpret_cast<int*>(pb + 28);
    *reinterpret_cast<int*>(pb + 36) = pitch;
    if (slot != -1) {
        sound = NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(device + 2584 + 4 * slot));
    }
    if (sound == 0) {
        return;
    }
    int* data = norrath_overlay_sIopFunctionData_11edf08;
    if (Type__C7VISound(sound) == 0) {
        int rate = SampleRate__C6VIWave(AsWave__7VISound(sound));
        int value = (rate << 12) / 48000 * *reinterpret_cast<int*>(pb + 36);
        unsigned long long mask = *reinterpret_cast<unsigned long long*>(pb + 16);
        data[0] = FirstChannel__13VISoundDeviceUl(device, mask);
        int next = NextChannel__13VISoundDeviceUli(device, mask, data[0]);
        data[2] = value / 4096;
        data[1] = next;
        SetIopFunction__FUii(5, 3);
    }
    if (Type__C7VISound(sound) == 1) {
        AsXm__7VISound(sound);
        long long mask = *reinterpret_cast<long long*>(pb + 16);
        data[1] = static_cast<int>(mask << 8 >> 32);
        data[2] = pitch;
        data[0] = static_cast<int>(mask & 0xffffff);
        SetIopFunction__FUii(11, 3);
    }
}
}
