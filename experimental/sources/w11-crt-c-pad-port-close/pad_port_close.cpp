/* Observed scePadPortClose at 0x002d38f8, 184 bytes. PadInfo (0x008dd8d0)
 * holds 28 bytes per slot and 112 per port; word +16 is zero when the
 * port/slot is not open, in which case the result is 0 and nothing happens.
 * Otherwise it fills the shared 128-byte padman buffer (0x008ddac0) with
 * command 14 in word 0, the port in word 1, the slot in word 2 and 1 in word
 * 4, then calls sceSifCallRpc on the first padsif client (0x008dd880) with
 * command 1, mode 0, that buffer as send and receive area of 128 bytes, no
 * completion function and no argument. A negative result returns 0 and leaves
 * the open word set. Otherwise the open word is cleared and word 3 of the
 * buffer is returned.
 * Inferred: command 14 is the port close request and word 4 a flag whose
 * meaning is not established here.
 */
extern "C" {
extern unsigned char norrath_libpad_port_info_view[];
extern unsigned char norrath_libpad_rpc_clients_view[];
extern int norrath_libpad_rpc_buffer_view[];

int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* done, void* done_arg);

int scePadPortClose(int port, int slot) {
    unsigned* info = reinterpret_cast<unsigned*>(norrath_libpad_port_info_view + slot * 28 + port * 112);
    if (info[4] == 0) return 0;
    norrath_libpad_rpc_buffer_view[0] = 14;
    norrath_libpad_rpc_buffer_view[1] = port;
    norrath_libpad_rpc_buffer_view[2] = slot;
    norrath_libpad_rpc_buffer_view[4] = 1;
    if (sceSifCallRpc(norrath_libpad_rpc_clients_view, 1, 0, norrath_libpad_rpc_buffer_view, 128,
                      norrath_libpad_rpc_buffer_view, 128, 0, 0) < 0)
        return 0;
    info[4] = 0;
    return norrath_libpad_rpc_buffer_view[3];
}
}
