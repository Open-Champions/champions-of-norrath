// Observed atexit at 0x002ff8d8, 160 bytes.
// The word at _impure_ptr addresses the reentrancy object. Offset 328 is the
// current 136-byte block. A zero word selects the block at offset 332 and
// stores that address. Each block holds a next address, a signed count, and
// 32 function addresses. A count of 32 or more allocates another block with
// __wrap_malloc(136); failure returns -1 and stores nothing. The new block's
// next word is the previous block. The function is stored at the current
// count, then the count increases by one. Success returns 0. This function
// does not call the registered functions.
extern "C" {
extern unsigned char* _impure_ptr;
void* __wrap_malloc(unsigned size);

struct ExitBlock {
    unsigned next;
    int count;
    unsigned functions[32];
};
static_assert(sizeof(ExitBlock) == 136, "observed atexit block");

#if defined(NORRATH_HOST_CONTRACT)
unsigned norrath_host_pack(__UINTPTR_TYPE__ bits);
__UINTPTR_TYPE__ norrath_host_unpack(unsigned packed);
#else
__attribute__((always_inline)) inline unsigned norrath_host_pack(__UINTPTR_TYPE__ bits) {
    return static_cast<unsigned>(bits);
}
__attribute__((always_inline)) inline __UINTPTR_TYPE__ norrath_host_unpack(unsigned packed) {
    return packed;
}
#endif

__attribute__((always_inline)) inline unsigned load32(const void* address) {
    unsigned value;
    __builtin_memcpy(&value, address, 4);
    return value;
}
__attribute__((always_inline)) inline void store32(void* address, unsigned value) {
    __builtin_memcpy(address, &value, 4);
}

int atexit(void (*function)()) {
    unsigned char* reent = _impure_ptr;
    unsigned char* slot = reent + 328;
    unsigned char* block;
    if (load32(slot) == 0) {
        block = reent + 332;
        store32(slot, norrath_host_pack(reinterpret_cast<__UINTPTR_TYPE__>(block)));
    } else {
        block = reinterpret_cast<unsigned char*>(norrath_host_unpack(load32(slot)));
    }
    int count = static_cast<int>(load32(block + 4));
    if (count >= 32) {
        auto* created = static_cast<unsigned char*>(__wrap_malloc(136));
        if (created == nullptr) return -1;
        store32(created + 4, 0);
        store32(created, load32(slot));
        store32(slot, norrath_host_pack(reinterpret_cast<__UINTPTR_TYPE__>(created)));
        block = created;
        count = 0;
    }
    store32(block + 8 + static_cast<unsigned>(count) * 4,
            norrath_host_pack(reinterpret_cast<__UINTPTR_TYPE__>(function)));
    store32(block + 4, static_cast<unsigned>(count + 1));
    return 0;
}
}
