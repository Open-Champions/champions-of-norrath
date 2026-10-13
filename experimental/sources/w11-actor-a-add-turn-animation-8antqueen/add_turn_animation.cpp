// Observed addTurnAnimation__8AntQueen at 0x002082d8, 424 bytes. Constants (bit patterns of the image's
// gp-relative floats at 0x00481340/44/48): A = 0x3db851eb (0.09), B = 0x3f75c28f (0.96), C = 0x45081000
// (2177.0). Fields: halfword h at +284, floats p at +980 and q at +984, halfword out at +988, signed byte n
// at +292 (re-read on every pass), block R = word at +36, records of 32 bytes starting at +296 (word id) with
// a float or word value at +304 in each record.
//  1. t = (float)h * 2^-9, limited to [-1, 1].
//  2. d = p + (t - q) * A;  s = q + d;  p = d - d * B;  q = s;  w = s + s;  out = (short)(int)(w * C).
//  3. When 0 < w, for each record i < n: id equal to R+180 or R+68 stores w at +304+32*i; id equal to R+188 or
//     R+76 stores the word zero there. Otherwise (w <= 0): R+180 or R+68 stores the word zero; R+188 or R+76
//     stores -w. Records matching none are left alone.
// Inferred: the pairs look like left/right turn animation numbers. Unknown: what p, q and out are. Nothing
// here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" void addTurnAnimation__8AntQueen(void* self) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    float t = static_cast<float>(half(o, 284)) * bits(0x3b000000u);
    if (1.0f < t) t = 1.0f;
    if (t < -1.0f) t = -1.0f;
    const float q0 = real(o, 984);
    const float d = real(o, 980) + (t - q0) * bits(0x3db851ebu);
    const float s = q0 + d;
    const float w = s + s;
    real(o, 984) = s;
    real(o, 980) = d - d * bits(0x3f75c28fu);
    half(o, 988) = static_cast<short>(static_cast<int>(w * bits(0x45081000u)));
    unsigned char* const r = from_word(word(o, 36));
    if (0.0f < w) {
        for (int i = 0; i < static_cast<signed char>(o[292]); ++i) {
            const unsigned id = word(o, 296 + 32 * i);
            if (id == word(r, 180) || id == word(r, 68)) real(o, 304 + 32 * i) = w;
            else if (id == word(r, 188) || id == word(r, 76)) word(o, 304 + 32 * i) = 0;
        }
    } else {
        for (int i = 0; i < static_cast<signed char>(o[292]); ++i) {
            const unsigned id = word(o, 296 + 32 * i);
            if (id == word(r, 180) || id == word(r, 68)) word(o, 304 + 32 * i) = 0;
            else if (id == word(r, 188) || id == word(r, 76)) real(o, 304 + 32 * i) = -w;
        }
    }
}
