// Observed: Begin__12VICollBufferi at 0x0108bb38 is 112 bytes. The buffer type word at offset 8
// must be 0, 1 or 3; any other value returns -1 and changes nothing. For an accepted type it
// takes the current entry index (word at offset 52), addresses the 28-byte entry at
// (pointer at offset 72) + index * 28, stores the low 16 bits of the argument at entry offset 0 and
// zero at entry offset 2 (both halfwords), and returns 0.
// Inferred: type 2 is rewritten to 3 by Init, so only 0/1/3 occur after initialisation; the
// entries are per-begin records (the argument is some id or flag). Unknown: what the argument
// means and what the other 24 bytes of an entry hold.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int Begin__12VICollBufferi(unsigned char* self, int value) {
    int type = *reinterpret_cast<int*>(self + 8);
    if (type != 0 && type != 1 && type != 3)
        return -1;
    int index = *reinterpret_cast<int*>(self + 52);
    unsigned char* entry = at(*reinterpret_cast<unsigned*>(self + 72)) + index * 28;
    *reinterpret_cast<short*>(entry) = static_cast<short>(value);
    *reinterpret_cast<short*>(entry + 2) = 0;
    return 0;
}
}
