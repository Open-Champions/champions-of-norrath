// Observed: Cache__7VIRFontR8VIRasterPCciPi at 0x01134a18, 972 bytes (the mangled name lists
// the parameters as raster, text, limit and output list). With F the font:
//  - surface = Surface__8VIRasteri(raster, F word +12); iw = 1.0f / (float)(surface word +8) and
//    ih = 1.0f / (float)(surface word +12).
//  - For each character of the text, while the next character is nonzero and fewer than `limit`
//    glyphs have been handled: g = FindChar__7VIRFontUs(F, character as unsigned); if negative
//    use FindChar(F, '?'), and if that is negative too return -1.
//    Glyph records are 12 bytes at the pointer F word +64: +0 source offset into the font data
//    (pointer F word +84), +4 16-bit row byte count, +8 16-bit cache-slot index (0xffff when not
//    cached), +10 8-bit width. Slot records are 56 bytes at the pointer F word +68: +0 target
//    offset in the locked surface, +4 next and +8 previous slot of an LRU list (-1 ends it), +12
//    16-bit glyph index held, +16 x and +20 y (ints), +24..+52 eight floats.
//  - If the glyph has no slot: take the slot F word +56 (the list head), clear the cache-slot
//    index of the glyph that slot currently holds (if any), assign the slot to the glyph, and fill
//    the slot's eight floats with the texture-coordinate quad (x*iw, y*ih), ((x+w)*iw, y*ih),
//    ((x+w)*iw, (y+H)*ih), (x*iw, (y+H)*ih), where H = F word +20 and w is the glyph width. On the
//    first miss only, LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(surface, 0, &pitch, &store,
//    &buffer) is called (negative returns -1). Then H rows of the glyph's row byte count are
//    copied with memcpy from the font data into buffer + the slot's offset, advancing the source
//    by the row byte count and the target by the pitch (F word +20 is re-read for the loop test).
//  - In every case the slot is unlinked from the LRU list (head F word +56 / tail F word +60
//    are patched when it was an end) and appended at the tail (the old tail's next is set
//    without checking for -1), and the slot index is stored at output[count]; count increases.
//  - After the loop: if F word +44 is less than count return -1; if a lock was taken call
//    UnlockMipLevel__9VISurfacei(surface, 0) and return -1 if it is negative; return count.
// Inferred: a glyph cache for text drawing: a small surface holds recently used glyph bitmaps in
// least-recently-used order, and the output list tells the caller which slot holds each glyph.
// Unknown: the member names, the VIPixelStore type (four bytes assumed), the meaning of F word
// +44 (a capacity is assumed) and the font data layout beyond the fields used.
#if defined(NORRATH_HOST_CONTRACT)
// On the 64-bit host a pointer field is modelled as a 32-bit offset into an arena.
extern "C" unsigned char* norrath_host_load_pointer(const void* field);
#define NORRATH_LOAD_POINTER(field) norrath_host_load_pointer(field)
#else
#define NORRATH_LOAD_POINTER(field) (*reinterpret_cast<unsigned char* const*>(field))
#endif

