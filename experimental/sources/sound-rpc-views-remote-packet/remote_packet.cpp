/* Observed sbuff at 0x008e5a00. st_size is 0. The next .bss symbol is
 * sceSd_gCd at 0x008e5a40, 64 bytes later. sceSdRemote writes the packet
 * words here and passes length 64. This is not the stream sbuff at
 * 0x007dd840. The image bytes are zeros.
 */
extern "C" {
alignas(64) unsigned char norrath_sound_remote_packet_view[64];
}
