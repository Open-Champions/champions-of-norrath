/* Observed sceSdRemoteInit loads the format at 0x0047b9a8 when bind
 * returns negative. The rodata C string is
 * "sceSdRemoteInit() RPC bind error!\n".
 */
extern "C" {
alignas(8) unsigned char norrath_sound_remote_bind_failure_format_view[] = {'s', 'c', 'e', 'S', 'd', 'R', 'e', 'm', 'o', 't', 'e', 'I', 'n', 'i', 't', '(', ')', ' ', 'R', 'P', 'C', ' ', 'b', 'i', 'n', 'd', ' ', 'e', 'r', 'r', 'o', 'r', '!', '\n', 0};
}
