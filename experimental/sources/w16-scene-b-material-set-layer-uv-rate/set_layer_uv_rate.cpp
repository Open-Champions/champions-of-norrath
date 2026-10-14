// Observed: SetLayerUVRate__10VIMaterialiRC7VIVect2 at 0x010cfa98 is 100 bytes.
// It copies two floats from the vector in a2 to material + 104 + 144 * layer, then,
// re-reading the source, sets bit 2 (mask 0x4) of the word at offset 12 if either
// float compares unequal to zero (a NaN counts as unequal). The mask is never
// cleared here. It stores 1 at offset 32 and returns 0.
// Inferred: offset 12 holds material flags; mask 0x4 means "some layer scrolls UVs".
// Unknown: the reader of the flag.
extern "C" {
int SetLayerUVRate__10VIMaterialiRC7VIVect2(unsigned char* material, int layer, const float* rate) {
    float* dst = reinterpret_cast<float*>(material + 104 + layer * 144);
    dst[0] = rate[0];
    dst[1] = rate[1];
    if (rate[0] != 0.0f || rate[1] != 0.0f)
        *reinterpret_cast<unsigned*>(material + 12) |= 0x4;
    *reinterpret_cast<int*>(material + 32) = 1;
    return 0;
}
}
