// Observed free at 0x003015a0, 72 bytes. It takes the lock service at 0x00302270
// with the current _impure_ptr (0x00448480) value, calls _free_r at 0x00300608
// with that pointer re-read and the caller's pointer, then tail-calls the unlock
// service at 0x003022f0 with the pointer read once more. Each of the three reads
// is a real load. The result is void. Same shape as malloc at 0x00301548.
// The _free_r callee is the authored norrath_player_free_reentrant.
// Unknown: the lock services' behavior beyond their authored sources.
extern "C" {
extern unsigned char* norrath_factory_impure_pointer_word;
void norrath_player_malloc_lock(unsigned char* reent);
void norrath_player_free_reentrant(unsigned char* reent, unsigned char* allocation);
void norrath_player_malloc_unlock(unsigned char* reent);

void free(void* allocation) {
    unsigned char* volatile* const impure = &norrath_factory_impure_pointer_word;
    norrath_player_malloc_lock(*impure);
    norrath_player_free_reentrant(*impure, static_cast<unsigned char*>(allocation));
    norrath_player_malloc_unlock(*impure);
}
}
