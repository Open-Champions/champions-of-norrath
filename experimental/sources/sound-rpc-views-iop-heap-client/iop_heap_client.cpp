/* Observed cd at 0x008e1cc0. st_size is 0. The next .bss symbol is
 * rdata at 0x008e1d00, 64 bytes later. sceSifInitIopHeap and
 * sceSifAllocIopHeap pass this object and read the word at offset 36.
 * The image bytes are zeros.
 */
extern "C" {
alignas(64) unsigned char norrath_sound_iop_heap_client_view[64];
}
