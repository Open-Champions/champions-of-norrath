/* Observed sceSifMCallRpc at 0x002e4be0, 616 bytes: the call routine of the
 * multi-client RPC module (same packet pool and helper copies as
 * sceSifMBindRpcParam). Arguments: client, function number, mode, send
 * buffer, send size, receive buffer, receive size, end function, and (on the
 * stack) the end argument. Client words used (by word index): 0 packet
 * address, 1 packet id, 2 reply semaphore, 5 data parameter, 6 the caller's
 * global-pointer register, 7 end function, 8 end argument, 9 bound flag,
 * 10 lock semaphore.
 * A nonzero mode prints "mode must be 0 in this version." and returns -1. The
 * lock semaphore (word 10) is awaited. A send buffer address that is not a
 * multiple of 64 prints "%s: send buffer addr is not 64 byte align. %08x=n"
 * with "sceSifMCallRpc" and the address, and returns -1; the receive buffer
 * gets the same test and a "receive" message. The lock is left taken on all
 * of these early exits. With no packet from the pool the result is -1. The
 * client and packet reference each other and the packet is filled: word 8
 * function, 9 send size, 10 receive address, 11 receive size, 13 the client's
 * bound flag. The data cache is written back for the send buffer, or for the
 * larger of the two sizes when send and receive are the same address, or
 * for each buffer with a positive size otherwise. Bit 0 of the mode would
 * select a path that sends command 0x8000001a with the end function marking
 * word 12; since a nonzero mode is rejected first, that path (kept here in
 * the order the original has it) cannot run. With mode 0 a reply semaphore
 * (max 1, initial 0) is created into word 2 (failure: free the packet, -3),
 * packet word 12 is set to 1, command 0x8000001a is sent from the packet with
 * 64 bytes, the send buffer, client word 5 and the send size (a zero send
 * frees the packet, deletes the semaphore, -2), and the reply semaphore is
 * awaited and deleted. The result is then 0. The fixed text is as observed,
 * including the "=n" ending of the two alignment messages.
 */
extern "C" {
struct SemaphoreParameter {
    int count;
    int max_count;
    int init_count;
    int wait_threads;
    unsigned attribute;
    unsigned option;
};

extern unsigned char norrath_sif_m_rpc_packet_pool_view[];

void* _sceRpcGetPacket(void* pool);
void _sceRpcFreePacket(void* packet);
int sceSifSendCmd(unsigned command, void* packet, int packet_bytes, void* source,
                  void* destination, int data_bytes);
void sceSifWriteBackDCache(void* address, int size);
int CreateSema(void* parameters);
int DeleteSema(int semaphore);
int WaitSema(int semaphore);
int scePrintf(const char* format, ...);

static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

#if defined(NORRATH_HOST_CONTRACT)
extern unsigned norrath_host_global_pointer;  // host check: stands in for the register
static unsigned global_pointer_bits() { return norrath_host_global_pointer; }
#else
static unsigned global_pointer_bits() {
    unsigned value;
    __asm__("move %0,$28" : "=r"(value));
    return value;
}
#endif

int sceSifMCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size, void* receive,
                   int receive_size, void (*end_function)(void*), void* end_argument) {
    auto* words = static_cast<unsigned*>(client);
    if (mode != 0) {
        scePrintf("mode must be 0 in this version.\n");
        return -1;
    }
    WaitSema(static_cast<int>(words[10]));
    if ((pointer_bits(send) & 0x3f) != 0) {
        scePrintf("%s: send buffer addr is not 64 byte align. %08x=n", "sceSifMCallRpc", pointer_bits(send));
        return -1;
    }
    if ((pointer_bits(receive) & 0x3f) != 0) {
        scePrintf("%s: receive buffer addr is not 64 byte align. %08x=n", "sceSifMCallRpc", pointer_bits(receive));
        return -1;
    }
    auto* packet = static_cast<unsigned*>(_sceRpcGetPacket(norrath_sif_m_rpc_packet_pool_view));
    if (packet == nullptr) return -1;
    words[8] = pointer_bits(end_argument);
    words[1] = packet[6];
    words[0] = pointer_bits(packet);
    words[7] = pointer_bits(reinterpret_cast<const void*>(end_function));
    words[6] = global_pointer_bits();
    packet[8] = function;
    packet[5] = pointer_bits(packet);
    packet[9] = static_cast<unsigned>(send_size);
    packet[10] = pointer_bits(receive);
    packet[11] = static_cast<unsigned>(receive_size);
    packet[7] = pointer_bits(client);
    packet[13] = words[9];
    if (send == receive) {
        sceSifWriteBackDCache(send, send_size < receive_size ? receive_size : send_size);
    } else {
        if (send_size > 0) sceSifWriteBackDCache(send, send_size);
        if (receive_size > 0) sceSifWriteBackDCache(receive, receive_size);
    }
    if ((mode & 1) != 0) {
        packet[12] = end_function == nullptr ? 0u : 1u;
        words[2] = 0xffffffffu;
        if (sceSifSendCmd(0x8000001au, packet, 64, send, reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(words[5])), send_size) != 0)
            return 0;
        _sceRpcFreePacket(packet);
        return -2;
    }
    SemaphoreParameter reply;
    reply.max_count = 1;
    reply.init_count = 0;
    reply.option = 0;
    const int reply_semaphore = CreateSema(&reply);
    words[2] = static_cast<unsigned>(reply_semaphore);
    if (reply_semaphore < 0) {
        _sceRpcFreePacket(packet);
        return -3;
    }
    packet[12] = 1;
    if (sceSifSendCmd(0x8000001au, packet, 64, send, reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(words[5])), send_size) == 0) {
        _sceRpcFreePacket(packet);
        DeleteSema(reply_semaphore);
        return -2;
    }
    WaitSema(reply_semaphore);
    DeleteSema(reply_semaphore);
    return 0;
}
}
