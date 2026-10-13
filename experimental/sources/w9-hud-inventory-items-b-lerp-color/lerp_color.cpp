// Observed lerpColor__FUiUif at 0x00193e20, 704 bytes.
// Arguments: two packed 32-bit colors (from, to) and a float fraction t. Each
// of the four bytes is converted to float independently as an unsigned value,
// interpolated as from + (to - from) * t, converted back to an unsigned integer
// (truncating), and the four results are shifted to their original byte
// positions (0, 8, 16, 24) and OR-ed together. There is no clamping and no
// masking: a fraction outside 0..1 can push a channel past 255 and spill into
// the next byte, as in the original. Which byte is red, green, blue or alpha is
// not established here; the function treats all four alike.
static inline unsigned lerpChannel(unsigned a, unsigned b, float t) {
    float fa = static_cast<float>(a);
    float fb = static_cast<float>(b);
    return static_cast<unsigned>(fa + (fb - fa) * t);
}

extern "C" unsigned lerpColor__FUiUif(unsigned from, unsigned to, float t) {
    unsigned c0 = lerpChannel(from & 0xff, to & 0xff, t);
    unsigned c1 = lerpChannel((from >> 8) & 0xff, (to >> 8) & 0xff, t);
    unsigned c2 = lerpChannel((from >> 16) & 0xff, (to >> 16) & 0xff, t);
    unsigned c3 = lerpChannel(from >> 24, to >> 24, t);
    return c0 | (c1 << 8) | (c2 << 16) | (c3 << 24);
}
