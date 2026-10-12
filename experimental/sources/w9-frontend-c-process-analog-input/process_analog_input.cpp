// Observed processAnalogInput__FiiPfT2 at 0x00168378, 148 bytes (pinned
// SLUS_205.65; a file-local symbol in the image, called from the pad data
// processing routine). Takes two raw stick bytes x and y (0..255, centered on
// 128) and writes two floats. Each axis is (value - 128) scaled by 2^-7 (bit
// pattern 0x3c000000) to roughly -1..1. With the length m = sqrt(x*x + y*y):
// if m < 0.041666664 (0x3d2aaaaa) both outputs are 0.0. Otherwise a factor
// f = (1.0 - 0.041666664 / m) * 10.68 (0x412ae147 as the multiplier) is applied
// and the outputs are x*f and y*f. The sqrt is the hardware sqrt.s instruction.
// Inferred: a radial dead zone with rescale, from that arithmetic.
// Unknown: the intended meaning of 10.68 (the result can exceed 1.0 near the
// rim, and no clamp is present), and whether the original used a named type.
static float bits_to_float(unsigned bits) {
    union {
        unsigned u;
        float f;
    } v;
    v.u = bits;
    return v.f;
}

extern "C" {
void processAnalogInput__FiiPfT2(int x, int y, float* out_x, float* out_y) {
    float scale = bits_to_float(0x3c000000u);
    float dead_zone = bits_to_float(0x3d2aaaaau);
    float gain = bits_to_float(0x412ae147u);
    float fx = static_cast<float>(x - 128) * scale;
    float fy = static_cast<float>(y - 128) * scale;
    float length = __builtin_sqrtf(fx * fx + fy * fy);
    if (length < dead_zone) {
        *out_x = 0.0f;
        *out_y = 0.0f;
        return;
    }
    float factor = (1.0f - dead_zone / length) * gain;
    *out_y = fy * factor;
    *out_x = fx * factor;
}
}
