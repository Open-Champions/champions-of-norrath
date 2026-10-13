/* Observed sceGsSetDefDBuffDc at 0x002d2250, 740 bytes. The arguments are the
 * double-buffer record (816 bytes, sixteen-byte aligned) and six values each
 * sign-extended to 16 bits: pixel mode, width, height, depth-test mode, depth
 * pixel mode and a clear flag. The record's parts, by offset:
 *   0 and 40     display environments, both set with sceGsSetDefDispEnv(pixel
 *                mode, width, height, 0, 0);
 *   96 and 464   drawing environments, sceGsSetDefDrawEnv(pixel mode, width,
 *                height, depth-test mode, depth pixel mode);
 *   224 and 592  the same with sceGsSetDefDrawEnv2;
 *   352 and 720  96-byte clear packets, written only when the clear flag is
 *                nonzero: sceGsSetDefClear(depth-test mode, 2048 - width / 2,
 *                2048 - height / 2, width, height, 0, 0, 0, 0, 0) with
 *                halving by arithmetic shift;
 *   80 and 448   16-byte packet headers, always rewritten as: low doubleword
 *                = 22 when the clear flag is nonzero, else 16, OR 0x8000
 *                (end-of-packet) OR 1 << 60 (one register descriptor); high
 *                doubleword = 14.
 * It then asks sceGszbufaddr(pixel mode, width, height) and, when the first
 * doubleword of the graphics parameter record (sceGsGetGParam, read at the
 * start but dereferenced at the end) masked with 0x0000ffff0000ffff equals 1
 * or its low halfword is zero, takes half of that address and replaces the
 * low nine bits of the doublewords at offsets 56, 96 and 224 with the low nine
 * bits of the half. Nothing is returned; the original leaves a stale value in
 * the result register.
 * Inferred: the three adjusted doublewords are frame-buffer base fields of the
 * second buffer; the parameter halfwords at 0 and 4 are the interlace and
 * field/frame modes (as used by sceGsSetDefDispEnv). Not established here.
 */
extern "C" {
unsigned char* sceGsGetGParam();
void sceGsSetDefDispEnv(unsigned char* env, int psm, int width, int height, int dx, int dy);
void sceGsSetDefDrawEnv(unsigned char* env, int psm, int width, int height, int ztest, int zpsm);
void sceGsSetDefDrawEnv2(unsigned char* env, int psm, int width, int height, int ztest, int zpsm);
void sceGsSetDefClear(unsigned char* env, int ztest, int x, int y, int width, int height, int r, int g, int b,
                      int a, int q);
int sceGszbufaddr(int psm, int width, int height);

static unsigned long long& wide(unsigned char* record, int offset) {
    return *reinterpret_cast<unsigned long long*>(record + offset);
}

void sceGsSetDefDBuffDc(unsigned char* db, int psm_in, int width_in, int height_in, int ztest_in, int zpsm_in,
                        int clear_in) {
    const int psm = static_cast<short>(psm_in);
    const int width = static_cast<short>(width_in);
    const int height = static_cast<short>(height_in);
    const int ztest = static_cast<short>(ztest_in);
    const int zpsm = static_cast<short>(zpsm_in);
    const int clear = static_cast<short>(clear_in);
    unsigned char* param = sceGsGetGParam();
    sceGsSetDefDispEnv(db, psm, width, height, 0, 0);
    sceGsSetDefDispEnv(db + 40, psm, width, height, 0, 0);
    sceGsSetDefDrawEnv(db + 96, psm, width, height, ztest, zpsm);
    sceGsSetDefDrawEnv2(db + 224, psm, width, height, ztest, zpsm);
    sceGsSetDefDrawEnv(db + 464, psm, width, height, ztest, zpsm);
    sceGsSetDefDrawEnv2(db + 592, psm, width, height, ztest, zpsm);
    if (clear != 0) {
        const int x = 2048 - (width >> 1);
        const int y = 2048 - (height >> 1);
        sceGsSetDefClear(db + 352, ztest, x, y, width, height, 0, 0, 0, 0, 0);
        sceGsSetDefClear(db + 720, ztest, x, y, width, height, 0, 0, 0, 0, 0);
    }
    const unsigned long long header = (clear != 0 ? 22ull : 16ull) | 0x8000ull | (1ull << 60);
    wide(db, 80) = header;
    wide(db, 88) = 14;
    wide(db, 448) = header;
    wide(db, 456) = 14;
    const int half = sceGszbufaddr(psm, width, height) >> 1;
    const unsigned long long low_bits = static_cast<unsigned long long>(half & 0x1ff);
    const unsigned long long first = *reinterpret_cast<unsigned long long*>(param);
    if ((first & 0x0000ffff0000ffffull) == 0x0000000100000001ull || *reinterpret_cast<short*>(param) == 0) {
        wide(db, 224) = (wide(db, 224) & ~0x1ffull) | low_bits;
        wide(db, 56) = (wide(db, 56) & ~0x1ffull) | low_bits;
        wide(db, 96) = (wide(db, 96) & ~0x1ffull) | low_bits;
    }
}
}
