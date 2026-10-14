/* Observed scePadInit2 at 0x002d35f0, 156 bytes. The argument is not read.
 * Four times over a 28-byte step, three words at offsets 16, 20 and 24 of
 * the PadInfo block (0x008dd8d0) are cleared, and the same three words 112
 * bytes further on, so eight entries are cleared in all. Then command 16 is
 * stored in word 0 and zero in word 4 of the shared padman buffer
 * (0x008ddac0), and sceSifCallRpc runs on the first padsif client
 * (0x008dd880) with command 1, mode 0, that buffer as send and receive area
 * of 128 bytes, no completion function and no argument. A negative result
 * returns 0; otherwise word 3 of the buffer is returned. The original
 * binding is local; it is defined global here so scePadInit can resolve it.
 */
extern "C" {
extern unsigned char norrath_libpad_rpc_clients_view[];
extern int norrath_libpad_rpc_buffer_view[];
extern int norrath_libpad_port_info_view[];

int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* done, void* done_arg);

int scePadInit2(int mode) {
    (void)mode;
    for (int slot = 0; slot < 4; ++slot) {
        int* entry = norrath_libpad_port_info_view + slot * 7;
        entry[4] = 0;
        entry[5] = 0;
        entry[6] = 0;
        entry[32] = 0;
        entry[33] = 0;
        entry[34] = 0;
    }
    norrath_libpad_rpc_buffer_view[0] = 16;
    norrath_libpad_rpc_buffer_view[4] = 0;
    if (sceSifCallRpc(norrath_libpad_rpc_clients_view, 1, 0, norrath_libpad_rpc_buffer_view, 128,
                      norrath_libpad_rpc_buffer_view, 128, 0, 0) < 0)
        return 0;
    return norrath_libpad_rpc_buffer_view[3];
}
}
