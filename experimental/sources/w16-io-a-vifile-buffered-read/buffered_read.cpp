// Observed: BufferedRead__6VIFilePvi at 0x010ca088, 1352 bytes. Arguments: the file, a destination buffer and a byte count. File fields
// (offsets from the file): word +0 (descriptor), +8 (end flag, cleared first), 64-bit +16 (position) and +24 (the descriptor's own
// position), word +72 (first sector of the file), unsigned word +76 (size), pointer +108 (the CD read-mode block, passed as file + 108),
// 64-bit +136 (file offset of the cached block), +144 and +152 (dirty range), +160 (position inside the cache), word +168 (a flag that
// the cached block ends at the end of the file), 64-bit +176 (bytes held) and word +188 (cache address).
// Calls: BufferedReadCopy__6VIFilePvi(file, destination, count) copies from the cache and returns the bytes copied;
// CDSync__6VIFilei(file, 0) returns non-zero on failure; CDRead__6VIFileUiUiPvP10sceCdRMode(file, sector, sectors, cache, file + 108)
// returns zero on failure; BufferClear__6VIFile resets the cache. "Write back" is the dirty-range sequence BufferedSeek__6VIFile... uses
// (sceLseek to +136 + +144 when not already there, sceWrite of the dirty bytes, +24 updated, the dirty range cleared when the write
// is complete).
//   1. got = BufferedReadCopy(file, buffer, count); rem = count - got. rem == 0 returns got.
//   2. When +168 is set: the end flag is set to 1 and got is returned.
//   3. Head, when the position is not a multiple of 2048: write back when the cache holds bytes, BufferClear, block = position / 2048,
//      +136 = block << 11, +160 = position - +136, CDSync (failure returns -1), CDRead one sector (failure returns -1), CDSync (failure
//      returns -1); then +176/+168 = size - +136 and 1 when that is below 2048, otherwise 2048 and 0; the new bytes are copied with
//      BufferedReadCopy; a complete read returns got, a short one with +168 set sets the end flag and returns got. Otherwise it
//      continues below.
//   4. Body, when more than 2048 bytes remain: write back, BufferClear, then min(rem, size - position) / 2048 whole sectors are read
//      in chunks of up to 16 sectors (CDSync, CDRead, CDSync; any failure returns -1), each chunk setting +136 = sector << 11,
//      +160 = position - +136, +176 = chunk << 11 and being copied out with BufferedReadCopy.
//   5. Tail, when bytes remain: rem == 0 returns got; otherwise write back when the cache holds bytes and BufferClear; a position equal
//      to the size sets the end flag and returns got; else read max(1, min(16, sectors left in the file)) sectors from the position's
//      sector (CDSync, CDRead, CDSync with the same failure result), set +176 to the smaller of size - position and the sectors read in
//      bytes (with +168 = 1 in the first case, 0 in the second), copy the data out, and return got, setting the end flag when
//      the copy came up short and +168 is set.
// Inferred: a sector-cached read over a CD-style file; 2048 is the sector size. A failure after some data was copied still returns -1.
// Unknown: the member names, and the units of the +72 / +76 words beyond what the arithmetic shows.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
extern "C" {
int sceLseek(int descriptor, int offset, int whence);
int sceWrite(int descriptor, unsigned buffer, int size);
int BufferClear__6VIFile(unsigned char* file);
int BufferedReadCopy__6VIFilePvi(unsigned char* file, unsigned char* destination, int count);
int CDSync__6VIFilei(unsigned char* file, int mode);
int CDRead__6VIFileUiUiPvP10sceCdRMode(unsigned char* file, unsigned sector, unsigned sectors, unsigned cache, unsigned char* mode);
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

static inline int sectorOf(long long value) {
    return static_cast<int>((value < 0 ? value + 2047 : value) >> 11);
}

extern "C" {
int BufferedRead__6VIFilePvi(unsigned char* file, unsigned char* buffer, int count) {
    *reinterpret_cast<int*>(file + 8) = 0;
    int got = BufferedReadCopy__6VIFilePvi(file, buffer, count);
    int rem = count - got;
    if (rem == 0) return got;
    if (word(file, 168) != 0) {
        *reinterpret_cast<int*>(file + 8) = 1;
        return got;
    }
    if ((quad(file, 16) & 0x7ff) != 0) {
        if (quad(file, 176) != 0) writeBack(file);
        BufferClear__6VIFile(file);
        long long position = quad(file, 16);
        int block = sectorOf(position);
        long long start = block << 11;
        quad(file, 136) = start;
        quad(file, 160) = position - start;
        if (CDSync__6VIFilei(file, 0) != 0) return -1;
        if (CDRead__6VIFileUiUiPvP10sceCdRMode(file, block + word(file, 72), 1, word(file, 188), file + 108) == 0) return -1;
        if (CDSync__6VIFilei(file, 0) != 0) return -1;
        long long room = static_cast<long long>(word(file, 76)) - quad(file, 136);
        if (room < 2048) {
            quad(file, 176) = room;
            word(file, 168) = 1;
        } else {
            word(file, 168) = 0;
            quad(file, 176) = 2048;
        }
        int copied = BufferedReadCopy__6VIFilePvi(file, buffer + got, rem);
        rem -= copied;
        got += copied;
        if (rem == 0) return got;
        if (word(file, 168) != 0) {
            *reinterpret_cast<int*>(file + 8) = 1;
            return got;
        }
    }
    if (rem > 2048) {
        if (quad(file, 176) != 0) writeBack(file);
        BufferClear__6VIFile(file);
        long long position = quad(file, 16);
        long long left = static_cast<long long>(word(file, 76)) - position;
        long long limit = left < rem ? left : rem;
        int sector = sectorOf(position);
        int sectors = sectorOf(limit);
        if (sectors > 0) {
            do {
                int chunk = sectors < 17 ? sectors : 16;
                int failed = CDSync__6VIFilei(file, 0);
                sectors -= chunk;
                if (failed != 0) return -1;
                if (CDRead__6VIFileUiUiPvP10sceCdRMode(file, sector + word(file, 72), chunk, word(file, 188), file + 108) == 0) return -1;
                if (CDSync__6VIFilei(file, 0) != 0) return -1;
                long long start = sector << 11;
                quad(file, 136) = start;
                quad(file, 160) = quad(file, 16) - start;
                quad(file, 176) = chunk << 11;
                int copied = BufferedReadCopy__6VIFilePvi(file, buffer + got, rem);
                sector += chunk;
                rem -= copied;
                got += copied;
            } while (sectors > 0);
        }
        if (rem == 0) return got;
    }
    if (quad(file, 176) != 0) writeBack(file);
    BufferClear__6VIFile(file);
    long long position = quad(file, 16);
    long long size = word(file, 76);
    if (position != size) {
        long long left = size - position;
        long long sectorsLeft = (left < 0 ? left + 2047 : left) >> 11;
        int sector = sectorOf(position);
        int sectors;
        if (sectorsLeft < 17) {
            sectors = static_cast<int>(sectorsLeft >= 1 ? sectorsLeft : 1);
        } else {
            sectors = 16;
        }
        long long start = sector << 11;
        quad(file, 136) = start;
        quad(file, 160) = position - start;
        if (CDSync__6VIFilei(file, 0) != 0) return -1;
        if (CDRead__6VIFileUiUiPvP10sceCdRMode(file, sector + word(file, 72), sectors, word(file, 188), file + 108) == 0) return -1;
        if (CDSync__6VIFilei(file, 0) != 0) return -1;
        long long bytes = sectors << 11;
        long long room = static_cast<long long>(word(file, 76)) - quad(file, 16);
        if (room < bytes) {
            quad(file, 176) = room;
            word(file, 168) = 1;
        } else {
            quad(file, 176) = bytes;
            word(file, 168) = 0;
        }
        int copied = BufferedReadCopy__6VIFilePvi(file, buffer + got, rem);
        got += copied;
        if (rem == copied) return got;
        if (word(file, 168) == 0) return got;
    }
    *reinterpret_cast<int*>(file + 8) = 1;
    return got;
}
}
