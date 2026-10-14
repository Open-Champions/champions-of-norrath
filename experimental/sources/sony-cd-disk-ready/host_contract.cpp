#include <cstdio>
#include <cstdint>
#define NORRATH_HOST_CONTRACT 1
#include "disk_ready.cpp"

static int prechk_command = -1;
static int prechk_result = 1;
static int rpc_result = 0;
static unsigned rpc_function = 0;
static int signals = 0;
extern "C" int _sceCd_ncmd_semid = 5;
extern "C" alignas(4) unsigned char _sceCd_cd_ncmd[40] = {};
extern "C" alignas(4) unsigned char _sceCd_ncmdrdata[8] = {};
extern "C" int _sceCd_ncmd_prechk(int command) {
    prechk_command = command;
    return prechk_result;
}
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned, void* send, int, void* receive,
                             int receive_size, void*, void*) {
    rpc_function = function;
    if (client != _sceCd_cd_ncmd || send != nullptr || receive != _sceCd_ncmdrdata || receive_size != 4) return -9;
    return rpc_result;
}
extern "C" int norrath_player_signal_semaphore(int semaphore) {
    if (semaphore != 5) return 1;
    signals += 1;
    return 0;
}
int main() {
    prechk_result = 0;
    if (sceCdNcmdDiskReady() != 0 || prechk_command != 2 || signals != 0) return 1;
    prechk_result = 1;
    rpc_result = -1;
    if (sceCdNcmdDiskReady() != 0 || signals != 1 || rpc_function != 14u) return 2;
    rpc_result = 0;
    _sceCd_ncmdrdata[0] = 6;
    if (sceCdNcmdDiskReady() != 6 || signals != 2) return 3;
    std::puts("disk ready contract passed");
    return 0;
}
