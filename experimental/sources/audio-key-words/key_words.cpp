/* Observed kons at 0x00486bc0 and koffs at 0x00486bc8. Both are .sbss.
 * soundStopSound addresses them as _gp-3632 and _gp-3624. _gp is 0x004879f0.
 * Each span to the next symbol is 8 bytes, two words. The voice-map bank
 * byte selects the word. Loader zeros.
 */
extern "C" {
alignas(8) unsigned char norrath_sound_key_on_words_view[8];
alignas(8) unsigned char norrath_sound_key_off_words_view[8];
}
