// Observed engineSyncVBlank at 0x00191180. GP 0x004879f0.
// Two loads of vblankCount at 0x00484438. When they differ, skip the wait.
// Otherwise call WaitSema on the word at vblankSemaphore2 (0x00486a60) until
// vblankCount changes. Return vblankField at 0x0048443c, sign-extended.
extern "C" {
extern volatile unsigned norrath_archive_vblank_count_view;
extern volatile unsigned char norrath_archive_vblank_field_view;
extern int norrath_archive_vblank_semaphore_view;
int norrath_player_wait_semaphore(int semaphore);
int norrath_archive_engine_sync_vblank() {
    unsigned seen = norrath_archive_vblank_count_view;
    if (norrath_archive_vblank_count_view == seen) {
        do {
            (void)norrath_player_wait_semaphore(norrath_archive_vblank_semaphore_view);
        } while (norrath_archive_vblank_count_view == seen);
    }
    return static_cast<int>(static_cast<signed char>(norrath_archive_vblank_field_view));
}
}
// Same body as engineSyncVBlank__Fv at 0x00191180.
#if defined(__APPLE__)
asm(".globl _engineSyncVBlank__Fv\n.set _engineSyncVBlank__Fv, _norrath_archive_engine_sync_vblank\n");
#else
asm(".globl engineSyncVBlank__Fv\n.set engineSyncVBlank__Fv, norrath_archive_engine_sync_vblank\n");
#endif
