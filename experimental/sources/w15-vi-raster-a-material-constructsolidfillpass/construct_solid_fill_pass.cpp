// Observed: ConstructSolidFillPass__10VIMaterialiR8VIRasterP7VIQWord at 0x010cf2a0 is 784 bytes in the pinned SLUS-20565 image.
// Arguments: the material (m), a pass index, the raster and an output quadword pointer; it returns 0. With r = m + 144*pass, flags =
// the word at m+12 and kind = the word at r+60, it writes these quadwords (numbered from the output pointer; quadwords 5-9 and 14-16
// are not touched):
//   0:  words {0, 0, 0, 0}
//   1:  64-bit {0x1000000000008003, 14}
//   2:  {flags & 1 ? 0x50000 : 0x30000, 71}
//   3:  {0x8000000000 | c, 66}, c = 0x48 for kind 1, 0x42 for 2, 0x44 for 3, 0x89 for 4 and 0x64 for every other kind
//   4:  {z & 0x1ff | z & 0x0f000000 | (flags & 2 ? 0 : 1 << 32), 78}, z = GetZBufferReg__C8VIRaster(raster)
//   10: {kind != 0 ? 0x2026400000008003 : 0x2006400000008003, 65}
//   11: {kind != 0 ? 0x2026c00000008003 : 0x2006c00000008003, 65}
//   12: {kind != 0 ? 0x2025400000008003 : 0x2005400000008003, 65}
//   13: four floats: the colour bytes at r+56 (bytes 0, 1, 2, 3) each converted to float and multiplied by the float 0x3b808080
//       and then by 255.0f; if kind != 0 the fourth (alpha) is also multiplied by 0.5f.
// Inferred: the packet for a flat-colour pass; the register numbers are 71 = TEST_1, 66 = ALPHA_1, 78 = ZBUF_1 and the kind word picks
// the alpha-blend setting (the codes differ only in the low byte) and, when nonzero, a textured-primitive variant of the three tags.
// Unknown: the meaning of kinds above 4 (they use the default), what the later tags select, and why quadwords 5-9 stay untouched.
// Nothing here ran on the EE.
extern "C" {
unsigned long long GetZBufferReg__C8VIRaster(unsigned char* raster);

int ConstructSolidFillPass__10VIMaterialiR8VIRasterP7VIQWord(unsigned char* material, int pass, unsigned char* raster, unsigned char* out) {
    unsigned char* record = material + pass * 144;
    unsigned* q = reinterpret_cast<unsigned*>(out);
    q[0] = 0;
    q[1] = 0;
    q[2] = 0;
    q[3] = 0;
    unsigned long long* d = reinterpret_cast<unsigned long long*>(out + 16);
    d[0] = 0x1000000000008003ull;
    d[1] = 14;
    d[2] = (*reinterpret_cast<unsigned*>(material + 12) & 1) ? 0x50000ull : 0x30000ull;
    d[3] = 71;
    unsigned kind = *reinterpret_cast<unsigned*>(record + 60);
    unsigned long long alpha;
    switch (kind) {
        case 1: alpha = 0x8000000048ull; break;
        case 2: alpha = 0x8000000042ull; break;
        case 3: alpha = 0x8000000044ull; break;
        case 4: alpha = 0x8000000089ull; break;
        default: alpha = 0x8000000064ull; break;
    }
    d[4] = alpha;
    d[5] = 66;
    unsigned long long z = GetZBufferReg__C8VIRaster(raster);
    unsigned long long zbuf = (z & 0x1ffull) | (z & 0x0f000000ull);
    if ((*reinterpret_cast<unsigned*>(material + 12) & 2) == 0) zbuf |= 1ull << 32;
    d[6] = zbuf;
    d[7] = 78;
    unsigned long long* tags = reinterpret_cast<unsigned long long*>(out + 160);
    tags[0] = *reinterpret_cast<int*>(record + 60) != 0 ? 0x2026400000008003ull : 0x2006400000008003ull;
    tags[1] = 65;
    tags[2] = *reinterpret_cast<int*>(record + 60) != 0 ? 0x2026c00000008003ull : 0x2006c00000008003ull;
    tags[3] = 65;
    tags[4] = *reinterpret_cast<int*>(record + 60) != 0 ? 0x2025400000008003ull : 0x2005400000008003ull;
    tags[5] = 65;
    const unsigned char* color = record + 56;
    const float scale = __builtin_bit_cast(float, 0x3b808080u);
    float rgba[4];
    rgba[3] = static_cast<float>(color[3]) * scale * 255.0f;
    rgba[0] = static_cast<float>(color[0]) * scale * 255.0f;
    rgba[1] = static_cast<float>(color[1]) * scale * 255.0f;
    rgba[2] = static_cast<float>(color[2]) * scale * 255.0f;
    if (*reinterpret_cast<int*>(record + 60) != 0) rgba[3] = rgba[3] * 0.5f;
    float* f = reinterpret_cast<float*>(out + 208);
    f[0] = rgba[0];
    f[3] = rgba[3];
    f[1] = rgba[1];
    f[2] = rgba[2];
    return 0;
}
}
