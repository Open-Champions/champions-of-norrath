// mallinfo at 0x00302340, 48 bytes.
// The caller passes the struct pointer. This loads the word at _impure_ptr
// (0x00448480) and calls _mallinfo_r with that pointer and the loaded word.
// The result is the caller's pointer, not the value returned by _mallinfo_r.
static_assert(sizeof(unsigned) == 4, "32-bit impure pointer");

extern "C" {
extern unsigned char _impure_ptr[];
void* _mallinfo_r(void* result, void* reent);
#if defined(NORRATH_MALLINFO_HOST_CHECK)
extern unsigned long long norrath_mallinfo_host_address_base;
#endif
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void* from_low(unsigned address) {
#if defined(NORRATH_MALLINFO_HOST_CHECK)
    return reinterpret_cast<void*>(norrath_mallinfo_host_address_base + address);
#else
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}
}

extern "C" void* mallinfo(void* result) {
    void* reent = from_low(read_word(_impure_ptr));
    _mallinfo_r(result, reent);
    return result;
}
