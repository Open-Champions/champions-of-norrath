// Observed startFractalLine__FP4Boltfi at 0x001842b8 (a local symbol), 504 bytes. B is a Bolt (the 1,840-byte record whose first
// 1,792 bytes the lightning starter hands over; the 28-byte entries below start at B+0), len the float argument and depth the int
// argument. linePos is a global int (the image symbol at 0x00486a00) and eRandState the generator word at 0x00483e58. The
// generator step is s = s * 0x19660d + 0x3c6ef35f in 32 bits; a "draw" below is (s >> 16) as an unsigned 16-bit value and
// u(draw) = float(draw) * 2^-16 doubled minus 1. In order:
//  1. Six steps of the generator run back to back from eRandState and the final state is stored; draws d1..d6 are their upper halves.
//     Let k = linePos; linePos becomes k + 1. E = B + 28 * k. Entries (all singles unless noted):
//       E+0 = u(d1) * len,   E+4 = u(d2) * len,   E+8 = u(d3) * len,
//       E+12 = (u(d4) * len - E+0 value) * 0.1,   E+16 = (u(d5) * len - E+4 value) * 0.1,   E+20 = (u(d6) * len - E+4 value) * 0.1,
//     where 0.1 is the single in the small-data area at 0x004800d8 (bits 0x3dcccccc). Then E+24 (a word) = rand() & 7 (the C library
//     rand is called after the six steps).
//  2. When depth is non-zero, startFractalLine__FP4Boltfi(B, len * 0.5, depth - 1) runs twice.
// Inferred: one level of a recursive jagged-line builder, depth 0 being a single entry and each deeper level doubling the count (the
// lightning starter passes the bolt's stored count). The subtraction of the second single, not the third, in E+20 is as read.
// Unknown: what the six singles and the word at +24 mean to the draw routine and whether linePos is reset elsewhere.
// Nothing here ran on the EE.
extern "C" {
extern int linePos;
extern int eRandState;
int rand();

static inline float unit_draw(unsigned state) {
    const float v = static_cast<float>(static_cast<int>(state >> 16)) * __builtin_bit_cast(float, 0x37800000u);
    return v + v - 1.0f;
}

void startFractalLine__FP4Boltfi(unsigned char* bolt, float length, int depth) {
    const unsigned s1 = static_cast<unsigned>(eRandState) * 0x19660du + 0x3c6ef35fu;
    const unsigned s2 = s1 * 0x19660du + 0x3c6ef35fu;
    const unsigned s3 = s2 * 0x19660du + 0x3c6ef35fu;
    const unsigned s4 = s3 * 0x19660du + 0x3c6ef35fu;
    const unsigned s5 = s4 * 0x19660du + 0x3c6ef35fu;
    const unsigned s6 = s5 * 0x19660du + 0x3c6ef35fu;
    const float tenth = __builtin_bit_cast(float, 0x3dccccccu);
    const int k = linePos;
    linePos = k + 1;
    float* const entry = reinterpret_cast<float*>(bolt + 28 * k);
    entry[0] = unit_draw(s1) * length;
    entry[1] = unit_draw(s2) * length;
    entry[2] = unit_draw(s3) * length;
    entry[3] = (unit_draw(s4) * length - entry[0]) * tenth;
    entry[4] = (unit_draw(s5) * length - entry[1]) * tenth;
    entry[5] = (unit_draw(s6) * length - entry[1]) * tenth;
    eRandState = static_cast<int>(s6);
    *reinterpret_cast<int*>(bolt + 28 * k + 24) = rand() & 7;
    if (depth != 0) {
        startFractalLine__FP4Boltfi(bolt, length * 0.5f, depth - 1);
        startFractalLine__FP4Boltfi(bolt, length * 0.5f, depth - 1);
    }
}
}
