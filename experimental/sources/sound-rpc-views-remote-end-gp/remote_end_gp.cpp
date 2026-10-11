/* Observed sceSdEndGp at 0x00446024. st_size is 4. The .data word is
 * zero. sceSdRemoteInit clears it after a successful bind.
 */
extern "C" {
alignas(4) unsigned char norrath_sound_remote_end_gp_word_view[4] = {0, 0, 0, 0};
}
