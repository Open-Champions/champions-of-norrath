/* Observed lgAudEnumerate at 0x002ea1c0, 444 bytes. Arguments: an index and a
 * pointer for the description. The library lock word (0x00447f7c) equal to -1
 * returns 0x8000000a with nothing else done. A null description pointer
 * prints "lgAudEnumerate: invalid parameter (description)\n" and returns
 * 0x80000004. Otherwise the lock semaphore is awaited, the index is stored in
 * word 2 of the shared request buffer (pointer at 0x00447f74) and
 * sceSifCallRpc runs on the library client (0x008ea780) with function 1,
 * mode 0, the buffer as the 32-byte send area and as the 368-byte receive
 * area, and no end function or argument. A nonzero call result prints
 * "lgAudEnumerate: rpc call failed\n", gives the semaphore back and returns
 * 0x80000000. Otherwise 332 bytes from byte 32 of the buffer are copied to the
 * description (inline, not by a library call), the semaphore is given back and buffer word 0 (read before the
 * copy) is returned. The callers walk indices until the result is 0x80000002.
 * The layout of the description is not established here.
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

typedef unsigned long long Unaligned64 __attribute__((aligned(1), may_alias));
typedef unsigned Unaligned32 __attribute__((aligned(1), may_alias));

unsigned lgAudEnumerate(unsigned index, unsigned char* description) {
    if (norrath_lgaud_semaphore_view == -1) return 0x8000000au;
    if (description == nullptr) {
        printf("lgAudEnumerate: invalid parameter (description)\n");
        return 0x80000004u;
    }
    WaitSema(norrath_lgaud_semaphore_view);
    unsigned char* buffer = norrath_lgaud_request_buffer_view;
    *reinterpret_cast<unsigned*>(buffer + 8) = index;
    if (sceSifCallRpc(norrath_lgaud_rpc_client_view, 1, 0, buffer, 32, buffer, 368, nullptr, nullptr) != 0) {
        printf("lgAudEnumerate: rpc call failed\n");
        SignalSema(norrath_lgaud_semaphore_view);
        return 0x80000000u;
    }
    buffer = norrath_lgaud_request_buffer_view;
    const unsigned result = *reinterpret_cast<unsigned*>(buffer);
    const unsigned char* source = buffer + 32;
    unsigned char* destination = description;
    for (int block = 0; block < 10; ++block) {
        reinterpret_cast<Unaligned64*>(destination)[0] = reinterpret_cast<const Unaligned64*>(source)[0];
        reinterpret_cast<Unaligned64*>(destination)[1] = reinterpret_cast<const Unaligned64*>(source)[1];
        reinterpret_cast<Unaligned64*>(destination)[2] = reinterpret_cast<const Unaligned64*>(source)[2];
        reinterpret_cast<Unaligned64*>(destination)[3] = reinterpret_cast<const Unaligned64*>(source)[3];
        source += 32;
        destination += 32;
    }
    *reinterpret_cast<Unaligned64*>(destination) = *reinterpret_cast<const Unaligned64*>(source);
    *reinterpret_cast<Unaligned32*>(destination + 8) = *reinterpret_cast<const Unaligned32*>(source + 8);
    SignalSema(norrath_lgaud_semaphore_view);
    return result;
}
}
