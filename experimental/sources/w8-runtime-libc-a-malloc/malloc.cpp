// Observed malloc at 0x00301548, 80 bytes. It takes the lock service at
// 0x00302270 with the current _impure_ptr (0x00448480) value, calls _malloc_r
// at 0x00301840 with that pointer re-read and the size, then calls the unlock
// service at 0x003022f0 with the pointer read once more, and returns the
// _malloc_r result. Each of the three pointer reads is a real load, so a
// service that replaces _impure_ptr is seen by the next call. The callees are
// the authored norrath_player_malloc_lock, norrath_player_malloc_reentrant and
// norrath_player_malloc_unlock. Same shape as the plain free at 0x003015a0.
// Unknown: the lock services' behavior beyond their authored sources.
extern "C" {
extern unsigned char* norrath_factory_impure_pointer_word;
void norrath_player_malloc_lock(unsigned char* reent);
unsigned char* norrath_player_malloc_reentrant(unsigned char* reent, unsigned size);
void norrath_player_malloc_unlock(unsigned char* reent);

void* malloc(unsigned size) {
    unsigned char* volatile* const impure = &norrath_factory_impure_pointer_word;
    norrath_player_malloc_lock(*impure);
    unsigned char* const allocation = norrath_player_malloc_reentrant(*impure, size);
    norrath_player_malloc_unlock(*impure);
    return allocation;
}
}
