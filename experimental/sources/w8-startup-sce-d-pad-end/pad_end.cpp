/* Observed scePadEnd at 0x002d3690, 128 bytes. Stores command 15 in word 0
 * of the shared padman buffer (0x008ddac0) and calls sceSifCallRpc on the
 * first padsif client (0x008dd880) with command 1, mode 0, that buffer as
 * send and receive area of 128 bytes, no completion function and no argument.
 * A negative result returns 0. Otherwise word 3 is the result; when it equals
 * 1 the isInit word (0x004448f8) is cleared. Word 3 is returned either way.
 */
extern "C" {
extern unsigned char norrath_libpad_rpc_clients_view[];
extern int norrath_libpad_rpc_buffer_view[];
extern int norrath_libpad_is_init_word[];

int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* done, void* done_arg);

int scePadEnd() {
    norrath_libpad_rpc_buffer_view[0] = 15;
    if (sceSifCallRpc(norrath_libpad_rpc_clients_view, 1, 0, norrath_libpad_rpc_buffer_view, 128,
                      norrath_libpad_rpc_buffer_view, 128, 0, 0) < 0)
        return 0;
    int result = norrath_libpad_rpc_buffer_view[3];
    if (result == 1) norrath_libpad_is_init_word[0] = 0;
    return result;
}
}
