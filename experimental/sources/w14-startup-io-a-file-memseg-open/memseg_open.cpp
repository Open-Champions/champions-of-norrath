// Observed: MemorySegmOpen__6VIFileiPQ26VIFile7MemSegmil at 0x01002350 is 308 bytes and
// returns 0 or -1. Arguments after the object: a count, a pointer to count
// sixteen-byte segment descriptors, a mode, and a long. The mode is stored at +4
// before anything else. A mode with none of the low three bits (7) returns -1. Else
// the count goes to +52, a block of count * 24 bytes comes from __builtin_vec_new
// and its pointer to +56 (a null result returns -1 with the null already stored).
// An 8-byte total at +32 is cleared, then for each descriptor in order the 24-byte
// record in the block gets: 8 bytes = the running total, 8 bytes = descriptor
// bytes 0 to 7, 8 bytes = descriptor bytes 8 to 15; the running total (re-read from
// +32) then grows by the record's second field (descriptor bytes 0 to 7) and is
// stored back to +32 each step. Afterwards, with bit 8 of the mode set the
// eight-byte values at +40 and +16 are both 0. Otherwise +40 receives the long
// (sign-extended to 8 bytes) and +16 gets that long when bit 0x10 is set together
// with at least one of bits 2 and 4, else 0. MemorySegmCalcSegm__6VIFile(this) is
// called, the word at +8 is cleared, and 0 is returned.
// Inferred: the descriptors are (length, something) pairs describing memory
// segments the file is read from; the record's first field is the byte offset of
// the segment within the whole, +32 the total length; +16 and +40 hold the long
// as a start and a limit when the mode asks for a window.
// Unknown: names of the fields, the meaning of mode bits 8 and 0x10, and the
// second descriptor field.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#endif

extern "C" {
void* __builtin_vec_new(unsigned size);
void MemorySegmCalcSegm__6VIFile(void* file);

int MemorySegmOpen__6VIFileiPQ26VIFile7MemSegmil(unsigned char* self, int count, unsigned char* segments, int mode, long extra) {
    typedef unsigned long long __attribute__((may_alias)) Quad;
    typedef long long __attribute__((may_alias)) SQuad;
    *reinterpret_cast<int*>(self + 4) = mode;
    if ((mode & 7) == 0) return -1;
    *reinterpret_cast<int*>(self + 52) = count;
    void* block = __builtin_vec_new(static_cast<unsigned>(count) * 24);
    unsigned address = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(block));
    *reinterpret_cast<unsigned*>(self + 56) = address;
    if (block == 0) return -1;
    *reinterpret_cast<Quad*>(self + 32) = 0;
    for (int index = 0; index < count; ++index) {
#ifdef NORRATH_HOST_ARENA
        unsigned char* record = norrath_host_pointer(*reinterpret_cast<unsigned*>(self + 56)) + 24 * index;
#else
        unsigned char* record = reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(self + 56))) + 24 * index;
#endif
        *reinterpret_cast<Quad*>(record) = *reinterpret_cast<Quad*>(self + 32);
        *reinterpret_cast<Quad*>(record + 8) = *reinterpret_cast<Quad*>(segments + 16 * index);
        *reinterpret_cast<Quad*>(record + 16) = *reinterpret_cast<Quad*>(segments + 16 * index + 8);
        *reinterpret_cast<Quad*>(self + 32) = *reinterpret_cast<Quad*>(self + 32) + *reinterpret_cast<Quad*>(record + 8);
    }
    *reinterpret_cast<SQuad*>(self + 40) = extra;
    if ((mode & 8) != 0) {
        *reinterpret_cast<Quad*>(self + 40) = 0;
        *reinterpret_cast<Quad*>(self + 16) = 0;
    } else if ((mode & 0x10) != 0 && (mode & 6) != 0) {
        *reinterpret_cast<SQuad*>(self + 16) = extra;
    } else {
        *reinterpret_cast<Quad*>(self + 16) = 0;
    }
    MemorySegmCalcSegm__6VIFile(self);
    *reinterpret_cast<int*>(self + 8) = 0;
    return 0;
}
}
