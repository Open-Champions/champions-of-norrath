// Observed: Begin__12VIPrimBufferi at 0x01101390 is 288 bytes. The buffer type word at offset 8
// selects through a nine-entry jump table: types 0, 2, 4, 5 and 8 all reach one body; types 1, 3,
// 6, 7 and any value above 8 return -1 and change nothing. The integer argument is never read.
// For an accepted type, with n the entry index at offset 72, c the cursor at offset 80 and R the
// array of 16-byte records at (pointer at offset 96):
//   * the word at ((pointer at offset 92) + n * 28) is set to 0;
//   * the static word _12VIPrimBuffer$PrimVQStart (0x01228a34) is set to c;
//   * R[c] has all four words zeroed and the cursor becomes c + 1;
//   * R[c + 1] has all four words zeroed (its first word after the cursor moved) and the cursor
//     becomes c + 2.
// It returns 0. Words are re-read between the stores, in the same order as the original.
// Inferred: the two zeroed 16-byte records are reserved room for a header (a DMA/GIF-style tag
// pair that End fills in) and PrimVQStart remembers where it begins. Unknown: the real names, why
// only five of the nine types are accepted, and what the 28-byte record's first word counts.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern int PrimVQStart __asm__("_12VIPrimBuffer$PrimVQStart");

extern "C" {
int Begin__12VIPrimBufferi(unsigned char* self, int) {
    unsigned type = *reinterpret_cast<unsigned*>(self + 8);
    if (type > 8 || !((1u << type) & 0x135u))
        return -1;
    int* counts = reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 92)) + *reinterpret_cast<int*>(self + 72) * 28);
    counts[0] = 0;
    int cursor = *reinterpret_cast<int*>(self + 80);
    PrimVQStart = cursor;
    unsigned char* records = at(*reinterpret_cast<unsigned*>(self + 96));
    int* record = reinterpret_cast<int*>(records + cursor * 16);
    record[0] = 0;
    record = reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 96)) + *reinterpret_cast<int*>(self + 80) * 16);
    record[1] = 0;
    record = reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 96)) + *reinterpret_cast<int*>(self + 80) * 16);
    record[2] = 0;
    record = reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 96)) + *reinterpret_cast<int*>(self + 80) * 16);
    record[3] = 0;
    cursor = *reinterpret_cast<int*>(self + 80) + 1;
    *reinterpret_cast<int*>(self + 80) = cursor;
    record = reinterpret_cast<int*>(records + cursor * 16);
    record[0] = 0;
    record = reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 96)) + *reinterpret_cast<int*>(self + 80) * 16);
    record[1] = 0;
    record = reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 96)) + *reinterpret_cast<int*>(self + 80) * 16);
    record[2] = 0;
    record = reinterpret_cast<int*>(at(*reinterpret_cast<unsigned*>(self + 96)) + *reinterpret_cast<int*>(self + 80) * 16);
    record[3] = 0;
    *reinterpret_cast<int*>(self + 80) = *reinterpret_cast<int*>(self + 80) + 1;
    return 0;
}
}
