/* batchIopBuffer at 0x00485950 is a 4-byte .sdata word whose stored value
 * is 0. IOPbuffer at 0x00486bd8 is .sbss. The next symbol begins 8 bytes
 * later; only the first word is the buffer pointer initialized by
 * norrath_sound_initialize. The following 4 bytes are not included.
 */
extern "C" {
alignas(4) unsigned char norrath_sound_batch_iop_buffer_word_view[4] = {0, 0, 0, 0};
alignas(8) unsigned char norrath_sound_iop_buffer_word_view[4];
}
