/* Observed sbuffin at 0x007dd880. st_size is 0. The next .bss symbol is
 * ambientGroup at 0x007dd8c0, 64 bytes later. soundRestore writes the first
 * 12 bytes before a send. The object span is the full 64 bytes. The image
 * bytes are zeros.
 */
extern "C" {
alignas(64) unsigned char norrath_sound_rpc_send_view[64];
}
