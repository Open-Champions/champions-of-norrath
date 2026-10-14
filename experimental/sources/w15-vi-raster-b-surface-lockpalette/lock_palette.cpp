// Observed: LockPalette__9VISurfaceRiR12VIPixelStorePPUc at 0x0112c9d8 is 164 bytes in the pinned SLUS-20565 image. Arguments: the
// surface (this) and three result pointers. If the surface's word at +28 is not -1 it returns -1 and writes nothing. Otherwise it calls
// ReorderPalette__9VISurface(surface), copies the surface word at +64 into +28, stores 1 in +32 and branches on the word at +40:
//  * zero: *first = 0, *third = 0 and *second = the surface word at +16 (read before the stores);
//  * non-zero: *first = that word, *second = the surface word at +52 and *third = the surface word at +312 plus the word at +44.
// It returns 0 in both cases. This is the palette counterpart of LockMipLevel (same +28/+32 lock words); UnlockPalette releases it.
// Inferred: +40 is the number of palette entries (1 and 256 are written by Init, 16 by one of its cases), the second result is a
// pixel-store enum, the third result is the address of the palette data, and the first is its entry count.
// Unknown: the meaning of +64 here (UnlockPalette compares it with +28), what ReorderPalette does, and the surface size.
// Nothing here ran on the EE.
extern "C" {
void ReorderPalette__9VISurface(unsigned char* surface);
int LockPalette__9VISurfaceRiR12VIPixelStorePPUc(unsigned char* self, int* first, int* second, unsigned* third) {
    if (*reinterpret_cast<int*>(self + 28) != -1) return -1;
    ReorderPalette__9VISurface(self);
    *reinterpret_cast<int*>(self + 28) = *reinterpret_cast<int*>(self + 64);
    *reinterpret_cast<int*>(self + 32) = 1;
    int entries = *reinterpret_cast<int*>(self + 40);
    if (entries == 0) {
        int pixelFormat = *reinterpret_cast<int*>(self + 16);
        *first = 0;
        *third = 0;
        *second = pixelFormat;
    } else {
        *first = entries;
        unsigned address = *reinterpret_cast<unsigned*>(self + 312) + *reinterpret_cast<unsigned*>(self + 44);
        *second = *reinterpret_cast<int*>(self + 52);
        *third = address;
    }
    return 0;
}
}
