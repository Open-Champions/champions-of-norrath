/* Observed sdata at 0x008e1d40. st_size is 0. The next .bss symbol is
 * _lih_data at 0x008e1d80, 64 bytes later. sceSifAllocIopHeap stores the
 * requested size in the first word and passes send length 4. The other
 * 60 bytes are the rest of this span. The image bytes are zeros.
 */
extern "C" {
alignas(64) unsigned char norrath_sound_iop_heap_send_view[64];
}
