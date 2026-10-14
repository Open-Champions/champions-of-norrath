// Observed lgAudClose at 0x002ea380, 196 bytes, from the audio library the image
// links in. Argument: a handle. The semaphore word (0x00447f7c) and the aligned
// block pointer (0x00447f74) are the ones lgAudInit publishes; the client is the
// 64-byte object at 0x008ea780.
//   - A semaphore word of -1 (init not done) returns 0x8000000a at once.
//   - WaitSema on the semaphore; the handle goes to word 3 (offset 12) of the
//     block; sceSifCallRpc runs on the client with operation 3, mode 0, the block
//     as both send and receive areas of 32 bytes each, and no completion.
//   - A nonzero call result prints "lgAudClose: rpc call failed\n", signals the
//     semaphore and returns 0x80000000.
//   - Otherwise word 0 of the block is read first, the semaphore is signalled,
//     and that word is returned.
// The semaphore word is re-read for each use; the block pointer is read once
// after the wait.
// Unknown: what the returned word means beyond a status, and the meaning of the
// operation number.
extern "C" {
int printf(const char* format, ...);
int WaitSema(int semaphore);
int SignalSema(int semaphore);
int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                  void* receive, int receive_size, void* completion,
                  void* completion_argument);

extern unsigned char norrath_lgaud_client[64];
extern int norrath_lgaud_semaphore;
extern unsigned char* norrath_lgaud_aligned_buffer;

int lgAudClose(int handle) {
    if (norrath_lgaud_semaphore == -1) return static_cast<int>(0x8000000au);
    WaitSema(norrath_lgaud_semaphore);
    unsigned char* const block = norrath_lgaud_aligned_buffer;
    *reinterpret_cast<int*>(block + 12) = handle;
    if (sceSifCallRpc(norrath_lgaud_client, 3, 0, block, 32, block, 32, 0, 0) != 0) {
        printf("lgAudClose: rpc call failed\n");
        SignalSema(norrath_lgaud_semaphore);
        return static_cast<int>(0x80000000u);
    }
    const int status = *reinterpret_cast<int*>(block);
    SignalSema(norrath_lgaud_semaphore);
    return status;
}
}
