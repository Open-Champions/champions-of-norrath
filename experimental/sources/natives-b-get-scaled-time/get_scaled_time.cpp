// Observed amxGetScaledTime at 0x0013b6f8, 44 bytes. The first AMX
// cell is converted from an integer to a float, multiplied by 60 and
// by the single-precision word 0x3c888888, then rounded to an integer
// with ties to even. The product for 1 is just under 1, so truncation
// would return 0.
extern "C" int amxGetScaledTime__FP5__amxPi(void*, int* params) {
    union Bits {
        unsigned word;
        float value;
    };
    Bits scale;
    scale.word = 0x3c888888u;
    const float cell = static_cast<float>(params[1]);
    float magnitude_source = (cell * 60.f) * scale.value;
    const int negative = magnitude_source < 0.f;
    float magnitude = negative ? -magnitude_source : magnitude_source;
    const int whole = static_cast<int>(magnitude);
    const float fraction = magnitude - static_cast<float>(whole);
    int rounded = whole;
    if (fraction > 0.5f || (fraction == 0.5f && (whole & 1) != 0))
        rounded = whole + 1;
    return negative ? -rounded : rounded;
}
