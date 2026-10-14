// Observed: CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo at 0x01132508 is 752 bytes in the pinned SLUS-20565 image.
// The cache argument is not used. From a surface s (words at s+8 width, s+12 height, s+16 a type 0..5, s+40, s+52, s+64 a level count) it
// fills the info record d (words at d+4, d+8, d+12, d+20, d+24, d+28, d+72, d+76, d+80 and an array from d+32) and returns 0, or -1 for
// an unsupported combination (state already written stays written):
//   * If s+40 is zero: d+12, d+4 and d+8 are set to 0. Otherwise by s+52: 2 -> d+12=0, d+4=0, d+8=s[40]*4; 3 -> d+12=0, d+4=1,
//     d+8=s[40]*4; 5 -> d+12=0, d+4=2, d+8=s[40]*2; any other value returns -1. d+8 is then rounded up to a multiple of 256.
//   * s+16 selects d+20: 0 -> 20, 1 -> 19, 2 -> 0, 3 -> 1, 5 -> 2; 4 and values above 5 return -1.
//   * d+24=0, d+28=0, d+72=d+76=d+80=-1. Then for up to s[64] levels, while the (halved each level) width and height are both at least
//     16: store d+24 into the word at d+32+4*d[28], compute a level size from the current width w and height h by d+20 (20: max(w,128) *
//     (w > 128 ? max(h,128) : h) / 2; 19: max(w,128) * (w > 128 ? max(h,64) : h); 0 or 1: max(w,64) * (w > 64 ? max(h,32) : h) * 4; 2:
//     max(w,64) * (w > 64 ? max(h,64) : h) * 2; any other d+20 returns -1), round the size up to a multiple of 256, add it to d+24,
//     increment d+28, and halve w and h.
//   * Finally d+24 is rounded up to a multiple of 8192.
// Inferred: this computes how many bytes of EE-side (GS local memory) space a texture and its mip levels need, with per-level offsets in
// d+32.. and a palette size in d+8; d+20 looks like a GS pixel storage mode (20, 19, 0, 1, 2 resemble PSMT4HH-style codes and the 24-bit
// and 32-bit modes - not verified). The asymmetric height rounding (only applied when the width exceeds the minimum) is as observed.
// Unknown: the true meaning of every field, the +72/+76/+80 sentinels, and why the asymmetry exists. Nothing here ran on the EE.
extern "C" {
int CalcCacheInfo__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfo(unsigned char*, unsigned char* surface, unsigned char* info) {
    int* s = reinterpret_cast<int*>(surface);
    int* d = reinterpret_cast<int*>(info);
    if (s[10] == 0) {
        d[3] = 0;
        d[1] = 0;
        d[2] = 0;
    } else {
        int mode = s[13];
        int bytes;
        if (mode == 3) {
            d[3] = 0;
            d[1] = 1;
            bytes = s[10] << 2;
        } else if (mode < 4) {
            if (mode != 2) return -1;
            d[3] = 0;
            d[1] = 0;
            bytes = s[10] << 2;
        } else {
            if (mode != 5) return -1;
            d[3] = 0;
            d[1] = 2;
            bytes = s[10] << 1;
        }
        d[2] = bytes;
        int low = d[2] & 0xff;
        d[2] = low == 0 ? d[2] : d[2] - low + 256;
    }
    switch (static_cast<unsigned>(s[4])) {
        case 0: d[5] = 20; break;
        case 1: d[5] = 19; break;
        case 2: d[5] = 0; break;
        case 3: d[5] = 1; break;
        case 5: d[5] = 2; break;
        default: return -1;
    }
    d[6] = 0;
    d[20] = -1;
    d[18] = -1;
    d[19] = -1;
    d[7] = 0;
    int width = s[2];
    int height = s[3];
    int levels = s[16];
    int i = 0;
    if (levels > 0) {
        for (;;) {
            if (width < 16 || height < 16) break;
            d[8 + d[7]] = d[6];
            unsigned format = static_cast<unsigned>(d[5]);
            int size;
            int w;
            int h;
            switch (format) {
                case 20:
                    w = width < 128 ? 128 : width;
                    h = width <= 128 ? height : (height < 128 ? 128 : height);
                    size = (w * h) >> 1;
                    break;
                case 19:
                    w = width < 128 ? 128 : width;
                    h = width <= 128 ? height : (height < 64 ? 64 : height);
                    size = w * h;
                    break;
                case 2:
                    w = width < 64 ? 64 : width;
                    h = width <= 64 ? height : (height < 64 ? 64 : height);
                    size = (w * h) << 1;
                    break;
                case 0:
                case 1:
                    w = width < 64 ? 64 : width;
                    h = width <= 64 ? height : (height < 32 ? 32 : height);
                    size = (w * h) << 2;
                    break;
                default:
                    return -1;
            }
            int rounded = size;
            int rest = size & 0xff;
            if (rest != 0) rounded = size - rest + 256;
            d[6] = d[6] + rounded;
            d[7] = d[7] + 1;
            ++i;
            width >>= 1;
            height >>= 1;
            if (i >= levels) break;
        }
    }
    int total = d[6];
    int low = total & 0x1fff;
    d[6] = low == 0 ? total : total - low + 8192;
    return 0;
}
}