extern "C" {
void* Surface__8VIRasteri(void* raster, int index);
int FindChar__7VIRFontUs(void* font, unsigned short character);
int LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(void* surface, int level, int* pitch, int* store, unsigned char** buffer);
int UnlockMipLevel__9VISurfacei(void* surface, int level);
#if defined(NORRATH_HOST_CONTRACT)
void* VICopyBytes(void* destination, const void* source, unsigned int length);
#else
void* VICopyBytes(void* destination, const void* source, unsigned int length) __asm__("memcpy");
#endif

int Cache__7VIRFontR8VIRasterPCciPi(void* font, void* raster, const char* text, int limit, int* output) {
    unsigned char* f = static_cast<unsigned char*>(font);
    unsigned char* surface = static_cast<unsigned char*>(Surface__8VIRasteri(raster, *reinterpret_cast<int*>(f + 12)));
    float iw = 1.0f / static_cast<float>(*reinterpret_cast<int*>(surface + 8));
    float ih = 1.0f / static_cast<float>(*reinterpret_cast<int*>(surface + 12));
    int pitch, store;
    unsigned char* buffer;
    int locked = 0;
    int count = 0;
    if (*text != 0 && limit > 0) {
        do {
            int glyph = FindChar__7VIRFontUs(font, static_cast<unsigned char>(*text));
            if (glyph < 0) {
                glyph = FindChar__7VIRFontUs(font, 63);
                if (glyph < 0) return -1;
            }
            unsigned char* glyphs = NORRATH_LOAD_POINTER(f + 64);
            unsigned char* record = glyphs + glyph * 12;
            unsigned char* slots = NORRATH_LOAD_POINTER(f + 68);
            int index = *reinterpret_cast<unsigned short*>(record + 8);
            if (index == 0xffff) {
                int height = *reinterpret_cast<int*>(f + 20);
                index = *reinterpret_cast<int*>(f + 56);
                unsigned char* slot = slots + index * 56;
                int previous = *reinterpret_cast<unsigned short*>(slot + 12);
                if (previous != 0xffff) *reinterpret_cast<unsigned short*>(glyphs + previous * 12 + 8) = 0xffff;
                int x = *reinterpret_cast<int*>(slot + 16);
                int y = *reinterpret_cast<int*>(slot + 20);
                float* quad = reinterpret_cast<float*>(slot + 24);
                float left = static_cast<float>(x);
                float top = static_cast<float>(y);
                *reinterpret_cast<unsigned short*>(record + 8) = static_cast<unsigned short>(index);
                *reinterpret_cast<unsigned short*>(slot + 12) = static_cast<unsigned short>(glyph);
                float bottom = static_cast<float>(y + height);
                left *= iw;
                top *= ih;
                bottom *= ih;
                quad[0] = left;
                quad[1] = top;
                unsigned char width = record[10];
                quad[3] = top;
                quad[2] = static_cast<float>(x + width) * iw;
                quad[5] = bottom;
                quad[4] = static_cast<float>(x + record[10]) * iw;
                quad[7] = bottom;
                quad[6] = left;
                if (locked == 0) {
                    if (LockMipLevel__9VISurfaceiRiR12VIPixelStorePPUc(surface, 0, &pitch, &store, &buffer) < 0) return -1;
                    glyphs = NORRATH_LOAD_POINTER(f + 64);
                    locked = 1;
                    slots = NORRATH_LOAD_POINTER(f + 68);
                    height = *reinterpret_cast<int*>(f + 20);
                    record = glyphs + glyph * 12;
                }
                int target = *reinterpret_cast<int*>(slots + index * 56);
                int row = 0;
                const unsigned char* source = NORRATH_LOAD_POINTER(f + 84) + *reinterpret_cast<int*>(record);
                int bytes = *reinterpret_cast<unsigned short*>(record + 4);
                if (height > 0) {
                    unsigned char* destination = buffer + target;
                    do {
                        VICopyBytes(destination, source, static_cast<unsigned int>(bytes));
                        ++row;
                        source += bytes;
                        destination += pitch;
                    } while (row < *reinterpret_cast<int*>(f + 20));
                }
                slots = NORRATH_LOAD_POINTER(f + 68);
            }
            unsigned char* slot = slots + index * 56;
            int previous = *reinterpret_cast<int*>(slot + 8);
            if (previous != -1) *reinterpret_cast<int*>(slots + previous * 56 + 4) = *reinterpret_cast<int*>(slot + 4);
            else *reinterpret_cast<int*>(f + 56) = *reinterpret_cast<int*>(slot + 4);
            int next = *reinterpret_cast<int*>(slot + 4);
            if (next != -1) *reinterpret_cast<int*>(slots + next * 56 + 8) = *reinterpret_cast<int*>(slot + 8);
            else *reinterpret_cast<int*>(f + 60) = *reinterpret_cast<int*>(slot + 8);
            int tail = *reinterpret_cast<int*>(f + 60);
            *reinterpret_cast<int*>(slots + tail * 56 + 4) = index;
            *reinterpret_cast<int*>(slot + 4) = -1;
            *reinterpret_cast<int*>(slot + 8) = *reinterpret_cast<int*>(f + 60);
            *reinterpret_cast<int*>(f + 60) = index;
            output[count] = glyph;
            ++count;
            ++text;
        } while (*text != 0 && count < limit);
    }
    if (*reinterpret_cast<int*>(f + 44) < count) return -1;
    if (locked != 0) {
        if (UnlockMipLevel__9VISurfacei(surface, 0) < 0) return -1;
    }
    return count;
}
}
