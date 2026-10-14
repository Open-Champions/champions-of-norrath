// Observed: ConstructTextPass__10VIMaterialiR8VIRasterP7VIQWord at 0x010cedf8 is 1192 bytes in the pinned SLUS-20565 image. Arguments:
// the material (m), a pass index, the raster and an output quadword pointer; it returns 0. With r = m + 144*pass, flags = the word at
// m+12, f4 = the word at r+52, kind = the word at r+60 it writes all 17 quadwords (numbered from the output pointer):
//   0:  words {f4 & 6 ? 3 : 1, 0, 0, 0}
//   1:  64-bit {0x1000000000008008, 14}
//   2:  {(flags & 1 ? 0x50000 : 0x30000) | (kind != 0 ? 0x3001 : 0x80b), 71}
//   3:  {0x8000000000 | c, 66}, c = 0x48 for kind 1, 0x42 for 2, 0x44 for 3, 0x89 for 4 and 0x64 for every other kind
//   4:  {(f4 >> 1 & 1) | (f4 & 4 ? 4 : 0), 8}
//   5-8: the eight 64-bit words at r+128 .. r+191 copied unchanged (four register-write quadwords the caller or Set__13VIEESurfCache...
//        filled at pass-record offsets +80..+143)
//   9:  {z & 0x1ff | z & 0x0f000000 | (flags & 2 ? 0 : 1 << 32), 78}, z = GetZBufferReg__C8VIRaster(raster)
//   10: {(b | 28) << 47 | 0x3000400000008003, 1042}, b = (g << 5) | ((kind != 0) << 6), g = 1 if the raster word at +17548 is nonzero
//       and kind != 4, else 0
//   11: {(b | 29) << 47 | 0x3000400000008003, 1042}
//   12: {(b | 10) << 47 | 0x2000400000008003, 65}
//   13: four floats from the colour bytes at r+56 (bytes 0, 1, 2, 3), each converted to float and multiplied by the float 0x3b808080
//       and then 128.0f; if kind != 0 the fourth is also multiplied by 0.5f
//   14: floats {r[64], r[68], r[72]} copied (the fourth word is not written)
//   15: floats {r[76], r[80], r[84]} copied (fourth word not written)
//   16: floats {r[88] + r[112], r[92] + r[116], r[96]} (fourth word not written)
// Inferred: this is the packet for a textured pass. Register numbers: 71 = TEST_1, 66 = ALPHA_1, 8 = CLAMP_1, 78 = ZBUF_1; the pass flags
// word f4 holds the clamp mode bits. Quadwords 5-8 forward the TEX0/TEX1/MIPTBP1/MIPTBP2 writes that Set__13VIEESurfCache... stores in
// the pass at offsets +80..+143 (r+128 = record + 80, since the record starts at r+48). Quadwords 14-16 look like three texture-matrix rows
// (the last adds an offset).
// Unknown: the roles of the tag encodings in quadwords 10-12, the 1042 register word, the f4 bits other than 1, 2 and 4, and the meaning
// of g. Nothing here ran on the EE.
extern "C" {
unsigned long long GetZBufferReg__C8VIRaster(unsigned char* raster);

int ConstructTextPass__10VIMaterialiR8VIRasterP7VIQWord(unsigned char* material, int pass, unsigned char* raster, unsigned char* out) {
    unsigned char* record = material + pass * 144;
    unsigned* q = reinterpret_cast<unsigned*>(out);
    q[0] = (*reinterpret_cast<unsigned*>(record + 52) & 6) != 0 ? 3 : 1;
    q[1] = 0;
    q[2] = 0;
    q[3] = 0;
    unsigned long long* d = reinterpret_cast<unsigned long long*>(out + 16);
    d[0] = 0x1000000000008008ull;
    d[1] = 14;
    unsigned long long test;
    if (*reinterpret_cast<int*>(record + 60) != 0) {
        test = (*reinterpret_cast<unsigned*>(material + 12) & 1) ? 0x53001ull : 0x33001ull;
    } else {
        test = (*reinterpret_cast<unsigned*>(material + 12) & 1) ? 0x5080bull : 0x3080bull;
    }
    d[2] = test;
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
    int bits = *reinterpret_cast<int*>(record + 52);
    unsigned long long clamp = static_cast<unsigned long long>(bits >> 1) & 1;
    if ((bits & 4) != 0) clamp |= 4;
    d[6] = clamp;
    d[7] = 8;
    const unsigned long long* source = reinterpret_cast<const unsigned long long*>(record + 128);
    d[8] = source[0];
    d[9] = source[1];
    d[10] = source[2];
    d[11] = source[3];
    d[12] = source[4];
    d[13] = source[5];
    d[14] = source[6];
    d[15] = source[7];
    unsigned long long z = GetZBufferReg__C8VIRaster(raster);
    unsigned long long zbuf = (z & 0x1ffull) | (z & 0x0f000000ull);
    if ((*reinterpret_cast<unsigned*>(material + 12) & 2) == 0) zbuf |= 1ull << 32;
    d[16] = zbuf;
    d[17] = 78;
    int g = 0;
    if (*reinterpret_cast<int*>(raster + 17548) != 0) g = *reinterpret_cast<int*>(record + 60) != 4 ? 1 : 0;
    unsigned long long b = (static_cast<unsigned long long>(g) << 5) | (static_cast<unsigned long long>(*reinterpret_cast<unsigned*>(record + 60) != 0) << 6);
    unsigned long long* tags = reinterpret_cast<unsigned long long*>(out + 160);
    tags[0] = ((b | 28) << 47) | 0x3000400000008003ull;
    tags[1] = 1042;
    tags[2] = ((b | 29) << 47) | 0x3000400000008003ull;
    tags[3] = 1042;
    tags[4] = ((b | 10) << 47) | 0x2000400000008003ull;
    tags[5] = 65;
    const unsigned char* color = record + 56;
    const float scale = __builtin_bit_cast(float, 0x3b808080u);
    float rgba[4];
    rgba[3] = static_cast<float>(color[3]) * scale * 128.0f;
    rgba[0] = static_cast<float>(color[0]) * scale * 128.0f;
    rgba[1] = static_cast<float>(color[1]) * scale * 128.0f;
    rgba[2] = static_cast<float>(color[2]) * scale * 128.0f;
    if (*reinterpret_cast<int*>(record + 60) != 0) rgba[3] = rgba[3] * 0.5f;
    float* f = reinterpret_cast<float*>(out + 208);
    f[0] = rgba[0];
    f[1] = rgba[1];
    f[2] = rgba[2];
    f[3] = rgba[3];
    const float* r = reinterpret_cast<const float*>(record + 64);
    float* g1 = reinterpret_cast<float*>(out + 224);
    g1[0] = r[0];
    g1[1] = r[1];
    g1[2] = r[2];
    float* g2 = reinterpret_cast<float*>(out + 240);
    g2[0] = r[3];
    g2[1] = r[4];
    g2[2] = r[5];
    float* g3 = reinterpret_cast<float*>(out + 256);
    g3[0] = r[6] + r[12];
    g3[1] = r[7] + r[13];
    g3[2] = r[8];
    return 0;
}
}
