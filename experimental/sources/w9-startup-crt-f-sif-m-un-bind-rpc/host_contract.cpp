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
static int create_result, send_result, packet_available;
extern "C" {
void* _sceRpcGetPacket(void* pool) {
    events.push_back(100);
    if (pool != norrath_sif_m_rpc_packet_pool_view || !packet_available) return nullptr;
    packet_store[6] = 0x66;
    return packet_store;
}
void _sceRpcFreePacket(void*) { events.push_back(101); }
int sceSifSendCmd(unsigned command, void* packet, int bytes, void* a, void* b, int c) {
    events.push_back(200);
    if (command != 0x8000001du || packet != packet_store || bytes != 64 || a || b || c) return 0;
    return send_result;
}
int CreateSema(void* parameters) {
    const int* w = static_cast<int*>(parameters);
    if (w[1] != 1 || w[2] != 0 || w[5] != 0) return -100;
    return create_result;
}
int DeleteSema(int s) { events.push_back(300 + s); return 0; }
int WaitSema(int s) { events.push_back(400 + s); return 0; }
int scePrintf(const char*, ...) { events.push_back(500); return 0; }
int sceSifMUnBindRpc(void*, unsigned);
}
static void reset() {
    std::memset(packet_store, 0xcc, sizeof packet_store);
    std::memset(client_store, 0xcc, sizeof client_store);
    client_store[10] = 6; client_store[4] = 0x1234; client_store[9] = 1; client_store[11] = 0x999;
    events.clear(); packet_available = 1; send_result = 1; create_result = 8;
}
int main() {
    reset();
    if (sceSifMUnBindRpc(client_store, 2) != -1 || events != std::vector<int>{500}) return 1;
    reset(); packet_available = 0;
    if (sceSifMUnBindRpc(client_store, 0) != -1 || events != std::vector<int>{406, 100}) return 2;
    reset(); create_result = -2;
    if (sceSifMUnBindRpc(client_store, 0) != -3 || events != std::vector<int>{406, 100, 101}) return 3;
    reset(); send_result = 0;
    if (sceSifMUnBindRpc(client_store, 0) != -2 || events != std::vector<int>{406, 100, 200, 101, 308}) return 4;
    reset();
    if (sceSifMUnBindRpc(client_store, 0) != 0x999) return 5;
    if (events != std::vector<int>{406, 100, 200, 408, 308, 306}) return 6;
    if (packet_store[8] != 0x1234 || packet_store[9] != 1 || client_store[1] != 0x66 || client_store[2] != 8) return 7;
    if (client_store[0] != unsigned(reinterpret_cast<std::uintptr_t>(packet_store))) return 8;
    return 0;
}
