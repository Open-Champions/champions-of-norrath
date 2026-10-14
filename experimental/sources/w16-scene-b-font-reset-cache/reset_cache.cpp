// Observed: ResetCache__7VIRFontR8VIRaster at 0x01135b18 is 356 bytes. Arguments: the
// font and a raster. Offsets are bytes from the font; words hold 32-bit addresses.
//   1. surface = Surface__8VIRasteri(raster, word +12).
//   2. LockPalette__9VISurfaceRiR12VIPixelStorePPUc(surface, &count, &store, &dest);
//      a negative result returns -1.
//   3. memcpy(dest, word +72, 4 * min(count, word +52)).
//   4. UnlockPalette__9VISurface(surface); a negative result returns -1.
//   5. Words +56 and +60 are set to -1. If the count at +44 is positive, the table at
//      word +68 is walked: entries are 56 bytes; for entry i the halfword at +12 is
//      set to -1, the word at +8 is i - 1 (or -1 for entry 0, which also sets word
//      +56 of the font to 0), the word at +4 is i + 1 (or -1 for the last entry, which
//      also sets font word +60 to i). The +44 count is re-read every pass.
//   6. If the count at +36 is positive, the halfword at (word +64) + 8 + 12 * k is set
//      to -1 for k < that count.
//   7. It returns 0.
// Inferred: this rebuilds a doubly linked free/used list over the 56-byte glyph cache
// entries (+56 head, +60 tail) and clears per-glyph slot ids in 12-byte records, after
// re-uploading a palette. Unknown: field names, the VIPixelStore size (it is passed by
// reference in an 8-byte stack slot here), and what the lists track.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
void* Surface__8VIRasteri(void* raster, int index);
int LockPalette__9VISurfaceRiR12VIPixelStorePPUc(void* surface, int* count, void* store, unsigned char** dest);
int UnlockPalette__9VISurface(void* surface);
void* memcpy(void* dest, const void* source, unsigned size);

namespace {
struct Locals {
    int count;
    unsigned char* dest;
    unsigned store[2];
};
}

int ResetCache__7VIRFontR8VIRaster(unsigned char* font, void* raster) {
    unsigned* w = reinterpret_cast<unsigned*>(font);
    void* surface = Surface__8VIRasteri(raster, static_cast<int>(w[3]));
    Locals locals;
    if (LockPalette__9VISurfaceRiR12VIPixelStorePPUc(surface, &locals.count, locals.store, &locals.dest) < 0)
        return -1;
    int count = locals.count;
    if (static_cast<int>(w[13]) < count)
        count = static_cast<int>(w[13]);
    memcpy(locals.dest, at(w[18]), static_cast<unsigned>(count) << 2);
    if (UnlockPalette__9VISurface(surface) < 0)
        return -1;
    w[15] = 0xffffffffu;
    w[14] = 0xffffffffu;
    int i = 0;
    if (static_cast<int>(w[11]) > 0) {
        unsigned char* base = at(w[17]);
        do {
            unsigned char* entry = base + i * 56;
            *reinterpret_cast<unsigned short*>(entry + 12) = 0xffff;
            if (i != 0) {
                *reinterpret_cast<int*>(entry + 8) = i - 1;
            } else {
                w[14] = 0;
                *reinterpret_cast<int*>(base + 8) = -1;
            }
            if (i != static_cast<int>(w[11]) - 1) {
                *reinterpret_cast<int*>(entry + 4) = i + 1;
            } else {
                w[15] = static_cast<unsigned>(i);
                *reinterpret_cast<int*>(entry + 4) = -1;
            }
            ++i;
        } while (i < static_cast<int>(w[11]));
    }
    int slots = static_cast<int>(w[9]);
    if (slots > 0) {
        unsigned char* p = at(w[16]) + 8;
        do {
            *reinterpret_cast<unsigned short*>(p) = 0xffff;
            p += 12;
        } while (--slots != 0);
    }
    return 0;
}
}
