/* Command storage used by sceSifInitCmd at 0x002d71a8. These are separate
 * .bss symbols. Sizes are the spans to the next symbol in that section.
 * sceSifInitCmd writes the state words, 32 handler records of 12 bytes, and
 * 32 register words before any caller reads them. Initial image bytes are
 * loader zeros. The original addresses are 64-byte aligned except the
 * 20-byte packet at 0x008dea00 (256-byte aligned), the handle at
 * 0x008dea14 (4-byte aligned), and the state at 0x008dea18 (8-byte aligned).
 */
extern "C" {
alignas(64) unsigned char norrath_sound_command_packet_buffer_view[128];
alignas(64) unsigned char norrath_sound_command_send_buffer_view[64];
alignas(16) unsigned char norrath_sound_command_initialization_packet_view[20];
alignas(4) unsigned char norrath_sound_command_dma_handle_view[4];
alignas(8) unsigned char norrath_sound_command_state_view[40];
alignas(64) unsigned char norrath_sound_command_system_handlers_view[384];
alignas(64) unsigned char norrath_sound_system_register_words_view[128];
/* _cmd_init_check at 0x00444964 is a .data word whose stored value is 0. */
alignas(4) unsigned char norrath_sound_command_initialized_word_view[4] = {0, 0, 0, 0};
}
