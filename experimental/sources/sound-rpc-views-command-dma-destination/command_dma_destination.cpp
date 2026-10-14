/* Observed command send loads one word at 0x008dea20. That address is
 * byte 8 of the command _data_table at 0x008dea18, already published as
 * the 40-byte command state object. This view is that word alone so the
 * send path can name it. It is .bss, so the bytes are zeros. It is not
 * an alias of the state object or of a SIF function.
 */
extern "C" {
alignas(4) unsigned char norrath_sound_command_dma_destination_word_view[4];
}
