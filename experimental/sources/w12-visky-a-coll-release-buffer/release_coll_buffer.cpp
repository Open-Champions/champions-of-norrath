// Observed: ReleaseCollBuffer__9VICollidei at 0x01097020 is 108 bytes. The collide object (this)
// holds a pool at byte offset 528 (pool = this+528). The pool's word at +8 points to an array of
// 12-byte entries; word 0 of entry [index] is a pointer to a buffer whose count at +4 is a signed
// word. The routine decrements that count and stores it. If the new count is positive it returns 0.
// Otherwise it calls Erase__t6VIPool1ZP12VICollBufferi(pool, index); a negative result is returned
// as -1 and nothing else happens. After a non-negative result, a non-null buffer is destroyed with
// _$_12VICollBuffer(buffer, 3). It then returns 0.
// Inferred: reference-counted collision buffers; the destructor flag value 3 is passed as seen
// (the calling convention of that destructor is not decoded here). The entry word is read before
// Erase runs, and the count at +4 is decremented even when the entry word is zero (address 4). Unknown: member names and what Erase does to the entry.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Erase__t6VIPool1ZP12VICollBufferi(unsigned char* pool, int index);
void _$_12VICollBuffer(unsigned char* buffer, int flags);

int ReleaseCollBuffer__9VICollidei(unsigned char* self, int index) {
    unsigned char* pool = self + 528;
    const unsigned char* entries = NORRATH_WORD_TO_POINTER(*reinterpret_cast<const unsigned*>(pool + 8));
    // The buffer stays a 32-bit word so the later null test is not folded away by the access at +4.
    const unsigned buffer_word = *reinterpret_cast<const unsigned*>(entries + 12 * index);
    int* counter = reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(buffer_word + 4));
    int count = *counter - 1;
    *counter = count;
    if (count > 0) return 0;
    if (Erase__t6VIPool1ZP12VICollBufferi(pool, index) < 0) return -1;
    if (buffer_word != 0) _$_12VICollBuffer(NORRATH_WORD_TO_POINTER(buffer_word), 3);
    return 0;
}
}
