/* Observed sceSifExitCmd at 0x002d7428, 52 bytes. Calls DisableDmac(5), then
 * RemoveDmacHandler(5, h) with h the word at 0x008dea14 (the DMA handler
 * identifier stored by sceSifInitCmd), then stores zero in the word
 * _cmd_init_check at 0x00444964. The image does not clear the handler word
 * or the command state. Returns nothing.
 */
extern "C" {
extern int norrath_sound_command_dma_handle_view;
extern int norrath_sound_command_initialized_word_view;

void DisableDmac(int channel);
void RemoveDmacHandler(int channel, int identifier);

void sceSifExitCmd() {
    DisableDmac(5);
    RemoveDmacHandler(5, norrath_sound_command_dma_handle_view);
    norrath_sound_command_initialized_word_view = 0;
}
}
