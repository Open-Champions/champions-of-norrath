#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#define NORRATH_HOST_CONTRACT 1
#include "init_rpc.cpp"

static std::vector<std::string> trace;
static int reg_value = 0;
static int sreg_reads = 0;
extern "C" unsigned char _data_table[64] = {};
extern "C" unsigned char _packet_buffer[128] = {};
extern "C" unsigned char _free_buffer[8] = {};
extern "C" unsigned char _free_buffer2[8] = {};
extern "C" int _sceSifInitCheck = 0;
extern "C" int DIntr() { trace.push_back("dintr"); return 0; }
extern "C" void EIntr() { trace.push_back("eintr"); }
extern "C" void sceSifInitCmd() { trace.push_back("initcmd"); }
extern "C" int sceSifAddCmdHandler(unsigned command, void (*handler)(void*, void*), void* parameter) {
    trace.push_back("handler:" + std::to_string(command));
    if (parameter != _data_table) trace.push_back("handler-bad");
    if (command == 0x80000008u && handler != _request_end) trace.push_back("end-bad");
    if (command == 0x80000009u && handler != _request_bind) trace.push_back("bind-bad");
    if (command == 0x8000000au && handler != _request_call) trace.push_back("call-bad");
    if (command == 0x8000000cu && handler != _request_rdata) trace.push_back("rdata-bad");
    return 0;
}
extern "C" void _request_end(void*, void*) {}
extern "C" void _request_bind(void*, void*) {}
extern "C" void _request_call(void*, void*) {}
extern "C" void _request_rdata(void*, void*) {}
extern "C" unsigned long long sceSifGetReg(unsigned identifier) {
    trace.push_back("getreg:" + std::to_string(identifier));
    return static_cast<unsigned long long>(reg_value);
}
extern "C" unsigned long long sceSifGetSreg(unsigned identifier) {
    trace.push_back("getsreg:" + std::to_string(identifier));
    sreg_reads += 1;
    return sreg_reads < 2 ? 0ull : 1ull;
}
extern "C" int sceSifSetReg(unsigned identifier, unsigned value) {
    trace.push_back("setreg:" + std::to_string(identifier) + ":" + std::to_string(value));
    return 0;
}
extern "C" int sceSifSendCmd(unsigned command, void* packet, int packet_bytes, void*, void*, int) {
    trace.push_back("send:" + std::to_string(command) + ":" + std::to_string(packet_bytes));
    if (packet != _packet_buffer + 64) trace.push_back("send-bad");
    return 1;
}
static unsigned word_at(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
int main() {
    _sceSifInitCheck = 7;
    sceSifInitRpc();
    if (trace.size() != 2 || trace[0] != "dintr" || trace[1] != "eintr") return 1;
    if (_sceSifInitCheck != 7) return 2;
    trace.clear();
    _sceSifInitCheck = 0;
    reg_value = 1;
    sceSifInitRpc();
    if (_sceSifInitCheck != 1) return 3;
    if (word_at(_data_table) != 1u || word_at(_data_table + 32) != 32u || word_at(_data_table + 36) != 0u) return 4;
    if (word_at(_data_table + 8) != 32u || word_at(_data_table + 24) != 32u) return 5;
    const unsigned packets = (static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(_packet_buffer)) & 0xffffffffu) | 0x20000000u;
    const unsigned free_packets = (static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(_free_buffer)) & 0xffffffffu) | 0x20000000u;
    const unsigned free_packets2 = (static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(_free_buffer2)) & 0xffffffffu) | 0x20000000u;
    if (word_at(_data_table + 4) != packets || word_at(_data_table + 20) != free_packets) return 6;
    if (word_at(_data_table + 28) != free_packets2) return 7;
    if (trace.back() != "getreg:2147483650") return 8;
    trace.clear();
    _sceSifInitCheck = 0;
    reg_value = 0;
    sreg_reads = 0;
    std::memset(_packet_buffer, 0, sizeof _packet_buffer);
    sceSifInitRpc();
    if (word_at(_packet_buffer + 76) != 1u) return 9;
    if (trace[trace.size() - 4] != "send:2147483650:16") return 10;
    if (trace[trace.size() - 1] != "setreg:2147483650:1") return 11;
    if (sreg_reads != 2) return 12;
    std::puts("sif init contract passed");
    return 0;
}
