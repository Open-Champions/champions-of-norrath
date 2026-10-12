/* Observed _bind at 0x00444a0c. st_size is 4. The .data word is
 * 0xffffffff. sceSifAllocIopHeap returns without allocating when this
 * signed word is negative.
 */
extern "C" {
alignas(4) unsigned char norrath_sound_iop_heap_bind_word_view[4] = {0xff, 0xff, 0xff, 0xff};
}
