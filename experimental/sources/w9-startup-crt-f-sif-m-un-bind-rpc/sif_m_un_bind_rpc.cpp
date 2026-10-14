// Observed sceSifMUnBindRpc at 0x002e4ac0, 284 bytes: the unbind routine of the
// multi-client RPC module (same packet pool and helper copies as
// sceSifMBindRpcParam). Arguments: client and mode. Client words used (by word
// index): 0 packet address, 1 packet id, 2 reply semaphore, 4 rpc number,
// 9 bound flag, 10 lock semaphore, 11 result word.
// A nonzero mode prints a fixed diagnostic and returns -1. Otherwise the lock
// semaphore (word 10) is awaited. With no packet available it returns -1 and
// the lock is left taken. With a packet, the client and packet reference each
// other, the packet gets the rpc number (word 4) and bound flag (word 9) from
// the client, and a reply semaphore (max 1, initial 0) is created into word 2
// (failure: free the packet, return -3, lock left taken). Command 0x8000001d
// is sent (a zero send frees the packet, deletes the reply semaphore, returns
// -2, lock left taken). Otherwise the reply semaphore is awaited and deleted,
// the lock semaphore is deleted, and word 11 of the client is returned.
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

int sceSifMUnBindRpc(void* client, unsigned mode) {
    auto* words = static_cast<unsigned*>(client);
    if (mode != 0) {
        scePrintf("mode must be 0 in this version.\n");
        return -1;
    }
    WaitSema(static_cast<int>(words[10]));
    auto* packet = static_cast<unsigned*>(_sceRpcGetPacket(norrath_sif_m_rpc_packet_pool_view));
    if (packet == nullptr) return -1;
    words[1] = packet[6];
    words[0] = pointer_bits(packet);
    packet[8] = words[4];
    packet[5] = pointer_bits(packet);
    packet[7] = pointer_bits(client);
    packet[9] = words[9];
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
    if (sceSifSendCmd(0x8000001du, packet, 64, nullptr, nullptr, 0) == 0) {
        _sceRpcFreePacket(packet);
        DeleteSema(reply_semaphore);
        return -2;
    }
    WaitSema(reply_semaphore);
    DeleteSema(reply_semaphore);
    DeleteSema(static_cast<int>(words[10]));
    return static_cast<int>(words[11]);
}
}
