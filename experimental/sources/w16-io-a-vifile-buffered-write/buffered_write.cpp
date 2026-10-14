// Observed: BufferedWrite__6VIFilePCvi at 0x010ca5d0, 768 bytes. Arguments: the file, a source buffer and a byte count n. File fields
// (offsets from the file): word +0 (descriptor), +8 (end flag, cleared first), 64-bit +16 (position), +24 (the descriptor's own
// position), +136 (file offset of the cached block), +144 and +152 (start and end of the dirty range inside the cache), +160
// (position inside the cache), +176 (bytes held), and the word +188 (cache address).
// "Write back" below is the sequence BufferedSeek__6VIFilelQ26VIFile10SeekOrigin also uses: when the dirty range is non-empty, T = +136
// + +144 and size = (int)(+152 - +144); sceLseek(descriptor, (int)T, 0) is called when T differs from +24 (its result goes to +24 and a
// result other than T skips the write); sceWrite(descriptor, cache + low word of +144, size) adds its result to +24 and, when it equals
// size, clears +144 and +152.
//  n > 2048: when the cache holds bytes, write back; then BufferClear__6VIFile(file); when +24 differs from +16, sceLseek(descriptor,
//    low word of +16, 0) stores its result at +24 and a result other than the position returns -1; then sceWrite(descriptor, buffer, n)
//    adds its result to +24 and +16 and the result is that count when it equals n, otherwise -1.
//  n <= 2048: when the cache holds no bytes, +136 = the position. When +160 is not negative and +160 + n <= 32767, the copy goes to
//    cache + +160. Otherwise the cache is rewritten: when it holds bytes, write back; then k = 0 (the held count when that is not
//    positive), and a loop copies bytes from cache[(low word of +16) - k] up to the position into cache[0], re-reading the cache
//    address each pass and never advancing the destination (the image does this; with k = 0 the loop runs only when the position is
//    positive); then +160, +152, +176 and +144 are set to k and +136 to the position minus k, and the copy goes to cache + k.
//    memcpy(destination, buffer, n) copies the data. Then P = +160: +144 = min(+144, P), +152 = max(+152, P + n), +176 = max(+176,
//    P + n), +16 = +16 + n, +160 = P + n, and the result is n.
// Inferred: a write-back byte cache of up to 32 KiB over a descriptor; large writes bypass it.
// Unknown: the member names, the intent of the byte loop whose destination never advances, and why the dirty start is min(+144, P)
// even for an empty dirty range.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
#ifndef NORRATH_MEMCPY_SYMBOL
#define NORRATH_MEMCPY_SYMBOL "memcpy"
#endif
extern "C" {
int sceLseek(int descriptor, int offset, int whence);
int sceWrite(int descriptor, unsigned buffer, int size);
int BufferClear__6VIFile(unsigned char* file);
// memcpy with the destination as an address word (the image's 32-bit pointer); same symbol and calling convention.
void* norrath_buffered_write_memcpy(unsigned destination, const void* source, unsigned count) __asm__(NORRATH_MEMCPY_SYMBOL);
}

static inline long long& quad(unsigned char* file, int offset) { return *reinterpret_cast<long long*>(file + offset); }
static inline unsigned& word(unsigned char* file, int offset) { return *reinterpret_cast<unsigned*>(file + offset); }

static void writeBack(unsigned char* file) {
    long long dirtyStart = quad(file, 144);
    long long dirtyEnd = quad(file, 152);
    if (dirtyStart == dirtyEnd) return;
    long long target = quad(file, 136) + dirtyStart;
    int size = static_cast<int>(dirtyEnd - dirtyStart);
    if (target != quad(file, 24)) {
        long long moved = sceLseek(static_cast<int>(word(file, 0)), static_cast<int>(target), 0);
        quad(file, 24) = moved;
        if (target != moved) return;
    }
    int written = sceWrite(static_cast<int>(word(file, 0)), word(file, 188) + word(file, 144), size);
    quad(file, 24) = written + quad(file, 24);
    if (written == size) {
        quad(file, 144) = 0;
        quad(file, 152) = 0;
    }
}

extern "C" {
int BufferedWrite__6VIFilePCvi(unsigned char* file, const void* buffer, int count) {
    *reinterpret_cast<int*>(file + 8) = 0;
    if (count > 2048) {
        if (quad(file, 176) != 0) writeBack(file);
        BufferClear__6VIFile(file);
        if (quad(file, 24) != quad(file, 16)) {
            long long moved = sceLseek(static_cast<int>(word(file, 0)), static_cast<int>(word(file, 16)), 0);
            long long expected = quad(file, 16);
            quad(file, 24) = moved;
            if (moved != expected) return -1;
        }
        int written = sceWrite(static_cast<int>(word(file, 0)), static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(buffer)), count);
        long long position = quad(file, 24);
        long long next = quad(file, 16) + written;
        quad(file, 16) = next;
        quad(file, 24) = position + written;
        return written == count ? written : -1;
    }
    long long held = quad(file, 176);
    if (held == 0) quad(file, 136) = quad(file, 16);
    long long inside = quad(file, 160);
    if (!(inside < 0 || 32767 < inside + count)) {
        norrath_buffered_write_memcpy(word(file, 188) + word(file, 160), buffer, count);
    } else {
        if (held != 0) {
            writeBack(file);
            held = quad(file, 176);
        }
        long long keep = held > 0 ? 0 : held;
        int k = static_cast<int>(keep);
        int from = static_cast<int>(word(file, 16)) - k;
        long long position = quad(file, 16);
        if (from < position) {
            do {
                unsigned char* cache = bytes_at(word(file, 188));
                cache[0] = cache[from];
                ++from;
            } while (from < quad(file, 16));
        }
        position = quad(file, 16);
        quad(file, 160) = k;
        quad(file, 152) = k;
        quad(file, 176) = k;
        quad(file, 144) = k;
        quad(file, 136) = position - k;
        norrath_buffered_write_memcpy(word(file, 188) + word(file, 160), buffer, count);
    }
    long long start = quad(file, 160);
    long long length = quad(file, 176);
    long long dirtyStart = quad(file, 144);
    long long dirtyEnd = quad(file, 152);
    long long end = start + count;
    long long position = quad(file, 16);
    if (start < dirtyStart) dirtyStart = start;
    if (length < end) length = end;
    if (!(end < dirtyEnd)) dirtyEnd = end;
    quad(file, 16) = position + count;
    quad(file, 176) = length;
    quad(file, 144) = dirtyStart;
    quad(file, 152) = dirtyEnd;
    quad(file, 160) = end;
    return count;
}
}
