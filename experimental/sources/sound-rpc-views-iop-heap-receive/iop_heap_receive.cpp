/* Observed rdata at 0x008e1d00. st_size is 0. The next .bss symbol is
 * sdata at 0x008e1d40, 64 bytes later. sceSifAllocIopHeap and the free-memory
 * query pass this address with receive length 4. The other 60 bytes are
 * the rest of this span. The image bytes are zeros.
 */
extern "C" {
alignas(64) unsigned char norrath_sound_iop_heap_receive_view[64];
}
