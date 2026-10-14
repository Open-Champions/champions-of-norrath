// Observed: InitSlots__7VIRFontR8VIRaster at 0x011357d0 is 548 bytes in the pinned SLUS-20565 image. Offsets written +N are byte offsets from this; surface+N from the surface.
//   surface = Surface__8VIRasteri(raster, +12); width = word surface+8; height = word surface+12;
//   LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(surface, 0, &pitch, &store, &buffer); if it is negative, return -1.
//   cell = max(+20, +24); if store == 0, cell += cell & 1. store selects a byte step per cell: 0 -> cell >> 1 (signed), 1 -> cell,
//   2, 3 and 6 -> cell << 2, 4 -> 3 * cell, 5 -> cell << 1; store >= 7 returns -1 with the surface still locked.
//   +44 = (width / cell) * (height / cell) (signed divisions; a zero cell traps with break 7, as the compiler emits for division).
//   Then, unless height < cell, walk rows y = 0, cell, 2*cell, ... while y + cell <= height, and in each row (unless width < cell) cells
//   x = 0, cell, ... while x + cell <= width, numbering slots 0, 1, 2, ... over the whole walk. Slot k is a 56-byte record at
//   (word +68) + 56 * k: word +0 = running byte offset (row * pitch, plus the byte step for each earlier cell in the row);
//   halfword +12 = 0xffff; word +16 = x; word +20 = y; word +8 = k - 1 (slot 0: +56 = 0 and slot 0's +8 = -1); word +4 = k + 1, except
//   the slot with k == +44 - 1, which gets +60 = k and +4 = -1. Finally UnlockMipLevel__9VISurfacei(surface, 0); return -1 if its
//   result is negative, else 0.
// Inferred: +12 is a surface id from CreateResources (it stores CreateSurface's result there), +20 and +24 are the glyph cell
// extent and widest glyph, and the slots are a doubly linked list of glyph cells in a texture (+56 first, +60 last, +8 previous,
// +4 next, -1 for none). The pitch and store come from the lock; the third lock result is unused here.
// Unknown: slot fields +24..+55 (not written here), +12's meaning, and whether the walk can overrun the array when +44 is
// smaller than the number of cells walked (nothing here checks it). The three callees are imports. Nothing here ran on the EE.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif
extern "C" {
unsigned char* Surface__8VIRasteri(unsigned char* raster, int index);
int LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(unsigned char* surface, int level, int* pitch, int* store, unsigned char** buffer);
int UnlockMipLevel__9VISurfacei(unsigned char* surface, int level);

int InitSlots__7VIRFontR8VIRaster(unsigned char* self, unsigned char* raster) {
    int* f = reinterpret_cast<int*>(self);
    unsigned char* surface = Surface__8VIRasteri(raster, f[3]);
    int width = *reinterpret_cast<int*>(surface + 8);
    int height = *reinterpret_cast<int*>(surface + 12);
    int pitch;
    int store;
    unsigned char* buffer;
    if (LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(surface, 0, &pitch, &store, &buffer) < 0) return -1;
    int cell = f[5];
    if (cell < f[6]) cell = f[6];
    if (store == 0) cell += cell & 1;
    int step;
    switch (static_cast<unsigned>(store)) {
    case 0: step = cell >> 1; break;
    case 1: step = cell; break;
    case 2: case 3: case 6: step = cell << 2; break;
    case 4: step = cell * 3; break;
    case 5: step = cell << 1; break;
    default: return -1;
    }
    int columns = width / cell;
    int rows = height / cell;
    f[11] = columns * rows;
    if (!(height < cell)) {
        int slot = 0;
        int y = 0;
        int yEnd = cell;
        for (;;) {
            int offset = y * pitch;
            if (!(width < cell)) {
                unsigned char* slots = NORRATH_WORD_TO_POINTER(static_cast<unsigned>(f[17]));
                int x = 0;
                int xEnd = cell;
                do {
                    unsigned char* record = slots + slot * 56;
                    *reinterpret_cast<unsigned short*>(record + 12) = 0xffff;
                    *reinterpret_cast<int*>(record + 16) = x;
                    *reinterpret_cast<int*>(record + 20) = y;
                    *reinterpret_cast<int*>(record) = offset;
                    if (slot != 0) {
                        *reinterpret_cast<int*>(record + 8) = slot - 1;
                    } else {
                        f[14] = 0;
                        *reinterpret_cast<int*>(slots + 8) = -1;
                    }
                    if (slot != f[11] - 1) {
                        *reinterpret_cast<int*>(record + 4) = slot + 1;
                    } else {
                        f[15] = slot;
                        *reinterpret_cast<int*>(record + 4) = -1;
                    }
                    x = xEnd;
                    ++slot;
                    xEnd = x + cell;
                    offset += step;
                } while (!(width < xEnd));
            }
            y = yEnd;
            yEnd = y + cell;
            if (height < yEnd) break;
        }
    }
    return UnlockMipLevel__9VISurfacei(surface, 0) < 0 ? -1 : 0;
}
}
