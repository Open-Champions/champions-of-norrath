// Observed colorScale__FUif at 0x00191a20, 176 bytes, pinned SLUS_205.65.
// The top byte of the packed colour is kept unchanged. Each of the three low
// bytes is converted to float, multiplied by the float argument, converted
// back to an integer with the EE conversion, raised to 0 when negative and
// lowered to 255 when 256 or more. The three results are repacked in place.
// Inferred: the integer conversion truncates toward zero on the EE.
// Not executed on hardware; NaN and out-of-range inputs are not examined.
static unsigned scaled(unsigned byte, float scale) {
    int v = static_cast<int>(static_cast<float>(static_cast<int>(byte)) * scale);
    if (v < 0) v = 0;
    if (v >= 256) v = 255;
    return static_cast<unsigned>(v);
}

extern "C" unsigned colorScale__FUif(unsigned color, float scale) {
    unsigned c1 = scaled((color >> 8) & 0xff, scale);
    unsigned c2 = scaled((color >> 16) & 0xff, scale);
    unsigned c0 = scaled(color & 0xff, scale);
    return ((color >> 24) << 24) | c0 | (c2 << 16) | (c1 << 8);
}
