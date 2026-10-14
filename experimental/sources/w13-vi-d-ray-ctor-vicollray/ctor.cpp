// Observed: __9VICollRay at 0x0109bf88 is 100 bytes in the pinned SLUS-20565 image and returns nothing. With the object as
// this it stores float 0.0 into six groups of three floats, at byte offsets +0, +12, +24, +36, +56 and +76 (so +0..47, +56..67
// and +76..87). It calls nothing, reads nothing and leaves +48..55 and everything after +87 untouched.
// Inferred: the object holds six three-float vectors, the first four contiguous and the last two after an 8-byte gap.
// Unknown: the purpose of each vector and of the bytes at +48..55. Nothing here ran on the EE.
extern "C" {
void __9VICollRay(void* self) {
    float* base = static_cast<float*>(self);
    static const int starts[6] = {0, 3, 6, 9, 14, 19};
    for (int g = 0; g < 6; ++g)
        for (int i = 0; i < 3; ++i) base[starts[g] + i] = 0.0f;
}
}
