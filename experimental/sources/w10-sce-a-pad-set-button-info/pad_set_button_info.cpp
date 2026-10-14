/* Observed scePadSetButtonInfo at 0x002d43e8, 176 bytes (a file-local routine
 * of the pad library). It fills the shared 128-byte RPC buffer (0x008ddac0):
 * word 0 = 10, word 1 = port, word 2 = slot, word 3 = the third argument. Then
 * sceSifCallRpc runs on the pad client table (0x008dd880) with function 1,
 * mode 0, that buffer as both the 128-byte send and receive area and no end
 * function or argument. A negative call result returns 0. Otherwise the reply
 * word 4 of the buffer is the result; when it is exactly 1, scePadSetReqState(
 * port, slot, 2) runs first and the reply word is read again afterwards.
 */
extern "C" {
extern unsigned char norrath_libpad_rpc_clients_view[];
extern int norrath_libpad_rpc_buffer_view[];

int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* end, void* end_param);
int scePadSetReqState(int port, int slot, int state);

int scePadSetButtonInfo(int port, int slot, unsigned mask) {
    int* const buffer = norrath_libpad_rpc_buffer_view;
    buffer[0] = 10;
    buffer[1] = port;
    buffer[2] = slot;
    buffer[3] = static_cast<int>(mask);
    if (sceSifCallRpc(norrath_libpad_rpc_clients_view, 1, 0, buffer, 128, buffer, 128, nullptr, nullptr) < 0)
        return 0;
    if (buffer[4] != 1) return buffer[4];
    scePadSetReqState(port, slot, 2);
    return buffer[4];
}
}
