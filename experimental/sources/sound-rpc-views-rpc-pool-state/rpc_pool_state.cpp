/* Observed the RPC _data_table at 0x008e0440. st_size is 0. The next
 * .bss symbol is rcv_adr.39 at 0x008e0480, 64 bytes later. This is not
 * the command _data_table at 0x008dea18. sceSifInitRpc writes words
 * through offset 36. Bytes 40 through 63 are the rest of this span.
 * The image bytes are zeros.
 */
extern "C" {
alignas(64) unsigned char norrath_sound_rpc_packet_pool_state_view[64];
}
