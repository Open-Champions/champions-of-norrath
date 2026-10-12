/* Observed _packet_buffer at 0x008dec40. st_size is 0. The next .bss
 * symbol is _free_buffer at 0x008df440, 2048 bytes later. sceSifInitRpc
 * publishes this address and later uses a 64-byte slot at offset 64.
 * The image bytes are zeros.
 */
extern "C" {
alignas(64) unsigned char norrath_sound_rpc_packet_buffer_view[2048];
}
