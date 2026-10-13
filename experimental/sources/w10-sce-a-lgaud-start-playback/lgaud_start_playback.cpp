/* Observed lgAudStartPlayback at 0x002ea9c0, 196 bytes. Argument: a device handle. The library lock word
 * (0x00447f7c) equal to -1 returns 0x8000000a with nothing else done.
 * Otherwise the lock semaphore is awaited, the handle is stored in word 3 of
 * the shared request buffer (pointer at 0x00447f74) and sceSifCallRpc runs on
 * the library client (0x008ea780) with function 6, mode 0, the buffer as the
 * 32-byte send area and as the 32-byte receive area, and no end function or
 * argument. A nonzero call result prints "lgAudStartPlayback: rpc call failed\n",
 * gives the semaphore back and returns 0x80000000. Otherwise buffer word 0
 * is read, the semaphore is given back and the word is returned.
 */
extern "C" {
extern int norrath_lgaud_semaphore_view;
extern unsigned char* norrath_lgaud_request_buffer_view;
extern unsigned char norrath_lgaud_rpc_client_view[];

int printf(const char* format, ...);
int WaitSema(int semaphore);
int SignalSema(int semaphore);
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* end, void* end_param);

unsigned lgAudStartPlayback(unsigned handle) {
    if (norrath_lgaud_semaphore_view == -1) return 0x8000000au;
    WaitSema(norrath_lgaud_semaphore_view);
    unsigned char* buffer = norrath_lgaud_request_buffer_view;
    *reinterpret_cast<unsigned*>(buffer + 12) = handle;
    if (sceSifCallRpc(norrath_lgaud_rpc_client_view, 6, 0, buffer, 32, buffer, 32, nullptr, nullptr) != 0) {
        printf("lgAudStartPlayback: rpc call failed\n");
        SignalSema(norrath_lgaud_semaphore_view);
        return 0x80000000u;
    }
    const unsigned result = *reinterpret_cast<unsigned*>(norrath_lgaud_request_buffer_view);
    SignalSema(norrath_lgaud_semaphore_view);
    return result;
}
}
