// Observed: Next__Ct6VIPool1ZP12VICollBufferi at 0x01207330, 44 bytes. With (pool, index) it reads the
// word at pool+8, indexes it in 12-byte steps, loads the word at +4 of that element and returns it. The value -1 is returned as 0x7fffffff (the constant is inline; no call is made).
// Inferred: element +4 is the next index of a chain, -1 ends it, and 0x7fffffff is the iterator end marker.
// Unknown: the other fields of the 12-byte element.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Next__Ct6VIPool1ZP12VICollBufferi(const void* pool, int index) {
    const unsigned char* elements = NORRATH_WORD_TO_POINTER(*(static_cast<const unsigned*>(pool) + 2));
    int next = *reinterpret_cast<const int*>(elements + index * 12 + 4);
    return next == -1 ? 0x7fffffff : next;
}
}
