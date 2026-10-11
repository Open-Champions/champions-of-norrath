#include <cstdio>
#include <cstdint>
#include <cstring>
#define NORRATH_HOST_CONTRACT 1
#include "stream.cpp"

static int prechk = 1;
static int rpc_result = 0;
static int writebacks = 0;
static int signals = 0;
static unsigned function_seen = 0;
static int send_size = 0;
extern "C" int SCE_CD_debug = 0;
extern "C" int _sceCd_ncmd_semid = 5;
extern "C" unsigned char _sceCd_ncmdsdata[24] = {};
extern "C" alignas(4) unsigned char _sceCd_ncmdrdata[8] = {};
extern "C" unsigned char _sceCd_cd_ncmd[4] = {};
extern "C" int _sceCd_ncmd_prechk(int command) { return command == 15 ? prechk : -1; }
extern "C" void sceSifWriteBackDCache(void* address, int bytes) {
    if (address == _sceCd_ncmdsdata && bytes == 20) writebacks += 1;
}
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned, void* send, int size, void* receive,
                             int receive_size, void*, void*) {
    function_seen = function;
    send_size = size;
    if (client != _sceCd_cd_ncmd || send != _sceCd_ncmdsdata || receive != _sceCd_ncmdrdata || receive_size != 4)
        return -9;
    return rpc_result;
}
extern "C" int norrath_player_signal_semaphore(int semaphore) {
    if (semaphore == 5) signals += 1;
    return 0;
}
extern "C" int scePrintf(const char*, ...) { return 0; }
static unsigned word_at(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
int main() {
    prechk = 0;
    if (sceCdStream(1, 2, nullptr, 7, nullptr) != 0u || signals != 0) return 1;
    prechk = 1;
    rpc_result = -1;
    unsigned char mode[3] = {9, 8, 7};
    unsigned char buffer[4];
    if (sceCdStream(4, 5, buffer, 7, mode) != 0u || signals != 1 || writebacks != 1) return 2;
    if (word_at(_sceCd_ncmdsdata) != 4u || word_at(_sceCd_ncmdsdata + 4) != 5u || word_at(_sceCd_ncmdsdata + 12) != 7u)
        return 3;
    if (_sceCd_ncmdsdata[16] != 9 || _sceCd_ncmdsdata[18] != 7 || function_seen != 9u || send_size != 20) return 4;
    rpc_result = 0;
    _sceCd_ncmdrdata[0] = 3;
    _sceCd_ncmdrdata[1] = 1;
    if (sceCdStream(0, 0, nullptr, 7, nullptr) != 0x103u || signals != 2) return 5;
    std::puts("sceCdStream contract passed");
    return 0;
}
