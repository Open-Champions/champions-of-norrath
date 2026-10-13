// Observed: ScaleStick__5VIPadUc at 0x010ff300 is 92 bytes and returns a float.
// The unsigned-byte stick reading (second argument; the object is not touched)
// has a dead band: 88 to 167 gives 0.0. Below 88 the result is (88 - value) * -k;
// from 168 up it is (value - 167) * k, where k is the single-float constant
// 0x3c3a2e8b (just under 1/88), so 0 gives -88k and 255 gives +88k, both a hair
// under 1.0 in magnitude.
// Inferred: the byte is a raw analog stick axis from the pad read buffer, and the
// result is a signed unit-range axis value.
// Unknown: why the dead band is asymmetric about 128 (88 below, 80 above), and
// whether other code relies on the exact rounding at the extremes.
extern "C" {
float ScaleStick__5VIPadUc(void* pad, unsigned char value) {
    (void)pad;
    const float step = 0.011363636f;
    if (value < 88) return static_cast<float>(88 - value) * -step;
    if (value >= 168) return static_cast<float>(value - 167) * step;
    return 0.0f;
}
}
