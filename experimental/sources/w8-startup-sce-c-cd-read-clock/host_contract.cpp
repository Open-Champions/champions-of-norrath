#include <cstdio>
#include <cstring>
#define NORRATH_HOST_CONTRACT 1
#include "read_clock.cpp"
static int prechk = 1, rpc_result = 0, signals = 0, rpc_calls = 0, bad_args = 0, prints = 0, print_one = 0, print_two = 0;
static unsigned function_seen;
extern "C" int SCE_CD_debug = 0;
extern "C" int _sceCd_scmd_semid = 6;
extern "C" alignas(4) unsigned char _sceCd_scmdrdata[16] = {};
extern "C" unsigned char _sceCd_cd_scmd[4] = {};
extern "C" int _sceCd_scmd_prechk(int command) { return command == 15 ? prechk : -1; }
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                             void* receive, int receive_size, void* end, void* end_param) {
    ++rpc_calls; function_seen = function;
    if (client != _sceCd_cd_scmd || mode != 0 || send != nullptr || send_size != 0 || receive != _sceCd_scmdrdata ||
        receive_size != 16 || end != nullptr || end_param != nullptr) bad_args = 1;
    return rpc_result;
}
extern "C" int norrath_player_signal_semaphore(int semaphore) { if (semaphore == 6) ++signals; return 0; }
extern "C" int scePrintf(const char* format, ...) {
    ++prints;
    if (std::strcmp(format, "Libcdvd call Clock read 1\n") == 0) print_one = prints;
    else if (std::strcmp(format, "Libcdvd call Clock read 2\n") == 0) print_two = prints;
    else bad_args = 1;
    return 0;
}
int main() {
    unsigned char record[10];
    std::memset(record, 0x55, sizeof record);
    prechk = 0; SCE_CD_debug = 1;
    if (sceCdReadClock(record) != 0u || rpc_calls != 0 || prints != 0 || signals != 0) return 1;
    prechk = 1; rpc_result = -1;
    if (sceCdReadClock(record) != 0u || rpc_calls != 1 || signals != 1 || print_one != 1 || print_two != 0 || record[0] != 0x55) return 2;
    if (function_seen != 1u || bad_args) return 3;
    rpc_result = 0; prints = print_one = print_two = 0;
    for (int index = 0; index < 16; ++index) _sceCd_scmdrdata[index] = static_cast<unsigned char>(0x10 + index);
    if (sceCdReadClock(record) != 0x13121110u || signals != 2) return 4;
    for (int index = 0; index < 8; ++index) if (record[index] != 0x14 + index) return 5;
    if (record[8] != 0x55 || record[9] != 0x55) return 6;
    if (print_one != 1 || print_two != 2) return 7;
    SCE_CD_debug = 0; prints = 0;
    if (sceCdReadClock(record) != 0x13121110u || prints != 0) return 8;
    std::puts("sceCdReadClock contract passed");
    return 0;
}
