/* Observed sbuff at 0x007dd840. st_size is 0. The next .bss symbol is
 * sbuffin at 0x007dd880, 64 bytes later. soundSleep and soundRestore pass
 * this address with receive length 64. The image bytes are zeros.
 */
extern "C" {
alignas(64) unsigned char norrath_sound_rpc_receive_view[64];
}
