// Observed __malloc_unlock at 0x003022f0, 76 bytes; the reentrancy argument is
// ignored. It decrements the nesting word at 0x004488e4 and stores it. When the
// result is zero it writes -1 to the owner word at 0x004488e0 and tail calls
// SignalSema (0x002d5100) with the word at 0x00444a30 (__sce_sema_id). When the
// result is not zero it returns at once. No ownership check is made, so an
// unlock from a thread that does not hold the lock still decrements.
// Unknown: behavior if the count underflows; the original has no guard.
extern "C" {
int SignalSema(int semaphore);
extern int __sce_sema_id;
extern int norrath_player_malloc_lock_owner_view;
extern int norrath_player_malloc_lock_count_view;

void __malloc_unlock(void* reent) {
    (void)reent;
    const int remaining = norrath_player_malloc_lock_count_view - 1;
    norrath_player_malloc_lock_count_view = remaining;
    if (remaining == 0) {
        norrath_player_malloc_lock_owner_view = -1;
        SignalSema(__sce_sema_id);
    }
}
}
