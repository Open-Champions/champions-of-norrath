// Observed _mallinfo_r at 0x0030b028, 240 bytes. The first argument is the
// caller's 40-byte result block and the second is the reentrancy pointer, which
// is passed on unchanged to the lock and unlock calls. With the lock held it
// runs __malloc_update_mallinfo and copies the 40-byte
// __malloc_current_mallinfo (0x004488b8) into a stack temporary, then unlocks and
// copies the temporary into the result. It returns the result pointer. The
// ten-word layout is the one of the allocator's own record; the field order is
// not reconfirmed here, so the words are copied without interpretation.
extern "C" {
struct norrath_mallinfo_block {
    int word[10];
};

extern norrath_mallinfo_block __malloc_current_mallinfo;

void __malloc_lock(void* reent);
void __malloc_unlock(void* reent);
void __malloc_update_mallinfo();

norrath_mallinfo_block* _mallinfo_r(norrath_mallinfo_block* result, void* reent) {
    norrath_mallinfo_block snapshot;
    __malloc_lock(reent);
    __malloc_update_mallinfo();
    snapshot = __malloc_current_mallinfo;
    __malloc_unlock(reent);
    *result = snapshot;
    return result;
}
}
