// Observed soundSetStreamVolume__Fif at 0x0024f5b8, 80 bytes.
// The float is clamped to the closed range 0 through 1. streamVolume at
// 0x0042de48 holds three words. An equal word skips soundUpdateAdjustments.
// A different word is stored and that call follows. max.s and min.s produce
// the clamp. The equal branch is bc1tl, so the store is skipped.
extern "C" {
extern float streamVolume[3];
void soundUpdateAdjustments__Fv();

static float bits(unsigned value) {
    float out;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&out);
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
    bytes[2] = static_cast<unsigned char>(value >> 16);
    bytes[3] = static_cast<unsigned char>(value >> 24);
    return out;
}

void soundSetStreamVolume__Fif(int index, float volume) {
    float clamped = volume;
    const float zero = bits(0);
    const float one = bits(0x3f800000u);
    if (clamped < zero) clamped = zero;
    if (clamped > one) clamped = one;
    if (streamVolume[index] == clamped) return;
    streamVolume[index] = clamped;
    soundUpdateAdjustments__Fv();
}
}
