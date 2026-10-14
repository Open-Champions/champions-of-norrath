// Observed: LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc at 0x0112c950 is 100 bytes in the pinned SLUS-20565 image. Arguments: the
// surface (this), a mip level, and three result pointers. The level's record is at surface + 64 + 24*level. If the surface's lock
// word at +28 is not -1 the routine returns -1 and writes nothing. Otherwise it stores the level in +28, stores 1 in +32 and
// writes: *first = record word at +16; *second = surface word at +16; *third = surface word at +312 plus record word at +12.
// It returns 0. The level is not range checked. The paired UnlockMipLevel__9VISurfacei clears +28 again.
// Inferred: +28 is the locked level (-1 when unlocked) and +32 a lock flag; the third result is the address of the level's pixel data
// (a base plus an offset); the first looks like a per-level size or pitch and the second a pixel-format word (the mangled type names
// a pixel-store enum).
// Unknown: the meanings of the first two results, the 24-byte record layout, and the surface size. Nothing here ran on the EE.
extern "C" {
int LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(unsigned char* self, int level, int* first, int* second, unsigned* third) {
    unsigned char* record = self + (level * 24 + 64);
    if (*reinterpret_cast<int*>(self + 28) != -1) return -1;
    *reinterpret_cast<int*>(self + 28) = level;
    *reinterpret_cast<int*>(self + 32) = 1;
    int pixelFormat = *reinterpret_cast<int*>(self + 16);
    int recordValue = *reinterpret_cast<int*>(record + 16);
    unsigned base = *reinterpret_cast<unsigned*>(self + 312);
    *first = recordValue;
    *second = pixelFormat;
    *third = base + *reinterpret_cast<unsigned*>(record + 12);
    return 0;
}
}
