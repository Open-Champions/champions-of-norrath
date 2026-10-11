/* Observed _free_buffer2 at 0x008dfc40. st_size is 0. The next .bss
 * symbol is the RPC _data_table at 0x008e0440, 2048 bytes later.
 * sceSifInitRpc publishes this address. The image bytes are zeros.
 */
extern "C" {
alignas(64) unsigned char norrath_sound_rpc_free_buffer2_view[2048];
}
