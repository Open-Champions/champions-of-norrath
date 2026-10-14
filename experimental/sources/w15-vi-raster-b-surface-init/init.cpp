// Observed: Init__9VISurfaceR8VIRasterii10VIBitDepthi at 0x0112c820 is 304 bytes in the pinned SLUS-20565 image. Arguments: the
// surface (this), a raster (never read), two ints (stored at +8 and +12), a bit-depth value and one more int (stored at +64).
// Sequence: Deallocate__9VISurface(surface); if that is negative return -1. Store the two ints at +8/+12 and the last int at +64. If
// the depth value (compared unsigned) is above 4 return -1. Otherwise a five-entry jump table sets three words:
//   depth 0: +16 = 0, +40 = 16,  +52 = 2
//   depth 1: +16 = 1, +40 = 256, +52 = 2
//   depth 2: +16 = 5, +40 = 0,   +52 = 2
//   depth 3 and 4: +16 = 2, +40 = 0, +52 = 2
// Then CalcSurfaceOffsets__9VISurface(surface), Allocate__9VISurface(surface) and SetDMAHeaders__9VISurface(surface) in that order;
// the first two returning a negative value, or the last one, makes the result -1. Otherwise it returns 0.
// Inferred: +16 is a pixel-store enum, +40 is the palette entry count (LockPalette reads it), +8/+12 are width and height, and the five
// depths are 4-bit, 8-bit and larger formats; none of those names are confirmed.
// Unknown: the VIBitDepth values, the role of +52 (always 2 here) and +64, and the callees' behaviour (unresolved imports here).
// Nothing here ran on the EE.
extern "C" {
int Deallocate__9VISurface(unsigned char* surface);
int CalcSurfaceOffsets__9VISurface(unsigned char* surface);
int Allocate__9VISurface(unsigned char* surface);
int SetDMAHeaders__9VISurface(unsigned char* surface);
int Init__9VISurfaceR8VIRasterii10VIBitDepthi(unsigned char* self, unsigned char* raster, int width, int height, int depth, int word64) {
    (void)raster;
    if (Deallocate__9VISurface(self) < 0) return -1;
    *reinterpret_cast<int*>(self + 8) = width;
    *reinterpret_cast<int*>(self + 12) = height;
    *reinterpret_cast<int*>(self + 64) = word64;
    if (static_cast<unsigned>(depth) > 4) return -1;
    switch (depth) {
    case 0:
        *reinterpret_cast<int*>(self + 40) = 16;
        *reinterpret_cast<int*>(self + 52) = 2;
        *reinterpret_cast<int*>(self + 16) = 0;
        break;
    case 1:
        *reinterpret_cast<int*>(self + 16) = 1;
        *reinterpret_cast<int*>(self + 40) = 256;
        *reinterpret_cast<int*>(self + 52) = 2;
        break;
    case 2:
        *reinterpret_cast<int*>(self + 16) = 5;
        *reinterpret_cast<int*>(self + 52) = 2;
        *reinterpret_cast<int*>(self + 40) = 0;
        break;
    default:
        *reinterpret_cast<int*>(self + 40) = 0;
        *reinterpret_cast<int*>(self + 52) = 2;
        *reinterpret_cast<int*>(self + 16) = 2;
        break;
    }
    if (CalcSurfaceOffsets__9VISurface(self) < 0) return -1;
    if (Allocate__9VISurface(self) < 0) return -1;
    return SetDMAHeaders__9VISurface(self) < 0 ? -1 : 0;
}
}
