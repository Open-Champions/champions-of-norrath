/* Observed gCd at 0x007dd7e0. st_size is 0. The next .bss symbol is sbuff
 * at 0x007dd840, so this object is 96 bytes. soundSleep and soundRestore
 * pass it to the RPC call. Bind and call use fields through offset 52.
 * Bytes 56 through 95 have no other symbol. The image bytes are zeros.
 * This is not an alias of a SIF function symbol.
 */
extern "C" {
alignas(32) unsigned char norrath_sound_rpc_client_view[96];
}
