// Observed __malloc_lock at 0x00302270, 124 bytes; the reentrancy argument is
// ignored. It asks the kernel for the current thread id (GetThreadId, 0x002d4fd0)
// and compares it with the owner word at 0x004488e0. When the caller already
// owns the lock it only increments the nesting word at 0x004488e4. Otherwise it
// waits on the semaphore whose id is the word at 0x00444a30 (__sce_sema_id,
// read after the id call), then stores the thread id as owner and increments the
// nesting word. Nothing is returned.
// Inferred: the two words are an owner and a nesting count, from the way the
// paired __malloc_unlock clears them. Initial values (-1 and 0) are owned by
// the memory-malloc-state views. Unknown: whether the semaphore is ever
// created before the first allocation; no creation site is shown here.
extern "C" {
int GetThreadId();
int WaitSema(int semaphore);
extern int __sce_sema_id;
// Defined as byte views by memory-malloc-state; the same words as the original's
// local owner and nesting variables.
extern int norrath_player_malloc_lock_owner_view;
extern int norrath_player_malloc_lock_count_view;

void __malloc_lock(void* reent) {
    (void)reent;
    const int thread = GetThreadId();
    if (norrath_player_malloc_lock_owner_view != thread) {
        WaitSema(__sce_sema_id);
        norrath_player_malloc_lock_owner_view = thread;
    }
    norrath_player_malloc_lock_count_view = norrath_player_malloc_lock_count_view + 1;
}
}
