/* Observed sceGsSetDefDrawEnv2 at 0x002d2970, 480 bytes. Same shape and arguments as
 * sceGsSetDefDrawEnv (the second drawing context), with these differences: the first
 * value uses (width >> 6) & 0x3f as the buffer width field instead of (width + 63) >> 6,
 * and the register numbers are 77, 79, 25, 65 (for the pairs at +0, +16, +32, +48) and 72
 * (at +112); the registers at +64, +88 and +104 are the same (26, 70, 69). The zbuffer
 * word, offset, scissor, bit-0 updates at +64/+80/+96, test value, the final memory
 * barrier (sync) and the result 8 are computed as in sceGsSetDefDrawEnv.
 * Inferred: the registers are the second context's frame, zbuffer, offset, scissor and
 * test registers; the different width rounding is exactly what the code does and is not
 * explained here.
 * Unknown: what sceGszbufaddr computes.
 */
extern "C" {
int sceGszbufaddr(int psm, int width, int height);

int sceGsSetDefDrawEnv2(unsigned long long* env, int psm_in, int width_in, int height_in, int ztest_in,
                       int zpsm_in) {
    const int psm = static_cast<short>(psm_in);
    const int width = static_cast<short>(width_in);
    const int height = static_cast<short>(height_in);
    const int ztest = static_cast<short>(ztest_in);
    const int zpsm = static_cast<short>(zpsm_in);
    env[1] = 77;
    env[0] = static_cast<unsigned long long>((width >> 6) & 0x3f) << 16 |
             static_cast<unsigned long long>(psm & 15) << 24;
    env[3] = 79;
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
    env[5] = 25;
    env[4] = offset_x | offset_y;
    env[7] = 65;
    env[6] = static_cast<unsigned long long>(width - 1) << 16 | static_cast<unsigned long long>(height - 1) << 48;
    env[9] = 26;
    env[8] = prmode | 1;
    env[11] = 70;
    env[10] = colclamp | 1;
    env[13] = 69;
    env[12] = (psm & 2) ? (env[12] | 1) : (env[12] & ~1ull);
    env[15] = 72;
    env[14] = ztest != 0 ? (static_cast<unsigned long long>(ztest & 3) << 17 | 0x10000u) : 0x30000u;
#if defined(__mips__)
    __asm__ volatile("sync" ::: "memory");
#endif
    return 8;
}
}
