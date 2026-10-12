/* Observed sceSd_gCd at 0x008e5a40. st_size is 0. The next .bss symbol
 * is stack at 0x008e5a80, 64 bytes later. sceSdRemoteInit binds this
 * client and polls the word at offset 36. The image bytes are zeros.
 */
extern "C" {
alignas(64) unsigned char norrath_sound_remote_rpc_client_view[64];
}
