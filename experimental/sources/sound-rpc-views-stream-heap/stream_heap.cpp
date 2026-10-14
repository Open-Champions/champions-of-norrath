/* Observed streamAllocation at 0x007dd280. st_size is 0. The next .bss
 * symbol is stream at 0x007dd290, 16 bytes later. soundSleep visits the
 * first three words. The fourth word is inside the span and is not given
 * another symbol. The image bytes are zeros.
 */
extern "C" {
alignas(16) unsigned char norrath_sound_stream_heap_words_view[16];
}
