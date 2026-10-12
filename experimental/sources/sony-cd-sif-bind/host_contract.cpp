#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#define NORRATH_HOST_CONTRACT 1
#include "bind_rpc.cpp"

static std::vector<std::string> trace;
static unsigned char packet_bytes[64];
static int have_packet = 1;
static int sema_result = 6;
static int send_result = 1;
extern "C" unsigned char _data_table[4] = {};
extern "C" void* _sceRpcGetPacket(void* pool) {
    trace.push_back(pool == _data_table ? "get" : "get-bad");
    return have_packet ? packet_bytes : nullptr;
}
extern "C" void _sceRpcFreePacket(void* packet) {
    trace.push_back(packet == packet_bytes ? "free" : "free-bad");
}
extern "C" int sceSifSendCmd(unsigned command, void* packet, int packet_bytes_count, void*, void*, int data_bytes) {
    trace.push_back("send:" + std::to_string(command) + ":" + std::to_string(packet_bytes_count) + ":" +
                    std::to_string(data_bytes));
    if (packet != packet_bytes) trace.push_back("send-bad");
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
int main() {
    unsigned char client[40];
    std::memset(client, 0xab, sizeof client);
    std::memset(packet_bytes, 0, sizeof packet_bytes);
    packet_bytes[24] = 5;
    have_packet = 0;
    if (sceSifBindRpc(client, 0x80000595u, 0) != -1) return 1;
    if (word_at(client + 16) != 0u || word_at(client + 36) != 0u) return 2;
    have_packet = 1;
    trace.clear();
    if (sceSifBindRpc(client, 0x80000595u, 1) != 0) return 3;
    if (word_at(packet_bytes + 32) != 0x80000595u || word_at(client + 4) != 5u) return 4;
    if (word_at(client + 8) != 0xffffffffu || trace[1] != "send:2147483657:64:0") return 5;
    send_result = 0;
    trace.clear();
    if (sceSifBindRpc(client, 1, 1) != -2 || trace.back() != "free") return 6;
    send_result = 1;
    trace.clear();
    if (sceSifBindRpc(client, 9, 0) != 0) return 7;
    if (trace[1] != "create:1:0" || trace[3] != "wait:6" || trace[4] != "delete:6") return 8;
    send_result = 0;
    trace.clear();
    if (sceSifBindRpc(client, 9, 0) != -2 || trace[3] != "free" || trace[4] != "delete:6") return 9;
    sema_result = -1;
    send_result = 1;
    trace.clear();
    if (sceSifBindRpc(client, 9, 0) != -3 || trace.back() != "free") return 10;
    std::puts("sif bind contract passed");
    return 0;
}
