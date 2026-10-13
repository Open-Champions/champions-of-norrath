// Observed sceSifMBindRpcParam at 0x002e4910, 428 bytes: the bind routine of
// the multi-client RPC module (its packet pool is the local at 0x008e8c00, its
// getter/free helpers the copies at 0x002e4700 and 0x002e47a8). Arguments:
// client, rpc number, mode, and three parameter words that are kept in the
// client. Client words used (by word index): 0 packet address, 1 packet id,
// 2 reply semaphore, 4 rpc number, 9 bound flag, 10 lock semaphore, 12..14 the
// three parameter words.
// A nonzero mode prints a fixed diagnostic and returns -1. Otherwise it
// creates a lock semaphore (max 1, initial 1) kept in word 10 even when
// creation fails, returns -3 on failure, and takes it. Words 4 and 9 and the
// parameters are then written, a packet is taken from the pool (none: delete
// the lock semaphore, return -1), the packet and client cross-reference each
// other, and the rpc number and parameters are copied into packet words 8..11.
// A reply semaphore (max 1, initial 0) is created into word 2 (failure: free
// the packet, delete the lock, return -3). Command 0x80000019 is sent (a zero
// send frees the packet, deletes both semaphores, returns -2), the reply
// semaphore is awaited and deleted, and the lock semaphore is deleted only
// when the bound flag (word 9) is still zero. Returns 0 on both of those exits.
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
int CreateSema(void* parameters);
int DeleteSema(int semaphore);
int WaitSema(int semaphore);
int scePrintf(const char* format, ...);

static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}

int sceSifMBindRpcParam(void* client, unsigned rpc_number, unsigned mode, unsigned first,
                        unsigned second, unsigned third) {
    auto* words = static_cast<unsigned*>(client);
    if (mode != 0) {
        scePrintf("mode must be 0 in this version.\n");
        return -1;
    }
    SemaphoreParameter lock;
    lock.max_count = 1;
    lock.init_count = 1;
    lock.option = 0;
    const int lock_semaphore = CreateSema(&lock);
    words[10] = static_cast<unsigned>(lock_semaphore);
    if (lock_semaphore < 0) return -3;
    WaitSema(lock_semaphore);
    words[4] = rpc_number;
    words[9] = 0;
    words[12] = first;
    words[13] = second;
    words[14] = third;
    auto* packet = static_cast<unsigned*>(_sceRpcGetPacket(norrath_sif_m_rpc_packet_pool_view));
    if (packet == nullptr) {
        DeleteSema(static_cast<int>(words[10]));
        return -1;
    }
    words[0] = pointer_bits(packet);
    words[1] = packet[6];
    packet[8] = rpc_number;
    packet[9] = first;
    packet[10] = second;
    packet[11] = third;
    packet[5] = pointer_bits(packet);
    packet[7] = pointer_bits(client);
    SemaphoreParameter reply;
    reply.max_count = 1;
    reply.init_count = 0;
    reply.option = 0;
    const int reply_semaphore = CreateSema(&reply);
    words[2] = static_cast<unsigned>(reply_semaphore);
    if (reply_semaphore < 0) {
        _sceRpcFreePacket(packet);
        DeleteSema(static_cast<int>(words[10]));
        return -3;
    }
    if (sceSifSendCmd(0x80000019u, packet, 64, nullptr, nullptr, 0) == 0) {
        _sceRpcFreePacket(packet);
        DeleteSema(reply_semaphore);
        DeleteSema(static_cast<int>(words[10]));
        return -2;
    }
    WaitSema(reply_semaphore);
    DeleteSema(reply_semaphore);
    if (words[9] == 0) DeleteSema(static_cast<int>(words[10]));
    return 0;
}
}
