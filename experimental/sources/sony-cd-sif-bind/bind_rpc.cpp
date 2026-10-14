// Observed sceSifBindRpc at 0x002d80a0, 320 bytes.
// Client words at +16 and +36 are cleared before the packet request, including
// when that request fails. Mode bit 1 sends command 0x80000009 and does not
// wait. A zero send frees the packet and returns -2. The waiting path frees
// the packet before deleting the semaphore when the send result is zero.
extern "C" {
extern unsigned char _data_table[];
void* _sceRpcGetPacket(void* pool);
void _sceRpcFreePacket(void* packet);
int sceSifSendCmd(unsigned command, void* packet, int packet_bytes, void* source,
                  void* destination, int data_bytes);
int CreateSema(void* parameters);
int DeleteSema(int semaphore);
int norrath_player_wait_semaphore(int semaphore);
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
int sceSifBindRpc(void* client, unsigned server, unsigned mode) {
    auto* client_bytes = static_cast<unsigned char*>(client);
    store_word(client_bytes + 16, 0u);
    store_word(client_bytes + 36, 0u);
    auto* packet = static_cast<unsigned char*>(_sceRpcGetPacket(_data_table));
    if (packet == nullptr) return -1;
    store_word(client_bytes, pointer_bits(packet));
    store_word(client_bytes + 4, load_word(packet + 24));
    store_word(packet + 32, server);
    store_word(packet + 20, pointer_bits(packet));
    store_word(packet + 28, pointer_bits(client));
    if ((mode & 1u) != 0u) {
        store_word(client_bytes + 8, 0xffffffffu);
        const int sent = sceSifSendCmd(0x80000009u, packet, 64, nullptr, nullptr, 0);
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
    const int sent = sceSifSendCmd(0x80000009u, packet, 64, nullptr, nullptr, 0);
    if (sent == 0) {
        _sceRpcFreePacket(packet);
        DeleteSema(static_cast<int>(load_word(client_bytes + 8)));
        return -2;
    }
    norrath_player_wait_semaphore(static_cast<int>(load_word(client_bytes + 8)));
    DeleteSema(static_cast<int>(load_word(client_bytes + 8)));
    return 0;
}
}
