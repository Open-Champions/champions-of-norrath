// Observed: Begin__7VIFaderff at 0x0117cd68 is 60 bytes and returns nothing.
// Arguments are the object, float a (f12) and float b (f13). The floats at
// offsets 8, 12 and 16 are involved. If b equals zero: offset 16 gets 0.0, and
// offsets 8 and 12 both get a. Otherwise offset 12 gets a, and offset 16 gets
// (a - the float at offset 8) / b; offset 8 is unchanged.
// Inferred: offset 8 is the current fade level, offset 12 the target and offset
// 16 a per-unit-time rate, because Update__7VIFader steps offset 8 by
// rate * elapsed seconds and clamps it to offset 12. Unknown: whether b is
// seconds; Update scales a millisecond-resolution tick difference by 0.001.
extern "C" {
void Begin__7VIFaderff(void* object, float a, float b) {
    float* words = static_cast<float*>(object);
    if (b == 0.0f) {
        words[4] = 0.0f;
        words[2] = a;
        words[3] = a;
    } else {
        words[3] = a;
        words[4] = (a - words[2]) / b;
    }
}
}
