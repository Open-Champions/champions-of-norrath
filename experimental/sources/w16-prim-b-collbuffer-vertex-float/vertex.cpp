// Observed: Vertex__12VICollBufferR7VIVect3 at 0x0108bbb0 is 124 bytes. If the type word at offset
// 8 is not 0 it returns -1 and changes nothing. Otherwise, with n the word at offset 60, it copies
// the three floats of the argument to (pointer at offset 76) + n * 12, stores n + 1 at offset 60,
// adds one to the halfword at offset 2 of the 28-byte entry at (pointer at offset 72) + (word at
// offset 52) * 28, and returns 0. The stores happen in the order x, count, y, entry halfword, z.
// Inferred: the buffer holds a growing vertex array and the entry's halfword counts the vertices
// added since Begin (End__12VICollBuffer accumulates it). Type 0 is the float-vertex layout; the
// halfword-vertex sibling accepts types 1 and 3. Unknown: the meaning of the entry's other fields.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int Vertex__12VICollBufferR7VIVect3(unsigned char* self, const float* v) {
    if (*reinterpret_cast<int*>(self + 8) != 0)
        return -1;
    int n = *reinterpret_cast<int*>(self + 60);
    float* dst = reinterpret_cast<float*>(at(*reinterpret_cast<unsigned*>(self + 76)) + n * 12);
    dst[0] = v[0];
    *reinterpret_cast<int*>(self + 60) = n + 1;
    dst[1] = v[1];
    unsigned char* entry = at(*reinterpret_cast<unsigned*>(self + 72)) + *reinterpret_cast<int*>(self + 52) * 28;
    *reinterpret_cast<unsigned short*>(entry + 2) = static_cast<unsigned short>(*reinterpret_cast<unsigned short*>(entry + 2) + 1);
    dst[2] = v[2];
    return 0;
}
}
