// Observed: BeginPrimGroup__12VICollBuffer at 0x0108baf0 is 44 bytes. It takes the group index
// at offset 44 of the object, locates the 32-byte record at (pointer at offset 68) + index * 32,
// zeroes the first two words (offsets 0 and 4) of that record, and returns 0.
// Inferred: the array holds per-group records whose first two words are running counts that
// start at zero. Unknown: names and meaning of the remaining 24 bytes of each record.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int BeginPrimGroup__12VICollBuffer(unsigned char* self) {
    unsigned char* groups = at(*reinterpret_cast<unsigned*>(self + 68));
    int index = *reinterpret_cast<int*>(self + 44);
    *reinterpret_cast<int*>(groups + index * 32) = 0;
    index = *reinterpret_cast<int*>(self + 44);
    *reinterpret_cast<int*>(groups + index * 32 + 4) = 0;
    return 0;
}
}
