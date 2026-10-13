// Observed animPlaySound__FPvP6Point3P9animSoundi at 0x00160490, extent 460. Arguments: a sound
// (the vag header pointer soundPlaySound takes), a Point3 position (pointer), an animSound
// record and an int index. Returns nothing. Nothing happens when the sound is null.
// Record bytes used (all signed): +10 (b10), +11 (b11), +12 (b12), +13 (b13), +14 (b14).
// eRandState steps are eRandState = eRandState * 0x19660d + 0x3c6ef35f, u = new state >> 16 (the
// same generator drawWorldDynamicLight uses), taken in this order and only when the guard holds:
//  1. k = b12 * 2; when b13 > 0: k += u % (b13 * 4 + 1) - b13 * 2.
//  2. k + 64 and b10 + 64 are each clamped to 0..127; pitch = pitchTab[b10 + 64 clamped] and
//     volume = pitchTab[k + 64 clamped] (pitchTab is the float table at 0x006cb078).
//  3. When b11 > 0: pitch *= powf(0x1.0f38f8p+0f, (float)(u % (b11 * 16) - b11 * 8) * 0.125f).
//  4. delay = 0.0f, or when b14 != 0: (float)(u % (b14 + 1)) * 0x1.11111p-6f.
//  5. soundPlaySound(sound, position, pitch, volume * groupVolumeScale[index], delay).
// The float constants are written as hexadecimal float literals because the decimal forms round
// differently in the reviewed compiler. They are the literals at 0x0047ff58 (the twelfth root of two to float precision) and
// 0x0047ff5c (one sixtieth) and 0.125f inline. The divides are signed, so a negative b14 gives a
// remainder of non-negative sign; b14 == -1 is a division by zero that no check here exercises.
// Inferred: the pitch and volume are randomised around the record's centre values, with pitchTab
// holding a 128-entry pitch-multiplier table (the 64-entry offset puts 0 in the middle), and the
// last float is a start delay in seconds. Unknown: the field meanings of +10..+14 beyond what the
// arithmetic shows, and the contents of pitchTab. Nothing here was executed on the EE.
struct Point3 {
    float x, y, z;
};
extern "C" int eRandState;
static inline int nextRandom() {
    unsigned state = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
    eRandState = static_cast<int>(state);
    return static_cast<int>(state >> 16);
}
static inline int clampIndex(int v) {
    if (v < 0) v = 0;
    if (v >= 128) v = 127;
    return v;
}
extern "C" {
extern float pitchTab[];
extern float groupVolumeScale[];
float powf(float x, float y);
void soundPlaySound__FP10_vagHeaderP6Point3fff(void* sound, Point3* position, float pitch, float volume, float delay);
void animPlaySound__FPvP6Point3P9animSoundi(void* sound, Point3* position, const unsigned char* record, int index) {
    if (sound == nullptr) return;
    const int b10 = static_cast<signed char>(record[10]);
    const int b11 = static_cast<signed char>(record[11]);
    const int b12 = static_cast<signed char>(record[12]);
    const int b13 = static_cast<signed char>(record[13]);
    const int b14 = static_cast<signed char>(record[14]);
    int k = b12 * 2;
    if (b13 > 0) k += nextRandom() % (b13 * 4 + 1) - b13 * 2;
    const int pitchIndex = clampIndex(b10 + 64);
    const int volumeIndex = clampIndex(k + 64);
    float pitch = pitchTab[pitchIndex];
    const float volume = pitchTab[volumeIndex];
    if (b11 > 0) {
        const int step = nextRandom() % (b11 * 16) - b11 * 8;
        pitch *= powf(0x1.0f38f8p+0f, static_cast<float>(step) * 0.125f);
    }
    float delay = 0.0f;
    if (b14 != 0) delay = static_cast<float>(nextRandom() % (b14 + 1)) * 0x1.11111p-6f;
    soundPlaySound__FP10_vagHeaderP6Point3fff(sound, position, pitch, volume * groupVolumeScale[index], delay);
}
}
