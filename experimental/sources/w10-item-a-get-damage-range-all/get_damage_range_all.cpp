// Observed getDamageRangeAll__4itemPQ24item11descriptionPiT2i at 0x00103ac8, 1116
// bytes (item description D, int* low, int* high, int extra). D's first word is a
// pointer B to a base record whose signed halfwords at +14 and +16 give the
// starting low and high values. D has four 8-byte modifier records starting at
// D+36: halfword +0 is a modifier id, +2 a first value, +4 a second value.
// "Lookup(id, off)" is the halfword at record+off of the first record whose id is
// matched, or 0 when none matches (so a present modifier with value 0 behaves as
// absent).
//   1. lo = (float)B[14], hi = (float)B[16].
//   2. id 35: when Lookup(35, +2) is nonzero, lo and hi are both multiplied by
//      (float)Lookup(35, +2) * 0.01f + 1.0f.
//   3. Only when `extra` is nonzero, for each of the ids 21, 22 and 24 in that order:
//      when Lookup(id, +2) is nonzero, lo += (float)Lookup(id, +2) and
//      hi += (float)Lookup(id, +4).
//   4. m = getDamageMultiplier__4itemPQ24item11description(D); *low =
//      (int)(lo * m + 0.5f) (truncated); the multiplier is fetched again for hi, and
//      *high = (int)(hi * m + 0.5f). Both results are truncated toward zero.
// The 0.01f constant is read from the image's constant pool. Unknown: what the
// modifier ids 21, 22, 24 and 35 mean and what the base record's halfwords at +14
// and +16 are.
extern "C" {
float getDamageMultiplier__4itemPQ24item11description(void*);

static inline int lookup(unsigned char* d, int id, int off) {
    for (int i = 0; i < 4; i++) {
        if (*reinterpret_cast<short*>(d + 36 + 8 * i) == id) return *reinterpret_cast<short*>(d + off + 8 * i);
    }
    return 0;
}

void getDamageRangeAll__4itemPQ24item11descriptionPiT2i(void* description, int* low, int* high, int extra) {
    unsigned char* d = static_cast<unsigned char*>(description);
    unsigned char* base = *reinterpret_cast<unsigned char**>(d);
    float lo = static_cast<float>(*reinterpret_cast<short*>(base + 14));
    float hi = static_cast<float>(*reinterpret_cast<short*>(base + 16));
    if (lookup(d, 35, 38) != 0) {
        float scale = static_cast<float>(lookup(d, 35, 38)) * 0.01f + 1.0f;
        hi *= scale;
        lo *= scale;
    }
    if (extra != 0) {
        if (lookup(d, 21, 38) != 0) {
            lo += static_cast<float>(lookup(d, 21, 38));
            hi += static_cast<float>(lookup(d, 21, 40));
        }
        if (lookup(d, 22, 38) != 0) {
            lo += static_cast<float>(lookup(d, 22, 38));
            hi += static_cast<float>(lookup(d, 22, 40));
        }
        if (lookup(d, 24, 38) != 0) {
            lo += static_cast<float>(lookup(d, 24, 38));
            hi += static_cast<float>(lookup(d, 24, 40));
        }
    }
    lo = static_cast<float>(static_cast<int>(lo * getDamageMultiplier__4itemPQ24item11description(d) + 0.5f));
    *low = static_cast<int>(lo);
    hi = static_cast<float>(static_cast<int>(hi * getDamageMultiplier__4itemPQ24item11description(d) + 0.5f));
    *high = static_cast<int>(hi);
}
}
