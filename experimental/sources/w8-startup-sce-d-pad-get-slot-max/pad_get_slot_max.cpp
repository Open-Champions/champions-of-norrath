/* Observed scePadGetSlotMax at 0x002d46e0, 104 bytes. Stores command 13 in
 * word 0 and the port argument in word 1 of the shared 128-byte padman buffer
 * (0x008ddac0), then calls sceSifCallRpc on the first padsif client
 * (0x008dd880) with command 1, mode 0, that buffer as send and receive area
 * of 128 bytes, no completion function and no argument. A negative result
 * returns 0; otherwise word 3 of the buffer is returned.
 */
extern "C" {
extern unsigned char norrath_libpad_rpc_clients_view[];
extern int norrath_libpad_rpc_buffer_view[];

int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* done, void* done_arg);

int scePadGetSlotMax(int port) {
    norrath_libpad_rpc_buffer_view[1] = port;
    norrath_libpad_rpc_buffer_view[0] = 13;
    if (sceSifCallRpc(norrath_libpad_rpc_clients_view, 1, 0, norrath_libpad_rpc_buffer_view, 128,
                      norrath_libpad_rpc_buffer_view, 128, 0, 0) < 0)
        return 0;
    return norrath_libpad_rpc_buffer_view[3];
}
}
