// Observed sceSifCallRpc at 0x002d8270, 500 bytes.
// A null packet returns -1. Otherwise the client and the 64-byte packet are
// filled, mode bit 2 skips the cache writeback, and mode bit 1 sends without
// waiting. A zero send result frees the packet and returns -2. A negative
// semaphore returns -3. The waiting path waits, then deletes that semaphore
// and returns 0. The original calls WaitSema; this calls the published
// semaphore wait of that same service.
extern "C" {
extern unsigned char _data_table[];
void* _sceRpcGetPacket(void* pool);
void _sceRpcFreePacket(void* packet);
void sceSifWriteBackDCache(void* address, int bytes);
int sceSifSendCmd(unsigned command, void* packet, int packet_bytes, void* source,
                  void* destination, int data_bytes);
int CreateSema(void* parameters);
int DeleteSema(int semaphore);
int norrath_player_wait_semaphore(int semaphore);
#if defined(NORRATH_HOST_CONTRACT)
unsigned norrath_sony_cd_current_gp();
#else
static unsigned current_gp() {
    unsigned value;
    asm volatile("move %0, $28" : "=r"(value));
    return value;
}
#endif
static unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                  void* receive, int receive_size, void* end_function, void* end_parameter) {
    auto* client_bytes = static_cast<unsigned char*>(client);
    auto* packet = static_cast<unsigned char*>(_sceRpcGetPacket(_data_table));
    if (packet == nullptr) return -1;
    store_word(client_bytes + 32, pointer_bits(end_parameter));
    store_word(client_bytes + 4, load_word(packet + 24));
    store_word(client_bytes, pointer_bits(packet));
    store_word(client_bytes + 28, pointer_bits(end_function));
#if defined(NORRATH_HOST_CONTRACT)
    store_word(client_bytes + 24, norrath_sony_cd_current_gp());
#else
    store_word(client_bytes + 24, current_gp());
#endif
    store_word(packet + 32, function);
    store_word(packet + 36, static_cast<unsigned>(send_size));
    store_word(packet + 40, pointer_bits(receive));
    store_word(packet + 44, static_cast<unsigned>(receive_size));
    store_word(packet + 20, pointer_bits(packet));
    const unsigned server = load_word(client_bytes + 36);
    store_word(packet + 28, pointer_bits(client));
    store_word(packet + 52, server);
    if ((mode & 2u) == 0u) {
        if (send == receive) {
            const int length = send_size < receive_size ? receive_size : send_size;
            sceSifWriteBackDCache(send, length);
        } else {
            if (send_size > 0) sceSifWriteBackDCache(send, send_size);
            if (receive_size > 0) sceSifWriteBackDCache(receive, receive_size);
        }
    }
    const unsigned destination = load_word(client_bytes + 20);
    auto* destination_pointer = reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(destination));
    if ((mode & 1u) != 0u) {
        store_word(packet + 48, end_function != nullptr ? 1u : 0u);
        store_word(client_bytes + 8, 0xffffffffu);
        const int sent = sceSifSendCmd(0x8000000au, packet, 64, send, destination_pointer, send_size);
        if (sent != 0) return 0;
        _sceRpcFreePacket(packet);
        return -2;
    }
    unsigned char workspace[12];
    store_word(workspace + 8, 0u);
    store_word(workspace + 4, 1u);
    const int semaphore = CreateSema(workspace);
    store_word(client_bytes + 8, static_cast<unsigned>(semaphore));
    if (semaphore < 0) {
        _sceRpcFreePacket(packet);
        return -3;
    }
    store_word(packet + 48, 1u);
    const int sent = sceSifSendCmd(0x8000000au, packet, 64, send, destination_pointer, send_size);
    if (sent == 0) {
        DeleteSema(static_cast<int>(load_word(client_bytes + 8)));
        _sceRpcFreePacket(packet);
        return -2;
    }
    norrath_player_wait_semaphore(static_cast<int>(load_word(client_bytes + 8)));
    DeleteSema(static_cast<int>(load_word(client_bytes + 8)));
    return 0;
}
}
