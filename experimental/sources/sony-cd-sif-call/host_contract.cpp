#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#define NORRATH_HOST_CONTRACT 1
#include "call_rpc.cpp"

static std::vector<std::string> trace;
static unsigned char packet_bytes[64];
static int packet_result = 1;
static int sema_result = 4;
static int send_result = 1;
static int gp_value = 0x44;
unsigned char _data_table[4];
extern "C" unsigned norrath_sony_cd_current_gp() { return gp_value; }
extern "C" void* _sceRpcGetPacket(void* pool) {
    trace.push_back(pool == _data_table ? "get" : "get-bad");
    if (packet_result == 0) return nullptr;
    return packet_bytes;
}
extern "C" void _sceRpcFreePacket(void* packet) {
    trace.push_back(packet == packet_bytes ? "free" : "free-bad");
}
extern "C" void sceSifWriteBackDCache(void* address, int bytes) {
    trace.push_back("wb:" + std::to_string(reinterpret_cast<std::uintptr_t>(address)) + ":" + std::to_string(bytes));
}
extern "C" int sceSifSendCmd(unsigned command, void* packet, int packet_bytes_count, void* source,
                             void* destination, int data_bytes) {
    trace.push_back("send:" + std::to_string(command) + ":" + std::to_string(packet_bytes_count) + ":" +
                    std::to_string(data_bytes));
    if (packet != packet_bytes || command != 0x8000000au) trace.push_back("send-bad");
    (void)source;
    (void)destination;
    return send_result;
}
extern "C" int CreateSema(void* parameters) {
    auto* bytes = static_cast<unsigned char*>(parameters);
    const unsigned init = bytes[4] | (bytes[5] << 8) | (bytes[6] << 16) | (bytes[7] << 24);
    const unsigned maximum = bytes[8] | (bytes[9] << 8) | (bytes[10] << 16) | (bytes[11] << 24);
    trace.push_back("create:" + std::to_string(init) + ":" + std::to_string(maximum));
    return sema_result;
}
extern "C" int DeleteSema(int semaphore) {
    trace.push_back("delete:" + std::to_string(semaphore));
    return 0;
}
extern "C" int norrath_player_wait_semaphore(int semaphore) {
    trace.push_back("wait:" + std::to_string(semaphore));
    return 0;
}
static unsigned word_at(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static int fail(int code) {
    std::fprintf(stderr, "call contract %d\n", code);
    for (const auto& item : trace) std::fprintf(stderr, "  %s\n", item.c_str());
    return code;
}
int main() {
    unsigned char client[40];
    unsigned char send_buf[8];
    unsigned char receive_buf[8];
    std::memset(client, 0, sizeof client);
    std::memset(packet_bytes, 0, sizeof packet_bytes);
    packet_bytes[24] = 9;
    client[36] = 3;
    client[20] = 8;
    packet_result = 0;
    if (sceSifCallRpc(client, 1, 0, send_buf, 4, receive_buf, 4, nullptr, nullptr) != -1) return fail(1);
    if (trace.size() != 1 || trace[0] != "get") return fail(2);
    packet_result = 1;
    trace.clear();
    void* end_parameter = send_buf;
    int result = sceSifCallRpc(client, 14, 3, send_buf, 4, receive_buf, 8, receive_buf, end_parameter);
    if (result != 0) return fail(3);
    if (word_at(client + 4) != 9u || word_at(client + 24) != 0x44u) return fail(4);
    if (word_at(client + 32) != static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(end_parameter))) return fail(5);
    if (word_at(packet_bytes + 32) != 14u || word_at(packet_bytes + 36) != 4u) return fail(6);
    if (word_at(packet_bytes + 44) != 8u || word_at(packet_bytes + 52) != 3u) return fail(7);
    if (word_at(packet_bytes + 48) != 1u || word_at(client + 8) != 0xffffffffu) return fail(8);
    if (trace.size() != 2 || trace[1] != "send:2147483658:64:4") return fail(9);
    trace.clear();
    send_result = 0;
    result = sceSifCallRpc(client, 1, 1, send_buf, 1, receive_buf, 1, nullptr, nullptr);
    if (result != -2 || trace.back() != "free") return fail(10);
    if (word_at(packet_bytes + 48) != 0u) return fail(11);
    trace.clear();
    send_result = 1;
    result = sceSifCallRpc(client, 2, 0, send_buf, 4, receive_buf, 8, nullptr, nullptr);
    if (result != 0) return fail(12);
    if (trace[1] != "wb:" + std::to_string(reinterpret_cast<std::uintptr_t>(send_buf)) + ":4") return fail(13);
    if (trace[2] != "wb:" + std::to_string(reinterpret_cast<std::uintptr_t>(receive_buf)) + ":8") return fail(14);
    if (trace[3] != "create:1:0" || trace[4].rfind("send:", 0) != 0 || trace[5] != "wait:4" || trace[6] != "delete:4")
        return fail(15);
    trace.clear();
    send_result = 0;
    result = sceSifCallRpc(client, 2, 0, send_buf, 4, receive_buf, 8, nullptr, nullptr);
    if (result != -2 || trace[5] != "delete:4" || trace[6] != "free") return fail(16);
    trace.clear();
    sema_result = -5;
    send_result = 1;
    result = sceSifCallRpc(client, 2, 0, send_buf, 0, receive_buf, 0, nullptr, nullptr);
    if (result != -3 || trace.back() != "free") return fail(17);
    if (word_at(client + 8) != 0xfffffffbU) return fail(18);
    trace.clear();
    sema_result = 4;
    result = sceSifCallRpc(client, 2, 2, send_buf, 4, receive_buf, 8, nullptr, nullptr);
    if (result != 0 || trace[1] != "create:1:0") return fail(19);
    trace.clear();
    result = sceSifCallRpc(client, 2, 0, send_buf, -1, send_buf, -5, nullptr, nullptr);
    if (trace[1] != "wb:" + std::to_string(reinterpret_cast<std::uintptr_t>(send_buf)) + ":-1") return fail(20);
    std::puts("sif call contract passed");
    return 0;
}
