// Observed: NewShakeCamera__18VISpellEventDriverP12VISpellEvent at 0x0117d5d0 is 300 bytes in the
// pinned SLUS-20565 image. Arguments: (this, record). Always returns 0.
//   1. e = the word at this+16 (the effect driver). If the int at e+96 differs from the int at
//      this+24, or the word at e+92 (a camera effect) is zero, it returns without doing anything.
//   2. Stores 1 at e+100. flags = the int at record+4; shake = flags & 0x10000; terp = flags & 0x20000.
//   3. When shake is set, by the byte at record+8: 0 calls Shake__14VICameraEffectQ214VICameraEffect5
//      SPEEDf(camera, 0, 0.0f); 1, 2 and 3 call it with (camera, that value, the float at record+20);
//      any other byte does nothing.
//   4. When terp is set it calls ShakeTerp__14VICameraEffectff(camera, the float at record+24, the
//      float at record+28). Returns 0.
// Inferred: record type 17 of the event table, a camera shake with up to four speed classes and an
// optional interpolation; e+96 appears to be the player/owner id the shake is meant for (it must match
// this+24), and e+100 is the flag the effect driver tests before resetting the camera effect.
// Unknown: what the speed classes mean, why class 0 passes 0.0f instead of the record's float, and the
// SPEED enumeration names. Nothing here ran on the EE.
extern "C" {
void Shake__14VICameraEffectQ214VICameraEffect5SPEEDf(void* camera, int speed, float amount);
void ShakeTerp__14VICameraEffectff(void* camera, float a, float b);

int NewShakeCamera__18VISpellEventDriverP12VISpellEvent(unsigned char* self, unsigned char* record) {
    unsigned char* effect = *reinterpret_cast<unsigned char**>(self + 16);
    if (*reinterpret_cast<int*>(effect + 96) != *reinterpret_cast<int*>(self + 24)) {
        return 0;
    }
    void* camera = *reinterpret_cast<void**>(effect + 92);
    if (camera == 0) {
        return 0;
    }
    *reinterpret_cast<int*>(effect + 100) = 1;
    int flags = *reinterpret_cast<int*>(record + 4);
    float terpB = *reinterpret_cast<float*>(record + 28);
    unsigned speed = record[8];
    bool terp = (flags & 0x20000) != 0;
    float amount = *reinterpret_cast<float*>(record + 20);
    float terpA = *reinterpret_cast<float*>(record + 24);
    if ((flags & 0x10000) != 0) {
        if (speed == 1) {
            Shake__14VICameraEffectQ214VICameraEffect5SPEEDf(camera, 1, amount);
        } else if (speed == 0) {
            Shake__14VICameraEffectQ214VICameraEffect5SPEEDf(camera, 0, 0.0f);
        } else if (speed == 2) {
            Shake__14VICameraEffectQ214VICameraEffect5SPEEDf(camera, 2, amount);
        } else if (speed == 3) {
            Shake__14VICameraEffectQ214VICameraEffect5SPEEDf(camera, 3, amount);
        }
    }
    if (terp) {
        ShakeTerp__14VICameraEffectff(camera, terpA, terpB);
    }
    return 0;
}
}
