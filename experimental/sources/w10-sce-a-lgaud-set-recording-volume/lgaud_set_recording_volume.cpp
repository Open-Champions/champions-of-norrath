/* Observed lgAudSetRecordingVolume at 0x002ead10, 244 bytes. Arguments: a
 * device handle and two volume values of which only the low bytes are kept.
 * The library lock word (0x00447f7c) equal to -1 returns 0x8000000a with
 * nothing else done. Otherwise the lock semaphore is awaited and the shared
 * request buffer (pointer at 0x00447f74) gets the first volume byte at byte
 * 16, the handle in word 3 and the second volume byte at byte 17.
 * sceSifCallRpc runs on the library client (0x008ea780) with function 14,
 * mode 0, the buffer as the 32-byte send area and as the 32-byte receive
 * area, and no end function or argument. A nonzero call result prints
 * "lgAudStopPlayback: rpc call failed\n" (the text of the stop-playback call,
 * as observed), gives the semaphore back and returns 0x80000000. Otherwise
 * buffer word 0 is read, the semaphore is given back and the word is returned.
 * Which volume is left or right is not established here.
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

unsigned lgAudSetRecordingVolume(unsigned handle, unsigned first, unsigned second) {
    if (norrath_lgaud_semaphore_view == -1) return 0x8000000au;
    WaitSema(norrath_lgaud_semaphore_view);
    norrath_lgaud_request_buffer_view[16] = static_cast<unsigned char>(first);
    *reinterpret_cast<unsigned*>(norrath_lgaud_request_buffer_view + 12) = handle;
    norrath_lgaud_request_buffer_view[17] = static_cast<unsigned char>(second);
    unsigned char* const buffer = norrath_lgaud_request_buffer_view;
    if (sceSifCallRpc(norrath_lgaud_rpc_client_view, 14, 0, buffer, 32, buffer, 32, nullptr, nullptr) != 0) {
        printf("lgAudStopPlayback: rpc call failed\n");
        SignalSema(norrath_lgaud_semaphore_view);
        return 0x80000000u;
    }
    const unsigned result = *reinterpret_cast<unsigned*>(norrath_lgaud_request_buffer_view);
    SignalSema(norrath_lgaud_semaphore_view);
    return result;
}
}
