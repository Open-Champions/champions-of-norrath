// Observed waterStep at 0x00187798 (a hand-written routine with no size in the symbol table; its code runs to 0x00187a58, 704 bytes).
// It uses the EE multimedia instructions on 128-bit registers, keeps the callee-saved registers in the scratchpad (0x70003f00) and
// restores them on exit. Arguments: A (a0), B (a1) and M (a2), all 16-byte aligned. A is an array of rows of 40 signed halfwords
// (an 80-byte pitch) of which the first 32 lanes of each row are used; B is an array of rows of 32 signed halfwords (a 64-byte
// pitch); M is 8 quadwords, 64 unsigned halfwords. All adds and subtracts below saturate to the signed 16-bit range and ">>" is an
// arithmetic shift of a halfword. The loop runs for k = 0 .. 31 and keeps three row-sized states between iterations (all zero at the
// start): the raw row k-1 of A (call it c), the horizontal sums h1 of row k-1 and h2 of row k-2. M is read as two sets of 32 lanes: lanes
// 0..31 for k < 16 and lanes 32..63 for k >= 16. In iteration k:
//  1. Row k of A is read (32 lanes, x). Its horizontal sum is h[i] = (x[i] + x[i+1]) + x[i-1], where x[-1] and x[32] are zero.
//  2. For each lane i: s = ((h[i] + h1[i]) + h2[i]) - c[i]; s = s >> 3; s = s - c[i]; s = s >> 2. v = B[k-1][i] + s.
//     v is kept only when the sign bit of the lane's mask word is set (otherwise it becomes 0), and the mask word is shifted left by
//     one (16 bits). Then v = v - (v >> 7); A[k-1][i] = c[i] + v and B[k-1][i] = v. (For k = 0 these are the rows just before A and B.)
//  3. h2 = h1; h1 = h; c = x.
// Nothing else is read or written (the scratchpad words used for saving registers are not part of the result).
// Inferred: a damped wave step on a water surface of 32 columns by 32 rows, where the mask bits select which cells may move (one bit per
// row, most significant first); the caller passes the patch's heights, velocities and a 128-byte bit table. Unknown: the units of the
// values and the 3/2/7 shifts' physical meaning. Nothing here ran on the EE.
namespace {
inline short clamp16(int v) { return v < -32768 ? static_cast<short>(-32768) : v > 32767 ? static_cast<short>(32767) : static_cast<short>(v); }
inline short add16(short a, short b) { return clamp16(static_cast<int>(a) + static_cast<int>(b)); }
inline short sub16(short a, short b) { return clamp16(static_cast<int>(a) - static_cast<int>(b)); }
}  // namespace

extern "C" void waterStep(void* heights, void* velocities, const void* masks) {
    short* const a = static_cast<short*>(heights);
    short* const b = static_cast<short*>(velocities);
    const unsigned short* const table = static_cast<const unsigned short*>(masks);
    short c[32] = {};
    short h1[32] = {};
    short h2[32] = {};
    unsigned short mask[32];
    for (int k = 0; k < 32; ++k) {
        if (k == 0 || k == 16) {
            for (int i = 0; i < 32; ++i) mask[i] = table[(k == 0 ? 0 : 32) + i];
        }
        const short* const x = a + 40 * k;
        short* const out = a + 40 * (k - 1);
        short* const velocity = b + 32 * (k - 1);
        short h[32];
        for (int i = 0; i < 32; ++i) {
            const short right = i < 31 ? x[i + 1] : static_cast<short>(0);
            const short left = i > 0 ? x[i - 1] : static_cast<short>(0);
            h[i] = add16(add16(x[i], right), left);
        }
        for (int i = 0; i < 32; ++i) {
            short s = add16(add16(h[i], h1[i]), h2[i]);
            s = sub16(s, c[i]);
            s = static_cast<short>(s >> 3);
            s = sub16(s, c[i]);
            s = static_cast<short>(s >> 2);
            short v = add16(velocity[i], s);
            if (!(mask[i] & 0x8000u)) v = 0;
            mask[i] = static_cast<unsigned short>(mask[i] << 1);
            v = sub16(v, static_cast<short>(v >> 7));
            out[i] = add16(c[i], v);
            velocity[i] = v;
        }
        for (int i = 0; i < 32; ++i) {
            h2[i] = h1[i];
            h1[i] = h[i];
            c[i] = x[i];
        }
    }
}
