/* Observed sceGsSetDefDrawEnv at 0x002d2598, 488 bytes. Arguments: a 128-byte record
 * and five values each sign-extended to 16 bits: pixel mode (psm), width, height,
 * depth-test mode (ztest) and depth pixel mode (zpsm). The record is eight pairs of
 * doublewords, the value first and a register number second:
 *   +0/+8    value ((width + 63) >> 6 & 0x3f) << 16 | (psm & 15) << 24, register 76
 *   +16/+24  value sign-extended-halfword(sceGszbufaddr(psm, width, height)) |
 *            (zpsm & 15) << 24, plus bit 32 when ztest is zero; register 78
 *   +32/+40  value (2048 - width / 2) << 4 | (2048 - height / 2) << 36, register 24
 *            (halving by arithmetic shift, truncated to 16 bits)
 *   +48/+56  value (width - 1) << 16 | (height - 1) << 48, register 64
 *   +64/+72  existing doubleword at +64 with bit 0 set, register 26
 *   +80/+88  existing doubleword at +80 with bit 0 set, register 70
 *   +96/+104 existing doubleword at +96 with bit 0 set when psm & 2, else cleared,
 *            register 69
 *   +112/+120 value 0x30000 when ztest is zero, else 0x10000 | (ztest & 3) << 17,
 *            register 71
 * A memory barrier (sync) follows and the result is 8.
 * Inferred: the register numbers are graphics-synthesizer register addresses of the
 * first drawing context in address/data packing; the result is the quadword count.
 * Unknown: what sceGszbufaddr computes beyond being called with (psm, width, height).
 * The 16-bit sign extension of the zbuffer word is as the code shows it.
 */
extern "C" {
int sceGszbufaddr(int psm, int width, int height);

int sceGsSetDefDrawEnv(unsigned long long* env, int psm_in, int width_in, int height_in, int ztest_in,
                       int zpsm_in) {
    const int psm = static_cast<short>(psm_in);
    const int width = static_cast<short>(width_in);
    const int height = static_cast<short>(height_in);
    const int ztest = static_cast<short>(ztest_in);
    const int zpsm = static_cast<short>(zpsm_in);
    env[1] = 76;
    env[0] = static_cast<unsigned long long>(((width + 63) >> 6) & 0x3f) << 16 |
             static_cast<unsigned long long>(psm & 15) << 24;
    env[3] = 78;
    const long long zbuf = static_cast<short>(sceGszbufaddr(psm, width, height));
    unsigned long long zvalue = static_cast<unsigned long long>(zbuf) | static_cast<unsigned long long>(zpsm & 15) << 24;
    if (ztest == 0) zvalue |= 0x8000ull << 17;
    env[2] = zvalue;
    const long long half_height = static_cast<short>(height >> 1);
    const long long half_width = static_cast<short>(width >> 1);
    const unsigned long long offset_x = static_cast<unsigned long long>(2048 - half_width) << 4;
    const unsigned long long offset_y = static_cast<unsigned long long>(2048 - half_height) << 36;
    const unsigned long long prmode = env[8];
    const unsigned long long colclamp = env[10];
    env[5] = 24;
    env[4] = offset_x | offset_y;
    env[7] = 64;
    env[6] = static_cast<unsigned long long>(width - 1) << 16 | static_cast<unsigned long long>(height - 1) << 48;
    env[9] = 26;
    env[8] = prmode | 1;
    env[11] = 70;
    env[10] = colclamp | 1;
    env[13] = 69;
    env[12] = (psm & 2) ? (env[12] | 1) : (env[12] & ~1ull);
    env[15] = 71;
    env[14] = ztest != 0 ? (static_cast<unsigned long long>(ztest & 3) << 17 | 0x10000u) : 0x30000u;
#if defined(__mips__)
    __asm__ volatile("sync" ::: "memory");
#endif
    return 8;
}
}
