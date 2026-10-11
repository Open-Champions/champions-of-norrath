/* Observed _sceSifInitCheck at 0x00444968. st_size is 4. The .data word
 * is zero. The command init word at 0x00444964 is a different object.
 */
extern "C" {
alignas(4) unsigned char norrath_sound_rpc_initialized_word_view[4] = {0, 0, 0, 0};
}
