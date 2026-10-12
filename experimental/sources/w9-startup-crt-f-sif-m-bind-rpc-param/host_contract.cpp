// Host check only; not an EE build input. Runs where pointers are 8 bytes, so
// the stored packet/client "addresses" are compared by their low 32 bits.
#include <cstdint>
#include <cstring>
#include <vector>
extern "C" {
unsigned char norrath_sif_m_rpc_packet_pool_view[64];
}
static unsigned packet_store[16];
static unsigned client_store[16];
static std::vector<int> events;
static int create_results[2], create_index, send_result, packet_available;
static int create_seen[2][3];
extern "C" {
void* _sceRpcGetPacket(void* pool) {
    events.push_back(100);
    if (pool != norrath_sif_m_rpc_packet_pool_view) return nullptr;
    if (!packet_available) return nullptr;
    packet_store[6] = 0x55;
    return packet_store;
}
void _sceRpcFreePacket(void*) { events.push_back(101); }
int sceSifSendCmd(unsigned command, void* packet, int bytes, void* a, void* b, int c) {
    events.push_back(200);
    if (command != 0x80000019u || packet != packet_store || bytes != 64 || a || b || c) return 0;
    return send_result;
}
int CreateSema(void* parameters) {
    const int* w = static_cast<int*>(parameters);
    create_seen[create_index][0] = w[1];
    create_seen[create_index][1] = w[2];
    create_seen[create_index][2] = w[5];
    return create_results[create_index++];
}
int DeleteSema(int s) { events.push_back(300 + s); return 0; }
int WaitSema(int s) { events.push_back(400 + s); return 0; }
int scePrintf(const char*, ...) { events.push_back(500); return 0; }
int sceSifMBindRpcParam(void*, unsigned, unsigned, unsigned, unsigned, unsigned);
}
static void reset() {
    std::memset(packet_store, 0xcc, sizeof packet_store);
    std::memset(client_store, 0xcc, sizeof client_store);
    events.clear(); create_index = 0; packet_available = 1; send_result = 1;
    create_results[0] = 7; create_results[1] = 8;
}
int main() {
    reset();
    if (sceSifMBindRpcParam(client_store, 3, 1, 0, 0, 0) != -1 || events != std::vector<int>{500}) return 1;
    reset(); create_results[0] = -4;
    if (sceSifMBindRpcParam(client_store, 3, 0, 0, 0, 0) != -3 || client_store[10] != unsigned(-4)) return 2;
    reset(); packet_available = 0;
    if (sceSifMBindRpcParam(client_store, 3, 0, 1, 2, 3) != -1) return 3;
    if (events != std::vector<int>{407, 100, 307}) return 4;
    reset(); create_results[1] = -9;
    if (sceSifMBindRpcParam(client_store, 3, 0, 1, 2, 3) != -3 || (events != std::vector<int>{407, 100, 101, 307})) return 5;
    reset(); send_result = 0;
    if (sceSifMBindRpcParam(client_store, 3, 0, 1, 2, 3) != -2 || (events != std::vector<int>{407, 100, 200, 101, 308, 307})) return 6;
    reset();
    if (sceSifMBindRpcParam(client_store, 0x80001201u, 0, 0x11, 0x22, 0x33) != 0) return 7;
    if (events != std::vector<int>{407, 100, 200, 408, 308, 307}) return 8;
    if (create_seen[0][0] != 1 || create_seen[0][1] != 1 || create_seen[0][2] != 0) return 9;
    if (create_seen[1][0] != 1 || create_seen[1][1] != 0 || create_seen[1][2] != 0) return 10;
    if (client_store[4] != 0x80001201u || client_store[9] != 0 || client_store[10] != 7 || client_store[2] != 8) return 11;
    if (client_store[12] != 0x11 || client_store[13] != 0x22 || client_store[14] != 0x33 || client_store[1] != 0x55) return 12;
    if (packet_store[8] != 0x80001201u || packet_store[9] != 0x11 || packet_store[10] != 0x22 || packet_store[11] != 0x33) return 13;
    if (client_store[0] != unsigned(reinterpret_cast<std::uintptr_t>(packet_store))) return 14;
    return 0;
}
