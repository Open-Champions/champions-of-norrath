// Observed: RefMap__7VIScenei at 0x01144fa8 is 28 bytes. It reads the word at +1052 of
// the scene (a table address), indexes it with a 12-byte stride by the second
// argument, and returns the first word of that entry. No range check, no writes.
// Inferred: a table of 12-byte reference-map slots whose first word is the map pointer
// (as in the sibling release/share functions). Unknown: the other two words per slot.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
unsigned RefMap__7VIScenei(unsigned char* scene, int index) {
    unsigned char* table = at(*reinterpret_cast<unsigned*>(scene + 1052));
    return *reinterpret_cast<unsigned*>(table + index * 12);
}
}
