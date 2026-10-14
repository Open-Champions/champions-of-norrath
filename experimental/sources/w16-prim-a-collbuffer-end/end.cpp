// Observed: End__12VICollBuffer at 0x0108b2e0 is 172 bytes. The type word at offset 8 must be 0, 1
// or 3; any other value returns -1 and changes nothing. For an accepted type, with g the 32-byte
// group record at (pointer at offset 68) + (word at offset 44) * 32 and e the 28-byte entry at
// (pointer at offset 72) + (word at offset 52) * 28: it increments the word at g offset 0, adds the
// signed halfword at e offset 2 to the word at g offset 4, increments the entry index at
// offset 52, and returns 0.
// Inferred: this closes one Begin(): Begin zeroes the entry's halfword at offset 2 (a per-entry
// count that other code fills in), and End accumulates it into the group's running total while
// counting the entry. Unknown: what the entry's halfword at offset 0 and the group's words mean.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int End__12VICollBuffer(unsigned char* self) {
    int type = *reinterpret_cast<int*>(self + 8);
    if (type != 0 && type != 1 && type != 3)
        return -1;
    unsigned char* groups = at(*reinterpret_cast<unsigned*>(self + 68));
    unsigned char* entries = at(*reinterpret_cast<unsigned*>(self + 72));
    int* group = reinterpret_cast<int*>(groups + *reinterpret_cast<int*>(self + 44) * 32);
    group[0] = group[0] + 1;
    int* total = reinterpret_cast<int*>(groups + *reinterpret_cast<int*>(self + 44) * 32);
    short count = *reinterpret_cast<short*>(entries + *reinterpret_cast<int*>(self + 52) * 28 + 2);
    total[1] = total[1] + count;
    *reinterpret_cast<int*>(self + 52) = *reinterpret_cast<int*>(self + 52) + 1;
    return 0;
}
}
