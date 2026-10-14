// Observed: TickPulse__C10VIUIWindow at 0x0103f6d8, 168 bytes. With the float p at offset 40 of the window
// and the 64-bit time stored at offset 352: a = 1 - (1 - p) * (1 - p); t = (float)(GetTime__7VITimer() - time)
// * 0.01f (bits 0x3c23d70a); when 6.2831855f (bits 0x40c90fda) is less than t, that amount is subtracted once;
// the result is a * (sinf(t + 1.5707964f (bits 0x3fc90fda)) * 0.5 + 0.5). The subtraction of the two 64-bit
// times uses the library signed 64-bit to float conversion (__floatdisf).
// Inferred: a blink brightness in 0..1 that eases in with the open progress p and cycles about every 628
// timer units; the subtract is a single wrap, not a modulo, so a longer elapsed time gives a larger sine
// argument rather than a reduced one.
// Unknown: the timer unit, and what the ids of callers do with the value beyond using it as alpha.
extern "C" {
long long GetTime__7VITimer();
float sinf(float x);

float TickPulse__C10VIUIWindow(const void* self) {
    const char* base = static_cast<const char*>(self);
    const float progress = *reinterpret_cast<const float*>(base + 40);
    const float ease = 1.0f - progress;
    const float shaped = 1.0f - ease * ease;
    const long long start = *reinterpret_cast<const long long*>(base + 352);
    float t = static_cast<float>(GetTime__7VITimer() - start) * __builtin_bit_cast(float, 0x3c23d70au);
    const float period = __builtin_bit_cast(float, 0x40c90fdau);
    if (period < t) t = t - period;
    return shaped * (sinf(t + __builtin_bit_cast(float, 0x3fc90fdau)) * 0.5f + 0.5f);
}
}
