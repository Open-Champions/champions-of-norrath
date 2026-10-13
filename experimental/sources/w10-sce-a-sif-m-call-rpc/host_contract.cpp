// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT -o /tmp/c host_contract.cpp sif_m_call_rpc.cpp
#include <cstdarg>
#include <initializer_list>
#include <cstdint>
#include <cstdio>
#include <cstring>
extern "C" {
unsigned char norrath_sif_m_rpc_packet_pool_view[32];
unsigned norrath_host_global_pointer = 0x00a5a5a0u;
static int log_len; static char log_kind[64]; static int log_arg[64];
static void note(char kind, int arg = 0) { log_kind[log_len] = kind; log_arg[log_len++] = arg; }
static unsigned packet[16];
static void* packet_to_give = packet; static void* pool_seen;
static char printed[4][160]; static int print_count;
static int sema_result = 9, send_result = 1;
static unsigned max_at_create, init_at_create, option_at_create;
static unsigned send_command; static void* send_packet; static int send_bytes; static void* send_src; static void* send_dst; static int send_len;
static unsigned packet_at_send[16];
static void *wb_addr[3]; static int wb_size[3], wb_count;
static void* free_arg; static int sem_waited[4], wait_count, deleted;
void* _sceRpcGetPacket(void* pool) { note('g'); pool_seen = pool; return packet_to_give; }
void _sceRpcFreePacket(void* p) { note('f'); free_arg = p; }
int WaitSema(int s) { note('w', s); sem_waited[wait_count++ & 3] = s; return 0; }
int DeleteSema(int s) { note('d', s); deleted = s; return 0; }
int CreateSema(void* parameter) {
    note('c');
    const unsigned* w = static_cast<const unsigned*>(parameter);
    max_at_create = w[1]; init_at_create = w[2]; option_at_create = w[5];
    return sema_result;
}
void sceSifWriteBackDCache(void* a, int n) { note('b'); if (wb_count < 3) { wb_addr[wb_count] = a; wb_size[wb_count] = n; } ++wb_count; }
int sceSifSendCmd(unsigned command, void* p, int bytes, void* src, void* dst, int len) {
    note('s'); send_command = command; send_packet = p; send_bytes = bytes; send_src = src; send_dst = dst; send_len = len;
    std::memcpy(packet_at_send, packet, sizeof packet);
    return send_result;
}
int scePrintf(const char* format, ...) {
    note('p');
    va_list args; va_start(args, format);
    std::vsnprintf(printed[print_count < 4 ? print_count : 3], 160, format, args);
    va_end(args); ++print_count; return 0;
}
int sceSifMCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size, void* receive,
                   int receive_size, void (*end_function)(void*), void* end_argument);
}
static unsigned bits(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
alignas(64) static unsigned char area[512];
static unsigned client[16];
static void reset() {
    log_len = 0; print_count = 0; wb_count = 0; wait_count = 0; sema_result = 9; send_result = 1; packet_to_give = packet;
    for (unsigned& w : client) w = 0x11111111u;
    for (unsigned& w : packet) w = 0x22222222u;
    client[10] = 0x55; client[9] = 0x66; client[5] = 0x77;
    packet[6] = 0x4242;
}
static void end_fn(void*) {}
static bool log_is(const char* kinds) { return std::strlen(kinds) == static_cast<size_t>(log_len) && std::memcmp(kinds, log_kind, log_len) == 0; }
int main() {
    char send_probe, recv_probe;
    (void)send_probe; (void)recv_probe;
    // nonzero mode: message and -1 before the lock is touched
    reset();
    if (sceSifMCallRpc(client, 3, 1, area, 64, area + 64, 64, end_fn, nullptr) != -1 || !log_is("p") ||
        std::strcmp(printed[0], "mode must be 0 in this version.\n") != 0) return 1;
    // unaligned send: lock taken, message, -1, nothing else
    reset();
    if (sceSifMCallRpc(client, 3, 0, area + 4, 64, area + 64, 64, nullptr, nullptr) != -1 || !log_is("wp")) return 2;
    if (sem_waited[0] != 0x55) return 3;
    char expect[160];
    std::snprintf(expect, sizeof expect, "sceSifMCallRpc: send buffer addr is not 64 byte align. %08x=n", bits(area + 4));
    if (std::strcmp(printed[0], expect) != 0) return 4;
    // unaligned receive
    reset();
    if (sceSifMCallRpc(client, 3, 0, area, 64, area + 65, 64, nullptr, nullptr) != -1 || !log_is("wp")) return 5;
    std::snprintf(expect, sizeof expect, "sceSifMCallRpc: receive buffer addr is not 64 byte align. %08x=n", bits(area + 65));
    if (std::strcmp(printed[0], expect) != 0) return 6;
    // 64-byte alignment, not just 32: offsets 32 are refused for both buffers, 64 is accepted
    reset();
    if (sceSifMCallRpc(client, 3, 0, area + 32, 64, area + 128, 64, nullptr, nullptr) != -1 || print_count != 1) return 28;
    reset();
    if (sceSifMCallRpc(client, 3, 0, area, 64, area + 160, 64, nullptr, nullptr) != -1 || print_count != 1) return 29;
    // send checked first
    reset();
    sceSifMCallRpc(client, 3, 0, area + 1, 64, area + 65, 64, nullptr, nullptr);
    if (std::strstr(printed[0], "send buffer") == nullptr || print_count != 1) return 7;
    // no packet
    reset(); packet_to_give = nullptr;
    if (sceSifMCallRpc(client, 3, 0, area, 64, area + 128, 64, nullptr, nullptr) != -1 || !log_is("wg") || client[0] != 0x11111111u) return 8;
    if (pool_seen != norrath_sif_m_rpc_packet_pool_view) return 9;
    // normal run
    reset();
    void* const end_arg = reinterpret_cast<void*>(0x00123450);
    const int r = sceSifMCallRpc(client, 0x12, 0, area, 64, area + 128, 192, end_fn, end_arg);
    if (r != 0 || !log_is("wgbbcswd")) return 10;
    if (client[8] != bits(end_arg) || client[1] != 0x4242 || client[0] != bits(packet) || client[7] != bits(reinterpret_cast<void*>(end_fn)) ||
        client[6] != 0x00a5a5a0u || client[2] != 9) return 11;
    for (int i : {3, 4, 5, 9, 10}) if (client[i] != (i == 10 ? 0x55u : i == 9 ? 0x66u : i == 5 ? 0x77u : 0x11111111u)) return 12;
    if (client[11] != 0x11111111u || client[12] != 0x11111111u || client[15] != 0x11111111u) return 13;
    if (packet[8] != 0x12 || packet[5] != bits(packet) || packet[9] != 64 || packet[10] != bits(area + 128) ||
        packet[11] != 192 || packet[7] != bits(client) || packet[13] != 0x66 || packet[12] != 1 || packet[6] != 0x4242) return 14;
    if (packet[0] != 0x22222222u || packet[4] != 0x22222222u || packet[14] != 0x22222222u || packet[15] != 0x22222222u) return 15;
    if (packet_at_send[12] != 1) return 16;                            // marked before the send
    if (wb_count != 2 || wb_addr[0] != area || wb_size[0] != 64 || wb_addr[1] != area + 128 || wb_size[1] != 192) return 17;
    if (max_at_create != 1 || init_at_create != 0 || option_at_create != 0) return 18;
    if (send_command != 0x8000001au || send_packet != packet || send_bytes != 64 || send_src != area ||
        send_dst != reinterpret_cast<void*>(0x77) || send_len != 64) return 19;
    if (sem_waited[0] != 0x55 || sem_waited[1] != 9 || deleted != 9) return 20;
    // zero or negative sizes are not written back
    reset();
    sceSifMCallRpc(client, 1, 0, area, 0, area + 128, -4, nullptr, nullptr);
    if (wb_count != 0 || client[7] != 0) return 21;
    reset();
    sceSifMCallRpc(client, 1, 0, area, -4, area + 128, 32, nullptr, nullptr);
    if (wb_count != 1 || wb_addr[0] != area + 128 || wb_size[0] != 32) return 22;
    // same buffer: one write-back of the larger size
    reset();
    sceSifMCallRpc(client, 1, 0, area, 32, area, 96, nullptr, nullptr);
    if (wb_count != 1 || wb_addr[0] != area || wb_size[0] != 96) return 23;
    reset();
    sceSifMCallRpc(client, 1, 0, area, 128, area, 96, nullptr, nullptr);
    if (wb_count != 1 || wb_size[0] != 128) return 24;
    // reply semaphore failure: packet freed, -3, no send
    reset(); sema_result = -2;
    if (sceSifMCallRpc(client, 1, 0, area, 64, area + 64, 64, nullptr, nullptr) != -3 || !log_is("wgbbcf") || free_arg != packet) return 25;
    if (client[2] != static_cast<unsigned>(-2) || packet[12] != 0x22222222u) return 26;
    // send refused: packet freed, semaphore deleted, -2, no wait for the reply
    reset(); send_result = 0;
    if (sceSifMCallRpc(client, 1, 0, area, 64, area + 64, 64, nullptr, nullptr) != -2 || !log_is("wgbbcsfd") || deleted != 9) return 27;
    return 0;
}
