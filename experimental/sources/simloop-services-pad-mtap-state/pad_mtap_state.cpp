/* Library-private objects of the pad and multitap RPC wrappers. Original
 * symbols are file-local in the executable, so the names here are views at
 * the observed addresses.
 *
 * .data: isInit at 0x004448f8 starts 0; isWarning at 0x004448fc starts 1.
 * .bss, spans taken from the next symbol:
 *   padsif 0x008dd880, 80 bytes: two 40-byte RPC client records (the server
 *     pointer sits at +36 of each).
 *   PadInfo 0x008dd8d0, 240 bytes: 28 bytes per slot, 112 per port.
 *   dma_iop_data 0x008dd9c0, 256 bytes: 32 bytes per slot, 128 per port.
 *   buffer 0x008ddac0, 128 bytes: the pad RPC send/receive area.
 *   port_open, port_close, get_connection, change_priority, module_version
 *     at 0x008ddb40 + 40*n: five 40-byte multitap RPC clients.
 *   sif_buffer 0x008ddc40, 128 bytes: the multitap RPC send/receive area.
 * The 16 bytes after module_version's 40 bytes up to sif_buffer are not
 * owned here.
 */
extern "C" {
unsigned char norrath_libpad_is_init_word[4] = {0, 0, 0, 0};
unsigned char norrath_libpad_is_warning_word[4] = {1, 0, 0, 0};
alignas(16) unsigned char norrath_libpad_rpc_clients_view[80];
alignas(16) unsigned char norrath_libpad_port_info_view[240];
alignas(64) unsigned char norrath_libpad_dma_info_view[256];
alignas(64) unsigned char norrath_libpad_rpc_buffer_view[128];
alignas(16) unsigned char norrath_libmtap_port_open_client_view[40];
alignas(8) unsigned char norrath_libmtap_port_close_client_view[40];
alignas(8) unsigned char norrath_libmtap_get_connection_client_view[40];
alignas(8) unsigned char norrath_libmtap_change_priority_client_view[40];
alignas(8) unsigned char norrath_libmtap_module_version_client_view[40];
alignas(64) unsigned char norrath_libmtap_rpc_buffer_view[128];
}
