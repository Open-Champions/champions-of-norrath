// memRealloc(void*, int) at 0x00144278, 240 bytes.
// Observed: the request is raised to 64 with a signed compare, then compared
// with the block's size word. Growth allocates and copies that size word's
// count, reloaded after allocation, and does not free the old block. Slack
// below 129, as a signed view of the unsigned difference, returns the same
// pointer. Larger slack splits an 8-byte-aligned tail, links it as an
// allocated block, and frees that tail. The allocation name passed on growth
// is the empty byte at original 0x00483b10, which has no symbol of its own.
static_assert(sizeof(unsigned) == 4 && sizeof(int) == 4, "32-bit size words");

extern "C" {
void* norrath_mem_alloc(int requested_bytes, char* allocation_name);
void norrath_mem_free(void* allocation);
void* memcpy(void* destination, const void* source, unsigned count);
void* norrath_mem_realloc(void* block, int request);
#if defined(NORRATH_REALLOC_HOST_CHECK)
extern unsigned long long norrath_realloc_host_address_base;
#endif
}

namespace {
int signed_word(unsigned value) {
    return value < 0x80000000u ? static_cast<int>(value) : -1 - static_cast<int>(~value);
}
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
void write_half(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
unsigned low_address(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
unsigned char* from_low(unsigned address) {
#if defined(NORRATH_REALLOC_HOST_CHECK)
    return reinterpret_cast<unsigned char*>(norrath_realloc_host_address_base + address);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}
char allocation_name[] = {0};
}

extern "C" void* norrath_mem_realloc(void* block, int request) {
    unsigned char* payload = static_cast<unsigned char*>(block);
    unsigned char* header = payload - 28;
    const int needed = request > 64 ? request : 64;
    const unsigned old_size = read_word(header + 4);
    if (signed_word(old_size) < needed) {
        void* fresh = norrath_mem_alloc(needed, allocation_name);
        // The copy length is the size word after allocation. The returned
        // pointer is the allocation result, not memcpy's result register.
        const unsigned bytes = read_word(header + 4);
        memcpy(fresh, block, bytes);
        return fresh;
    }
    const unsigned slack = old_size - static_cast<unsigned>(needed);
    if (signed_word(slack) < 129) return block;

    const unsigned aligned = (static_cast<unsigned>(needed) + 7u) & ~7u;
    unsigned char* fresh_header = header + aligned + 28u;
    const unsigned successor = read_word(header + 12);
    write_word(fresh_header + 12, successor);
    write_word(fresh_header + 16, low_address(header));
    write_word(fresh_header + 4, (old_size - aligned) - 28u);
    unsigned char* next = from_low(successor);
    write_half(fresh_header, 0xabcdu);
    write_word(next + 16, low_address(fresh_header));
    write_half(fresh_header + 2, 1u);
    write_word(header + 12, low_address(fresh_header));
    write_word(header + 4, aligned);
    write_word(fresh_header + 8, 0u);
    norrath_mem_free(fresh_header + 28);
    return block;
}
