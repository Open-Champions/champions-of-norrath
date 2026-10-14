// Observed: SetDMAHeaders__9VISurface at 0x0112c500 is 332 bytes and returns 0. It
// writes a 64-byte block (four quadwords) through a pointer built as
// (word at +312) + (stored pointer); the stored pointer is then replaced by the sum.
//   block word 0..2      = 0
//   block word 3         = (size - 1) | 0x50000000
//   quadword 1           = {0x1000000000000001, 14}
//   quadword 2           = {0, 0x53}
//   quadword 3           = {((size - 4) | 0x8000) | (0x8000 << 44), 0}
// where size is a count word next to the stored pointer. It runs once for the pointer
// at +56 (size at +60) when the word at +40 is non-zero, then for each of the first
// "+64" records of a 24-byte array (pointer at +84 + 24 * i, size at +88 + 24 * i).
// The +64 count is re-read every iteration.
// Inferred: the blocks look like PS2 DMA tag plus GIF tag setup for texture upload
// (inferred from the constants only); +312 is a base address added to offsets.
// Unknown: the object's other fields and the precise meaning of the records.
typedef unsigned long long u64;

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

static void write_header(unsigned char* block, int size) {
    unsigned* w = reinterpret_cast<unsigned*>(block);
    w[0] = 0;
    w[1] = 0;
    w[2] = 0;
    w[3] = static_cast<unsigned>(size - 1) | 0x50000000u;
    u64* q = reinterpret_cast<u64*>(block + 16);
    q[0] = 0x1000000000000001ull;
    q[1] = 14;
    q[2] = 0;
    q[3] = 0x53;
    q[4] = static_cast<u64>(static_cast<long long>(static_cast<int>((size - 4) | 0x8000))) | (0x8000ull << 44);
    q[5] = 0;
}

extern "C" {
int SetDMAHeaders__9VISurface(unsigned char* self) {
    if (*reinterpret_cast<int*>(self + 40) != 0) {
        unsigned address = *reinterpret_cast<unsigned*>(self + 312) + *reinterpret_cast<unsigned*>(self + 56);
        unsigned char* block = at(address);
        *reinterpret_cast<unsigned*>(self + 56) = address;
        write_header(block, *reinterpret_cast<int*>(self + 60));
    }
    for (int i = 0; i < *reinterpret_cast<int*>(self + 64); ++i) {
        unsigned char* record = self + 84 + i * 24;
        unsigned address = *reinterpret_cast<unsigned*>(self + 312) + *reinterpret_cast<unsigned*>(record);
        unsigned char* block = at(address);
        *reinterpret_cast<unsigned*>(record) = address;
        write_header(block, *reinterpret_cast<int*>(record + 4));
    }
    return 0;
}
}
