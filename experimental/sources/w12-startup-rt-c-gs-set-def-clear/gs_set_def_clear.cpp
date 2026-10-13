/* Observed sceGsSetDefClear at 0x002d2780, 252 bytes. Arguments: a 96-byte record, then
 * ztest, x, y, width and height (each sign-extended to 16 bits), red, green and blue
 * (low bytes only), alpha (low byte, read from the first stack slot after blue) and q (a
 * 32-bit word from the last stack slot). The record is six pairs of doublewords, value
 * first and register number second:
 *   +0/+8    0x30000, register 71
 *   +16/+24  6, register 0
 *   +32/+40  red | green << 8 | blue << 16 | alpha << 24 | 0x3f80000000000000, register 1
 *   +48/+56  (x << 4) | (y << 4) << 16 | q << 32, register 5
 *   +64/+72  ((x + width) << 4) | ((y + height) << 4) << 16 | q << 32, register 5
 *   +80/+88  0x30000 when ztest is zero, else 0x10000 | (ztest & 3) << 17, register 71
 * The coordinate terms are 32-bit results sign-extended to 64 bits before combining, so
 * negative coordinates set the upper bits. A memory barrier (sync) follows and the result
 * is 6.
 * Inferred: the packet draws one sprite (primitive 6) with the given colour; 0x3f800000
 * is 1.0f in the upper half of the colour doubleword; the registers are graphics
 * synthesizer addresses (primitive, colour, vertex, test) in address/data packing.
 * Unknown: the meaning of q beyond the word the code ORs into both vertices.
 */
extern "C" {
int sceGsSetDefClear(unsigned long long* env, int ztest_in, int x_in, int y_in, int width_in, int height_in,
                     int red, int green, int blue, int alpha, unsigned q) {
    const int ztest = static_cast<short>(ztest_in);
    const int x = static_cast<short>(x_in);
    const int y = static_cast<short>(y_in);
    const int width = static_cast<short>(width_in);
    const int height = static_cast<short>(height_in);
    const unsigned long long q_high = static_cast<unsigned long long>(q) << 32;
    const long long x0 = static_cast<int>(static_cast<unsigned>(x) << 4);
    const long long y0 = static_cast<int>(static_cast<unsigned>(y) << 4);
    const long long x1 = static_cast<int>(static_cast<unsigned>(x + width) << 4);
    const long long y1 = static_cast<int>(static_cast<unsigned>(y + height) << 4);
    env[2] = 6;
    env[5] = 1;
    env[4] = static_cast<unsigned long long>(red & 0xff) | 0x3f80000000000000ull |
             static_cast<unsigned long long>(blue & 0xff) << 16 | static_cast<unsigned long long>(green & 0xff) << 8 |
             static_cast<unsigned long long>(alpha & 0xff) << 24;
    env[6] = static_cast<unsigned long long>(x0) | static_cast<unsigned long long>(y0) << 16 | q_high;
    env[9] = 5;
    env[8] = static_cast<unsigned long long>(x1) | static_cast<unsigned long long>(y1) << 16 | q_high;
    env[11] = 71;
    env[1] = 71;
    env[0] = 0x30000;
    env[3] = 0;
    env[7] = 5;
    env[10] = ztest != 0 ? (static_cast<unsigned long long>(ztest & 3) << 17 | 0x10000u) : 0x30000u;
#if defined(__mips__)
    __asm__ volatile("sync" ::: "memory");
#endif
    return 6;
}
}
