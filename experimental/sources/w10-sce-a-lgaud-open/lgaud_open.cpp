/* Observed lgAudOpen at 0x002e9550, 356 bytes. Arguments: a device number, a
 * pointer to the open parameters and a pointer for the returned handle (a
 * fourth argument is passed by callers but not used). The library lock word
 * (0x00447f7c) equal to -1 returns 0x8000000a with nothing else done. These
 * parameter checks print a message and return 0x80000004: a null handle
 * pointer ("lgAudOpen: invalid parameter (device)\n" - the text names the
 * wrong argument), a null parameter pointer ("... (openparam)\n"), and a zero
 * first byte of the parameters ("... (openparam->Mode)\n"). Then the lock
 * semaphore is awaited and, in the shared request buffer (pointer at
 * 0x00447f74), word 2 gets the device number and bytes 32 to 45 get the first
 * 14 bytes of the parameters (copied inline, not by a library call). sceSifCallRpc runs on the library client
 * (0x008ea780) with function 2, mode 0, the buffer as the 48-byte send area
 * and as the 32-byte receive area, and no end function or argument. A nonzero
 * call result prints "lgAudOpen: rpc call failed\n", gives the semaphore back
 * and returns 0x80000000. Otherwise buffer word 3 becomes the caller's
 * handle, the semaphore is given back and buffer word 0 (read before the
 * semaphore is given back) is returned.
 * The meaning of the parameter bytes is not established here.
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
typedef unsigned short Unaligned16 __attribute__((aligned(1), may_alias));

unsigned lgAudOpen(unsigned device, const unsigned char* parameters, unsigned* handle) {
    if (norrath_lgaud_semaphore_view == -1) return 0x8000000au;
    if (handle == nullptr) {
        printf("lgAudOpen: invalid parameter (device)\n");
        return 0x80000004u;
    }
    if (parameters == nullptr) {
        printf("lgAudOpen: invalid parameter (openparam)\n");
        return 0x80000004u;
    }
    if (parameters[0] == 0) {
        printf("lgAudOpen: invalid parameter (openparam->Mode)\n");
        return 0x80000004u;
    }
    WaitSema(norrath_lgaud_semaphore_view);
    unsigned char* buffer = norrath_lgaud_request_buffer_view;
    *reinterpret_cast<unsigned*>(buffer + 8) = device;
    *reinterpret_cast<Unaligned64*>(buffer + 32) = *reinterpret_cast<const Unaligned64*>(parameters);
    *reinterpret_cast<Unaligned32*>(buffer + 40) = *reinterpret_cast<const Unaligned32*>(parameters + 8);
    *reinterpret_cast<Unaligned16*>(buffer + 44) = *reinterpret_cast<const Unaligned16*>(parameters + 12);
    if (sceSifCallRpc(norrath_lgaud_rpc_client_view, 2, 0, buffer, 48, buffer, 32, nullptr, nullptr) != 0) {
        printf("lgAudOpen: rpc call failed\n");
        SignalSema(norrath_lgaud_semaphore_view);
        return 0x80000000u;
    }
    buffer = norrath_lgaud_request_buffer_view;
    *handle = *reinterpret_cast<unsigned*>(buffer + 12);
    const unsigned result = *reinterpret_cast<unsigned*>(buffer);
    SignalSema(norrath_lgaud_semaphore_view);
    return result;
}
}
