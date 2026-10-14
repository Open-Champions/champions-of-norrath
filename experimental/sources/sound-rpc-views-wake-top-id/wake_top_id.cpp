/* Observed topId at 0x00444958. st_size is 4. The .data word is zero.
 * iWakeupThread returns without queueing a wake when this word is zero.
 */
extern "C" {
alignas(4) unsigned char norrath_sound_wake_top_id_word_view[4] = {0, 0, 0, 0};
}
