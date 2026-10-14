// Observed: CalcSurfaceOffsets__9VISurface at 0x0112c2b0 is 592 bytes in the pinned SLUS-20565 image. All offsets are bytes from this.
// Palette part: the word at +52 selects a block size. For 2 the byte count is 4 * (word at +40); for 5 it is 2 * (word at +40); for any
// other value +44, +48 and +60 are set to 0 and the running total starts at 0. For 2 and 5: +44 = 64, +48 = pad16(bytes) + 64 where
// pad16 rounds up to a multiple of 16, +60 = (+48) >> 4 (signed), and the running total starts at +48. +56 is set to 0 in every case.
// Level part: w = +8, h = +12, levels = 0. If w < 16, or h < 16, or the signed word at +64 is not positive, no level is laid out.
// Otherwise, for each level i = 0, 1, ... it calls CalcMipLevelStride__9VISurfacei(this, w); a negative result returns -1 at once
// (stores already made stay, +308 and +64 are not written). With rows = stride * h and a = pad16(rows + 64) it stores
//   +68+24i = w, +72+24i = h, +76+24i = total + 64, +80+24i = stride, +84+24i = total, +88+24i = a >> 4,
// then total += a, w = max(w >> 1, 1), h = max(h >> 1, 1), levels += 1. It goes on to the next level only if the halved w and h are
// both at least 16, levels < 10 and levels < the word at +64 (re-read each time). Finally +308 = total, +64 = levels, return 0.
// Inferred: +40 is a palette entry count and +52 its pixel-store value (the two supported values match the 32-bit and 16-bit steps used
// by the font loader); the records from +64 + 24 * level are the ones LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc reads (+12 data
// offset, +16 the first lock result); +308 is the byte size Allocate__9VISurface requests. The +88+24i word falls on the next record's first
// word, which is overwritten by the next level's stores or by +64 for level 0's slot.
// Unknown: the stride unit, what +60 and +84 are used for, and CalcMipLevelStride's behaviour (an import). Nothing here ran on the EE.
extern "C" {
int CalcMipLevelStride__9VISurfacei(unsigned char* self, int width);

int CalcSurfaceOffsets__9VISurface(unsigned char* self) {
    int* s = reinterpret_cast<int*>(self);
    int total = 0;
    int kind = s[13];
    s[14] = 0;
    if (kind == 2 || kind == 5) {
        int count = s[10];
        int bytes = kind == 2 ? count << 2 : count << 1;
        int padded = ((bytes + 15) & ~15) + 64;
        s[11] = 64;
        s[12] = padded;
        s[15] = padded >> 4;
        total = padded;
    } else {
        s[11] = 0;
        s[12] = 0;
        s[15] = 0;
    }
    int w = s[2];
    int h = s[3];
    int levels = 0;
    if (!(w < 16) && !(h < 16) && s[16] > 0) {
        for (;;) {
            int stride = CalcMipLevelStride__9VISurfacei(self, w);
            int rows = stride * h;
            if (stride < 0) return -1;
            int block = rows + 64;
            int padded = (block & 15) != 0 ? (block & ~15) + 16 : block;
            int* level = s + 17 + 6 * levels;
            level[0] = w;
            level[2] = total + 64;
            level[3] = stride;
            level[1] = h;
            level[4] = total;
            level[5] = padded >> 4;
            w >>= 1;
            if (w < 1) w = 1;
            h >>= 1;
            if (h < 1) h = 1;
            ++levels;
            total += padded;
            if (w < 16 || h < 16 || levels >= 10 || levels >= s[16]) break;
        }
    }
    s[77] = total;
    s[16] = levels;
    return 0;
}
}
