// Observed: ScaleStick__5VIPadUc at 0x010ff300 is 92 bytes; the object is not read.
// The byte argument v maps to a float: v < 88 gives (88 - v) * -K, v >= 168 gives
// (v - 167) * K, and 88 <= v < 168 gives 0.0f, where K is the single-float
// 0x3c3a2e8b (one eighty-eighth). So 0 -> -1.0f, 255 -> +1.0f and the centre band
// 88..167 is a dead zone. Inferred: the argument is one analog stick axis byte.
// Unknown: the callers' axis sign convention.
extern "C" {
float ScaleStick__5VIPadUc(void* pad, unsigned char value) {
    (void)pad;
    unsigned v = value;
    float low = __builtin_bit_cast(float, 0xbc3a2e8bu);
    float high = __builtin_bit_cast(float, 0x3c3a2e8bu);
    if (v < 88) return static_cast<float>(static_cast<int>(88 - v)) * low;
    if (v >= 168) return static_cast<float>(static_cast<int>(v - 167)) * high;
    return 0.0f;
}
}
