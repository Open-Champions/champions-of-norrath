// Observed: MemoryRead__6VIFilePvi at 0x01002218, 156 bytes. Arguments: the file, a destination buffer and a requested byte count.
// The word at +8 is cleared first. With P the 64-bit position at +16 and S the 64-bit size at +40, the count actually moved is
// n = min(S - P, requested) (a signed comparison, truncated to 32 bits). When n > 0 it copies n bytes with memcpy from
// (the pointer word at +48) + (low word of P) to the destination. The position then becomes P + n, with P re-read after the copy.
// The result is 0 when n >= requested; otherwise the word at +8 is set to 1 and the result is -1.
// Inferred: +48 holds the base address of an in-memory image; +8 is an end-of-data flag that a short read raises.
// Unknown: the member names and whether a negative remaining size is ever seen.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* bytes_at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* bytes_at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif
extern "C" {
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);

int MemoryRead__6VIFilePvi(unsigned char* file, void* buffer, int count) {
    *reinterpret_cast<int*>(file + 8) = 0;
    long long remaining = *reinterpret_cast<long long*>(file + 40) - *reinterpret_cast<long long*>(file + 16);
    long long limit = remaining < count ? remaining : count;
    int moved = static_cast<int>(limit);
    if (moved > 0) {
        memcpy(buffer, bytes_at(*reinterpret_cast<unsigned*>(file + 48)) + *reinterpret_cast<int*>(file + 16), moved);
    }
    *reinterpret_cast<long long*>(file + 16) = moved + *reinterpret_cast<long long*>(file + 16);
    if (moved < count) {
        *reinterpret_cast<int*>(file + 8) = 1;
        return -1;
    }
    return 0;
}
}
