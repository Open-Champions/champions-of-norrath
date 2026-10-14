// Observed: Vertex__12VICollBufferR10VIVect3S16 at 0x0108b1e8 is 248 bytes. It accepts the type
// word at offset 8 only when it is 1 or 3; any other value returns -1 and changes nothing. With n
// the word at offset 60 and the argument holding three 16-bit values:
//   type 1: copies the three halfwords to (pointer at offset 76) + n * 6;
//   type 3: copies the three halfwords to (pointer at offset 76) + n * 8, then stores the bytes at
//           object offsets 80 and 81 at record offsets 6 and 7.
// Either way it advances the word at offset 60 by one, adds one to the halfword at offset 2 of the
// 28-byte entry at (pointer at offset 72) + (word at offset 52) * 28, and returns 0.
// Inferred: the 16-bit counterpart of Vertex__12VICollBufferR7VIVect3; the two bytes at 80/81 are
// per-vertex tags in the 8-byte layout (offset 80 is set by VertexGroup__12VICollBufferSc).
// Unknown: what the byte at offset 81 is, and the VIVect3S16 fixed-point format.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int Vertex__12VICollBufferR10VIVect3S16(unsigned char* self, const unsigned short* v) {
    int type = *reinterpret_cast<int*>(self + 8);
    if (type != 1 && type != 3)
        return -1;
    int n = *reinterpret_cast<int*>(self + 60);
    unsigned char* base = at(*reinterpret_cast<unsigned*>(self + 76));
    if (type == 1) {
        unsigned short* dst = reinterpret_cast<unsigned short*>(base + n * 6);
        dst[0] = v[0];
        *reinterpret_cast<int*>(self + 60) = n + 1;
        dst[1] = v[1];
        dst[2] = v[2];
    } else {
        unsigned short* dst = reinterpret_cast<unsigned short*>(base + n * 8);
        dst[0] = v[0];
        dst[1] = v[1];
        dst[2] = v[2];
        reinterpret_cast<unsigned char*>(dst)[6] = self[80];
        reinterpret_cast<unsigned char*>(base + *reinterpret_cast<int*>(self + 60) * 8)[7] = self[81];
        *reinterpret_cast<int*>(self + 60) = *reinterpret_cast<int*>(self + 60) + 1;
    }
    unsigned char* entry = at(*reinterpret_cast<unsigned*>(self + 72)) + *reinterpret_cast<int*>(self + 52) * 28;
    *reinterpret_cast<unsigned short*>(entry + 2) = static_cast<unsigned short>(*reinterpret_cast<unsigned short*>(entry + 2) + 1);
    return 0;
}
}
