// Observed processAnalogInput__FiiPfT2 at 0x00168378, 148 bytes (pinned
// SLUS_205.65; a file-local symbol in the image, called from the pad data
// processing routine). Takes two raw stick bytes x and y (0..255, centered on
// 128) and writes two floats. Each axis is (value - 128) scaled by 2^-7 (bit
// pattern 0x3c000000) to roughly -1..1. With the length m = sqrt(x*x + y*y):
// if m < 0.32 (0x3ea3d70a) both outputs are 0.0. Otherwise a factor
// f = (1.0 - 0.32 / m) * 1.4705882 (0x3fbc3c3c, about 1 / 0.68) is applied and
// the outputs are x*f and y*f. The sqrt is the hardware sqrt.s instruction.
// Inferred: a radial dead zone with rescale, from that arithmetic; the gain
// is the reciprocal of one minus the dead zone only numerically.
// Unknown: the intended meaning of the constants (the result can reach
// slightly above 1.0 near the rim; no clamp is present), and whether the
// original used a named type.
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
    float dead_zone = bits_to_float(0x3ea3d70au);
    float gain = bits_to_float(0x3fbc3c3cu);
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
