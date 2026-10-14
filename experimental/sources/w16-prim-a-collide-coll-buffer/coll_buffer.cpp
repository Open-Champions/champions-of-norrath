// Observed: CollBuffer__9VICollidei at 0x01096fd0 is 28 bytes. The collision object holds a
// pointer at offset 536 to an array of 12-byte entries; the function returns the first word
// of entry `index` (no bounds check).
// Inferred: that first word is the VICollBuffer pointer for a slot and the entry holds two more
// words. Unknown: the other two words and the number of entries.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
unsigned CollBuffer__9VICollidei(unsigned char* self, int index) {
    unsigned char* entries = at(*reinterpret_cast<unsigned*>(self + 536));
    return *reinterpret_cast<unsigned*>(entries + index * 12);
}
}
