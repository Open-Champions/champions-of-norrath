// Observed: three local objects of the network start-up code in the pinned SLUS_205.65 symbol
// table. net_ready is a 4-byte word at 0x0118e6b8 and unet_startup_thread_id a 4-byte word at
// 0x0118e6bc, both right after sLibNetInitialized and before hostname (the symbol table
// gives their 4-byte sizes). unet_startup_thread_stack starts at 0x011e31d8; the next
// symbol, sFreeTrailFx, starts 0x2010 (8208) bytes later. unet_startup_async rounds the stack
// start up to 16 and hands the thread 8192 bytes, which fits in the 8208. All three read as zero
// in the image.
// Inferred: net_ready is set to 1 by unet_startup_async_status once the thread has finished, so
// zero means "not finished"; the stack is the whole region the thread's stack is carved from.
// Unknown: whether the region is exactly 8208 bytes (it is the distance to the next symbol, an
// upper bound), and the writers of net_ready other than the status function. Relative placement
// of the three objects is not reproduced.
extern "C" {
int net_ready = 0;
int unet_startup_thread_id = 0;
alignas(16) unsigned char unet_startup_thread_stack[8208];
}
