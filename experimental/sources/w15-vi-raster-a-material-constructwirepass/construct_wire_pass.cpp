// Observed: ConstructWirePass__10VIMaterialiR8VIRasterP7VIQWord at 0x010cf5b0 is 364 bytes in the pinned SLUS-20565 image. Arguments:
// the material, a pass index, the raster (not used) and an output quadword pointer q; it returns 0. It writes 8 of the pass's 17
// quadwords (quadword numbers counted from q, each written in full; quadwords 4-9 and 14-16 are not touched):
//   0:  words {2, 0, 0, 0}
//   1:  64-bit {0x1000000000008002, 14}
//   2:  {0x50000, 71}
//   3:  {0x8000000064, 66}
//   10: {0x2006400000008003, 65}
//   11: {0x2006c00000008003, 65}
//   12: {0x2005400000008003, 65}
//   13: four floats, each a byte of the four colour bytes at material + 144*pass + 56 (bytes 0, 1, 2, 3 in order) converted to float and
//       multiplied by the float 0x3b808080 and then by 128.0f.
// Inferred: this is the packet for a wireframe pass: a GIF tag with a few A+D register writes (71 = TEST_1, 66 = ALPHA_1; 65 is the
// register number written after the later tags) and a colour. The tag words in quadwords 10-12 contain the GIF PRIM encodings; the pass
// has a fixed colour from the record and scales it to a 0..128 range as the surrounding packet expects.
// Unknown: what the later-tag registers are, why quadwords 4-9 are skipped (they may be filled by another builder), and the colour range.
// Nothing here ran on the EE.
extern "C" {
int ConstructWirePass__10VIMaterialiR8VIRasterP7VIQWord(unsigned char* material, int pass, unsigned char*, unsigned char* out) {
    unsigned* q = reinterpret_cast<unsigned*>(out);
    q[0] = 2;
    q[1] = 0;
    q[2] = 0;
    q[3] = 0;
    unsigned long long* d = reinterpret_cast<unsigned long long*>(out + 16);
    d[0] = 0x1000000000008002ull;
    d[1] = 14;
    d[2] = 0x50000ull;
    d[3] = 71;
    d[4] = 0x8000000064ull;
    d[5] = 66;
    unsigned long long* tags = reinterpret_cast<unsigned long long*>(out + 160);
    tags[0] = 0x2006400000008003ull;
    tags[1] = 65;
    tags[2] = 0x2006c00000008003ull;
    tags[3] = 65;
    tags[4] = 0x2005400000008003ull;
    tags[5] = 65;
    const unsigned char* color = material + pass * 144 + 56;
    const float scale = __builtin_bit_cast(float, 0x3b808080u);
    float* f = reinterpret_cast<float*>(out + 208);
    f[0] = static_cast<float>(color[0]) * scale * 128.0f;
    f[1] = static_cast<float>(color[1]) * scale * 128.0f;
    f[2] = static_cast<float>(color[2]) * scale * 128.0f;
    f[3] = static_cast<float>(color[3]) * scale * 128.0f;
    return 0;
}
}
