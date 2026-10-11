// Initial player-malloc words beside the bin image.
// Observed file bytes, little endian:
//   __malloc_trim_threshold  0x00448890  8 bytes  0x00020000
//   __malloc_top_pad         0x00448898  8 bytes  0
//   __malloc_sbrk_base       0x004488a0  4 bytes  0xffffffff
//   __malloc_max_sbrked_mem  0x004488a8  8 bytes  0
//   __malloc_max_total_mem   0x004488b0  8 bytes  0
//   __malloc_current_mallinfo 0x004488b8 40 bytes 0
//   __malloc_lock_owner      0x004488e0  4 bytes  0xffffffff
//   call_count               0x004488e4  4 bytes  0
//   __sce_sema_id            0x00444a30  4 bytes  0
// Four unowned zero bytes sit between the break base and the max-break word.
// The semaphore word is also the startup symbol __sce_sema_id; this definition
// uses the name the allocator already calls.
extern "C" {
unsigned char norrath_player_malloc_trim_threshold_view[8] = {0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char norrath_player_malloc_top_pad_view[8] = {};
unsigned char norrath_player_malloc_break_base_view[4] = {0xff, 0xff, 0xff, 0xff};
unsigned char norrath_player_malloc_max_break_view[8] = {};
unsigned char norrath_player_malloc_max_total_view[8] = {};
unsigned char norrath_player_malloc_current_info_view[40] = {};
unsigned char norrath_player_malloc_lock_owner_view[4] = {0xff, 0xff, 0xff, 0xff};
unsigned char norrath_player_malloc_lock_count_view[4] = {};
unsigned char norrath_player_malloc_semaphore_view[4] = {};
}
