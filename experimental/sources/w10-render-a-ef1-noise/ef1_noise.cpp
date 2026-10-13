// Observed ef1Noise__Fi at 0x0015a540, 128 bytes. The low four bits of the argument are a
// fraction t = (x & 15) / 16 (the literal 0.0625f). The argument shifted right
// arithmetically by four is the index i. Two signed halfwords are read from the table
// noiseTable (0x006021c8), at index (i & 255) as a and at ((i + 1) & 255) as b. The result
// is (a * (1 - t) + b * t) * 2^-15 (the literal 0x38000000), each product rounded to single
// precision in that order.
// Inferred: a one-dimensional interpolated lookup into the 256-entry table that
// initNoiseTable fills. Unknown: the name's "ef1" meaning and the callers' value range.
extern "C" {
extern short noiseTable[256];
float ef1Noise__Fi(int x) {
    const float t = static_cast<float>(x & 15) * 0.0625f;
    const int index = x >> 4;
    const float b = static_cast<float>(noiseTable[(index + 1) & 255]);
    const float a = static_cast<float>(noiseTable[index & 255]);
    return (a * (1.0f - t) + b * t) * 3.0517578125e-05f;
}
}
