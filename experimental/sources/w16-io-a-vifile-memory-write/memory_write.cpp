// Observed: MemoryWrite__6VIFilePCvi at 0x010022b8, 152 bytes. Arguments: the file, a source buffer and a byte count. The word at
// +8 is cleared first. With P the 64-bit position at +16 and L the 64-bit limit at +32, the count actually moved is
// n = min(L - P, requested) (signed comparison, truncated to 32 bits). When n > 0 it copies n bytes with memcpy from the source to
// (the pointer word at +48) + (low word of P). The position becomes P + n (P re-read after the copy), and the 64-bit word at +40
// is raised to the new position when the new position is not below it. The result is 0 when n equals the requested count and -1
// otherwise.
// Inferred: +32 is the capacity of the in-memory image, +40 the high-water size that reads (MemoryRead) clamp to.
// Unknown: the member names.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
extern "C" {
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);

int MemoryWrite__6VIFilePCvi(unsigned char* file, const void* buffer, int count) {
    *reinterpret_cast<int*>(file + 8) = 0;
    long long remaining = *reinterpret_cast<long long*>(file + 32) - *reinterpret_cast<long long*>(file + 16);
    long long limit = remaining < count ? remaining : count;
    int moved = static_cast<int>(limit);
    if (moved > 0) {
        memcpy(bytes_at(*reinterpret_cast<unsigned*>(file + 48)) + *reinterpret_cast<int*>(file + 16), buffer, moved);
    }
    long long position = moved + *reinterpret_cast<long long*>(file + 16);
    long long size = *reinterpret_cast<long long*>(file + 40);
    *reinterpret_cast<long long*>(file + 16) = position;
    *reinterpret_cast<long long*>(file + 40) = position < size ? size : position;
    return moved == count ? 0 : -1;
}
}
