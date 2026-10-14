// Observed: MemorySegmWrite__6VIFilePCvi at 0x01001ff8, 332 bytes. Arguments: the file, a source buffer and a byte count. It is the
// mirror of MemorySegmRead__6VIFilePvi (0x01001ea8) with the copy direction reversed: the 64-bit word at +32 (not +40) bounds the
// count, n = min(L - P, requested) (signed, truncated to 32 bits), and each pass copies c = min(remaining, length(current) - offset)
// bytes from the source buffer to pointer(current) + low word of offset, then advances the source, the offset and (when the offset
// reaches the segment length and the index differs from the word at +52) the segment index, exactly as the read path does.
// Afterwards P becomes P + n and the 64-bit word at +40 is raised to the new position when the new position is not below it.
// The result is 0 when n equals the requested count and -1 otherwise. The word at +8 is cleared first and is not set again.
// File words used: +8, +16 (position), +32, +40, +52, +56 (pointer to 24-byte records: length at +8, pointer word at +16),
// +60 (segment index), +64 (64-bit offset in the segment).
// Inferred: the write path of a segment-backed in-memory file.
// Unknown: the member names.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
extern "C" {
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);

int MemorySegmWrite__6VIFilePCvi(unsigned char* file, const unsigned char* buffer, int count) {
    *reinterpret_cast<int*>(file + 8) = 0;
    long long remaining = *reinterpret_cast<long long*>(file + 32) - *reinterpret_cast<long long*>(file + 16);
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
            memcpy(bytes_at(*reinterpret_cast<unsigned*>(record + 16) + *reinterpret_cast<int*>(file + 64)), buffer, chunk);
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
    long long position = moved + *reinterpret_cast<long long*>(file + 16);
    long long size = *reinterpret_cast<long long*>(file + 40);
    *reinterpret_cast<long long*>(file + 16) = position;
    *reinterpret_cast<long long*>(file + 40) = position < size ? size : position;
    return moved == count ? 0 : -1;
}
}
