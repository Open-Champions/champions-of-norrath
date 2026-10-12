#pragma once
// Heap block header as read by memAlloc, memFree and memRealloc (private image,
// 0x144278..0x144640). 28 bytes precede every payload on the EE:
//   +0  magic    0xABCD on every header these routines write
//   +2  state    0 free, 1 allocated, 2 a header absorbed into its neighbor
//   +4  size     payload bytes
//   +8  name     the allocation name pointer (zero when free)
//   +12 next     following block in address order, circular
//   +16 prev     preceding block
//   +20 tag      the global allocation tag at allocation time
//   +24 caller   the return address of the memAlloc call
struct HeapBlock {
    unsigned short magic;
    unsigned short state;
    int size;
    const char* name;
    HeapBlock* next;
    HeapBlock* prev;
    unsigned tag;
    unsigned caller;
};

#if defined(__mips__)
static_assert(sizeof(HeapBlock) == 28 && sizeof(void*) == 4, "EE header size");
#endif

// Header size in bytes. 28 on the EE; the host checks use their own struct size,
// so every constant below is written in terms of it.
constexpr int kHeaderBytes = static_cast<int>(sizeof(HeapBlock));
constexpr unsigned short kHeaderMagic = 0xABCD;

inline HeapBlock* block_of(void* payload) {
    return reinterpret_cast<HeapBlock*>(static_cast<char*>(payload) - kHeaderBytes);
}
inline char* payload_of(HeapBlock* block) {
    return reinterpret_cast<char*>(block) + kHeaderBytes;
}
