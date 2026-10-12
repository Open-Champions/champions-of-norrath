// Observed memalign at 0x00301318, 100 bytes. It takes the lock service at
// 0x00302270 with the current _impure_ptr (0x00448480) value, calls _memalign_r
// at 0x00301380 with that pointer re-read, the alignment and the size, then calls
// the unlock service at 0x003022f0 with the pointer read once more. It returns the
// _memalign_r result. Argument order is (alignment, size) at both levels.
// Unknown: _memalign_r (not authored here; it stays an unresolved import under
// its symbol name) and the lock services' behavior beyond their sources.
extern "C" {
extern unsigned char* norrath_factory_impure_pointer_word;
void norrath_player_malloc_lock(unsigned char* reent);
void* _memalign_r(unsigned char* reent, unsigned alignment, unsigned size);
void norrath_player_malloc_unlock(unsigned char* reent);

void* memalign(unsigned alignment, unsigned size) {
    unsigned char* volatile* const impure = &norrath_factory_impure_pointer_word;
    norrath_player_malloc_lock(*impure);
    void* const allocation = _memalign_r(*impure, alignment, size);
    norrath_player_malloc_unlock(*impure);
    return allocation;
}
}
