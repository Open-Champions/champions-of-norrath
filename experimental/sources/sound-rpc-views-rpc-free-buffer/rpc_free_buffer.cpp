/* Observed _free_buffer at 0x008df440. st_size is 0. The next .bss
 * symbol is _free_buffer2 at 0x008dfc40, 2048 bytes later. sceSifInitRpc
 * publishes this address. The image bytes are zeros.
 */
extern "C" {
alignas(64) unsigned char norrath_sound_rpc_free_buffer_view[2048];
}
