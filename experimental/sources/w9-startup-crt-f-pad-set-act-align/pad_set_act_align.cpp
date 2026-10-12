/* Observed scePadSetActAlign at 0x002d4258, 216 bytes. Fills the shared
 * 128-byte padman buffer (0x008ddac0): word 0 = 8, word 1 = port, word 2 =
 * slot, and six bytes copied from the third argument to buffer offset 12.
 * sceSifCallRpc then runs on the first padsif client (0x008dd880) with
 * command 1, mode 0, the buffer as send and receive area of 128 bytes, no
 * completion function and no argument. A negative result returns 0. Otherwise
 * reply word 5 is the result; when it is exactly 1,
 * scePadSetReqState(port, slot, 2) is called first and word 5 is read again
 * after that call.
 */
extern "C" {
extern unsigned char norrath_libpad_rpc_clients_view[];
extern int norrath_libpad_rpc_buffer_view[];

int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* done, void* done_arg);
void scePadSetReqState(int port, int slot, int state);

int scePadSetActAlign(int port, int slot, const unsigned char* data) {
    norrath_libpad_rpc_buffer_view[0] = 8;
    norrath_libpad_rpc_buffer_view[1] = port;
    norrath_libpad_rpc_buffer_view[2] = slot;
    unsigned char* destination = reinterpret_cast<unsigned char*>(norrath_libpad_rpc_buffer_view) + 12;
    for (int index = 0; index < 6; ++index) destination[index] = data[index];
    if (sceSifCallRpc(norrath_libpad_rpc_clients_view, 1, 0, norrath_libpad_rpc_buffer_view, 128,
                      norrath_libpad_rpc_buffer_view, 128, 0, 0) < 0)
        return 0;
    if (norrath_libpad_rpc_buffer_view[5] != 1) return norrath_libpad_rpc_buffer_view[5];
    scePadSetReqState(port, slot, 2);
    return norrath_libpad_rpc_buffer_view[5];
}
}
