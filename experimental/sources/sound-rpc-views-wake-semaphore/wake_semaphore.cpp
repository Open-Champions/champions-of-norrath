/* Observed topSema at 0x008de0c0. st_size is 0. iWakeupThread loads one
 * word from this address and passes it to the semaphore signal. The next
 * symbol, topArg, starts 8 bytes later. The word at 0x008de0c4 is not
 * referenced and is not included. The image byte of this word is zero.
 */
extern "C" {
alignas(4) unsigned char norrath_sound_wake_semaphore_word_view[4];
}
