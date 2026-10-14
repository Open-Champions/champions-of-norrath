// Observed: MemorySegmRead__6VIFilePvi at 0x01001ea8, 336 bytes. Arguments: the file, a destination buffer and a requested count.
// File words used: +8 (cleared first), 64-bit position P at +16, 64-bit size S at +40, segment count minus one at +52, pointer to
// the segment records at +56, current segment index at +60 and the 64-bit offset inside the current segment at +64. A record is
// 24 bytes: a 64-bit length at +8 and a pointer word at +16 (the word at +0 is not read here).
// The count moved is n = min(S - P, requested) (signed, truncated to 32 bits). While n bytes remain, each pass copies
// c = min(remaining, length(current) - offset) bytes with memcpy from pointer(current) + low word of offset to the destination,
// reduces the remainder by c, advances the destination and the offset by c, and then, if the offset has reached the segment length
// and the current index differs from the word at +52, moves to the next segment (index + 1, offset 0). All file words are re-read
// after each memcpy. Afterwards P becomes P + n; the result is 0 when n >= requested, otherwise the word at +8 is set to 1 and the
// result is -1.
// Inferred: this is the read path for a file backed by several in-memory segments; MemorySegmCalcSegm positions +60 and +64.
// Unknown: the member names and the third doubleword of a record.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
extern "C" {
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);

int MemorySegmRead__6VIFilePvi(unsigned char* file, unsigned char* buffer, int count) {
    *reinterpret_cast<int*>(file + 8) = 0;
    long long remaining = *reinterpret_cast<long long*>(file + 40) - *reinterpret_cast<long long*>(file + 16);
    long long limit = remaining < count ? remaining : count;
    int moved = static_cast<int>(limit);
    if (moved > 0) {
        int left = moved;
        do {
            unsigned char* record = bytes_at(*reinterpret_cast<unsigned*>(file + 56)) + *reinterpret_cast<int*>(file + 60) * 24;
            long long room = *reinterpret_cast<long long*>(record + 8) - *reinterpret_cast<long long*>(file + 64);
            long long part = left < room ? left : room;
            int chunk = static_cast<int>(part);
            left -= chunk;
            memcpy(buffer, bytes_at(*reinterpret_cast<unsigned*>(record + 16) + *reinterpret_cast<int*>(file + 64)), chunk);
            *reinterpret_cast<long long*>(file + 64) = chunk + *reinterpret_cast<long long*>(file + 64);
            int index = *reinterpret_cast<int*>(file + 60);
            record = bytes_at(*reinterpret_cast<unsigned*>(file + 56)) + index * 24;
            buffer += chunk;
            if (*reinterpret_cast<long long*>(file + 64) >= *reinterpret_cast<long long*>(record + 8) &&
                index != *reinterpret_cast<int*>(file + 52)) {
                *reinterpret_cast<long long*>(file + 64) = 0;
                *reinterpret_cast<int*>(file + 60) = index + 1;
            }
        } while (left > 0);
    }
    *reinterpret_cast<long long*>(file + 16) = moved + *reinterpret_cast<long long*>(file + 16);
    if (moved < count) {
        *reinterpret_cast<int*>(file + 8) = 1;
        return -1;
    }
    return 0;
}
}
