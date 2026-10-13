// Observed: GetLightning__12VIAtmospherePf at 0x0106a290 is 576 bytes. Arguments: atmosphere (a0)
// and a float pointer (a1). Fields: ints at +1516 and +1520, the float at +1192 (a time step), the
// floats at +4512 (time t), +4516 (duration d), +4520 (level L) and +3912 (the output copy).
// Constants used (truncated single floats): A = 0x3e4ccccc, B = 0x3e999999, C = 0x3f4ccccc, S =
// 0x3b808080 (about 1/255), R = 2^-31.
//   1. If the int at +1516 is not 1, or the int at +1520 is not 1, it stores 0.0f at t and returns 0.
//   2. If t == 0.0f it stores 0.0f at +3912 and returns 0.
//   3. now = t + step is stored at +4512. If now < A: value = now * 5.0f + ((float)r1 - (float)r2) *
//      R * A where r1, r2 are two successive rand() results. Otherwise: value = (now - A) / (d - A)
//      + ((float)r1 - (float)r2) * R * B with two rand() results; then a third rand() result with
//      (r & 15) < 2 replaces value by (float)((131 * r4) & 0xff) * S * A + C for a fourth result r4.
//   4. If d <= the stored time it stores 0.0f at +4512. With half = d * 0.5f: if t_old <= half and
//      half < the stored time (re-read) and a further rand() & 15 is < 6, it calls
//      RecomputeLightningPatches__12VIAtmosphere(atmosphere).
//   5. value is limited to [0.0f, 1.0f] (max with 0.0f, then min with 1.0f), multiplied by
//      (1.0f - the float at +4520), then by C. The result is stored at +3912 and through the
//      pointer, and the function returns 1.
// Inferred: a lightning flash envelope: ramps up for the first part of the flash (t < 0.2), then
// decays towards the duration, with random flicker; occasionally (probability 6/16) the bolt
// patches are recomputed as the flash passes its midpoint; the result is a brightness scaled by
// one minus the level at +4520 and a constant 0.8.
// Unknown: the names and units of the fields at +1192, +1516, +1520, +4512..+4520 and +3912; what
// the two int flags mean; the reason for the two-step random flicker.
extern "C" {
int rand(void);
void RecomputeLightningPatches__12VIAtmosphere(void* atmosphere);

int GetLightning__12VIAtmospherePf(unsigned char* atmosphere, float* out) {
    float* time = reinterpret_cast<float*>(atmosphere + 4512);
    if (*reinterpret_cast<int*>(atmosphere + 1516) != 1 || *reinterpret_cast<int*>(atmosphere + 1520) != 1) {
        *time = 0.0f;
        return 0;
    }
    float before = *time;
    if (before == 0.0f) {
        *reinterpret_cast<float*>(atmosphere + 3912) = 0.0f;
        return 0;
    }
    float now = before + *reinterpret_cast<float*>(atmosphere + 1192);
    *time = now;
    float value;
    if (now < 0x1.999998p-3f) {
        value = now * 5.0f;
        int first = rand();
        int second = rand();
        value = value + (static_cast<float>(first) - static_cast<float>(second)) * 0x1.0p-31f * 0x1.999998p-3f;
    } else {
        value = (now - 0x1.999998p-3f) / (*reinterpret_cast<float*>(atmosphere + 4516) - 0x1.999998p-3f);
        int first = rand();
        int second = rand();
        value = value + (static_cast<float>(first) - static_cast<float>(second)) * 0x1.0p-31f * 0x1.333332p-2f;
        if ((rand() & 0xf) < 2) {
            value = static_cast<float>((131 * rand()) & 0xff) * 0x1.010100p-8f * 0x1.999998p-3f + 0x1.999998p-1f;
        }
    }
    float duration = *reinterpret_cast<float*>(atmosphere + 4516);
    if (duration <= *time) {
        *time = 0.0f;
    }
    float half = duration * 0.5f;
    if (before <= half && half < *time && (rand() & 0xf) < 6) {
        RecomputeLightningPatches__12VIAtmosphere(atmosphere);
    }
    value = value > 0.0f ? value : 0.0f;
    value = value < 1.0f ? value : 1.0f;
    value = value * (1.0f - *reinterpret_cast<float*>(atmosphere + 4520));
    value = value * 0x1.999998p-1f;
    *reinterpret_cast<float*>(atmosphere + 3912) = value;
    *out = value;
    return 1;
}
}
