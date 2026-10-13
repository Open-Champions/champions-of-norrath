// Observed: Decrement__12VIWndOptions at 0x010c4b58 is 724 bytes. With this, g the result of VIGetGameOptions__Fv(),
// mUI the word _5VIWnd$mUI and k the unsigned word at +1152 of this (a jump table of 12 entries is used for
// k < 12; other values only run the final clamp), after calling VIGetFeatureFlags__Fv() once:
//  0: g[6] = (g[6] != 1); SoundPlay__4VIUI11VIUISoundId(mUI, 11).
//  1: g[7] = (g[7] == 0); sound 11.
//  2: g[4] = (g[4] + m - 1) % m with the byte g[4] taken unsigned and m = 10 when feature flags & 0x100 is nonzero,
//     else 3 (signed remainder); sound 11.
//  3: when the float at g+8 is above 0: g+8 = max(0, g+8 - 0.1) (a value just below 0.1, bits 0x3dcccccc; a NaN result
//     becomes 0) and sound 11, otherwise sound 9; then
//     SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(
//     the word at (mUI) + 76, 0, the float g+8).
//  4: the same with the float at g+12, sound class 1.
//  5: g[5] ^= 1; SetOutputMode__13VISoundDevicei(the word at mUI + 76, g[5] != 0); sound 11.
//  6: g[145] ^= 1; sound 11.   7: g[146] = (g[146] == 0); sound 11.
//  8, 9, 10, 11: the byte at this+1174, +1175, +1176, +1177 respectively ^= 1; sound 11.
// Finally, for every k: when the float at g+8 is below the value with bits 0x3d4ccccc it is set to 0; then the same for
// the float at g+12.
// Inferred: this is the left/decrease key of the options window; rows 0..11 are the settings rows of the three option
// screens and rows 3 and 4 are volume sliders (volumes on the sound device are set live); the sound ids 9 and 11 are
// "rejected" and "changed" cues from their use only.
// Unknown: the row names, the meaning of m (10 or 3) and the feature flag bit, and the result type (void here).
extern "C" {
unsigned char* VIGetGameOptions__Fv();
unsigned VIGetFeatureFlags__Fv();
extern unsigned _5VIWnd$mUI;
void SoundPlay__4VIUI11VIUISoundId(unsigned ui, int sound);
void SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(void* device, int sound_class, float volume);
void SetOutputMode__13VISoundDevicei(void* device, int mode);

static inline void* device_of(unsigned ui) {
    return reinterpret_cast<void*>(static_cast<unsigned long>(*reinterpret_cast<unsigned*>(static_cast<unsigned long>(ui) + 76)));
}

void Decrement__12VIWndOptions(void* self) {
    unsigned char* window = static_cast<unsigned char*>(self);
    unsigned char* g = VIGetGameOptions__Fv();
    const unsigned flags = VIGetFeatureFlags__Fv();
    const int modulus = (flags & 0x100) ? 10 : 3;
    const float step = __builtin_bit_cast(float, 0x3dcccccc);
    float* volumes = reinterpret_cast<float*>(g + 8);
    switch (*reinterpret_cast<unsigned*>(window + 1152)) {
    case 0:
        g[6] = (g[6] ^ 1) != 0;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        break;
    case 1:
        g[7] = g[7] < 1;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        break;
    case 2:
        g[4] = (static_cast<int>(g[4]) + modulus - 1) % modulus;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        break;
    case 3:
    case 4: {
        const int slot = *reinterpret_cast<unsigned*>(window + 1152) - 3;
        if (0.0f < volumes[slot]) {
            float lowered = volumes[slot] - step;
            if (!(0.0f <= lowered)) lowered = 0.0f;
            volumes[slot] = lowered;
            SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        } else {
            SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 9);
        }
        SetSoundClassVolume__13VISoundDevice12VISOUNDCLASSf(device_of(_5VIWnd$mUI), slot, volumes[slot]);
        break;
    }
    case 5: {
        g[5] ^= 1;
        SetOutputMode__13VISoundDevicei(device_of(_5VIWnd$mUI), g[5] != 0);
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        break;
    }
    case 6:
        g[145] ^= 1;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        break;
    case 7:
        g[146] = g[146] < 1;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        window[1174 + (*reinterpret_cast<unsigned*>(window + 1152) - 8)] ^= 1;
        SoundPlay__4VIUI11VIUISoundId(_5VIWnd$mUI, 11);
        break;
    default:
        break;
    }
    const float floor_value = __builtin_bit_cast(float, 0x3d4ccccc);
    if (volumes[0] < floor_value) volumes[0] = 0.0f;
    if (volumes[1] < floor_value) volumes[1] = 0.0f;
}
}
