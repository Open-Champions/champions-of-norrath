// Observed __sce_sema_id at 0x00444a30 and __sce_eh_sema_id at 0x00444a34.
// Each is a four-byte object in .data, and the file bytes are zero. They are
// adjacent in that order. This is not norrath_player_malloc_semaphore_view.
extern "C" {
int __sce_sema_id = 0;
int __sce_eh_sema_id = 0;
}
static_assert(sizeof(int) == 4, "EE semaphore ids are words");
