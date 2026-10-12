#define NORRATH_HOST_CONTRACT 1
#include "cd_mmode.cpp"

#include <cstdio>
#include <cstring>

extern "C" unsigned char _sceCd_scmdsdata[8] = {};
extern "C" unsigned char _sceCd_cd_scmd[4] = {};
extern "C" unsigned char _sceCd_scmdrdata[8] = {};
extern "C" int _sceCd_scmd_semid = 9;

static int prechk_command;
static int prechk_result;
static int writebacks;
static void* writeback_address;
static int writeback_bytes;
static int rpcs;
static int rpc_operation;
static int rpc_send_size;
static int rpc_receive_size;
static void* rpc_client;
static void* rpc_send;
static void* rpc_receive;
static int rpc_result;
static int signals;
static int signal_id;
static unsigned signal_plant;

extern "C" int _sceCd_scmd_prechk(int command) {
    prechk_command = command;
    return prechk_result;
}
extern "C" void sceSifWriteBackDCache(void* address, int bytes) {
    ++writebacks;
    writeback_address = address;
    writeback_bytes = bytes;
}
extern "C" int sceSifCallRpc(void* client, int operation, int, void* send, int send_size,
                             void* receive, int receive_size, void*, void*) {
    ++rpcs;
    rpc_client = client;
    rpc_operation = operation;
    rpc_send = send;
    rpc_send_size = send_size;
    rpc_receive = receive;
    rpc_receive_size = receive_size;
    return rpc_result;
}
extern "C" int SignalSema(int semaphore) {
    ++signals;
    signal_id = semaphore;
    store_word(_sceCd_scmdrdata, signal_plant);
    return 0;
}

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}

int main() {
    std::memset(_sceCd_scmdsdata, 0xa5, sizeof _sceCd_scmdsdata);
    prechk_result = 0;
    if (sceCdMmode(2) != 0 || prechk_command != 34 || writebacks != 0 || rpcs != 0 || signals != 0) return 1;
    if (_sceCd_scmdsdata[0] != 0xa5) return 2;

    prechk_result = 1;
    rpc_mmode = 99;
    outcnt_mmode = 6;
    incnt_mmode = 7;
    rpc_result = -3;
    if (sceCdMmode(2) != 0 || signals != 1 || signal_id != 9 || rpcs != 1) return 3;
    if (load_word(_sceCd_scmdsdata) != 2 || writeback_address != _sceCd_scmdsdata || writeback_bytes != 4) return 4;
    if (rpc_operation != 99 || rpc_send_size != 6 || rpc_receive_size != 7) return 5;
    if (rpc_client != _sceCd_cd_scmd || rpc_send != _sceCd_scmdsdata || rpc_receive != _sceCd_scmdrdata) return 6;

    store_word(_sceCd_scmdrdata, 0x11223344u);
    signal_plant = 0;
    rpc_result = 0;
    signals = 0;
    if (sceCdMmode(8) != static_cast<int>(0x11223344u) || signals != 1 || load_word(_sceCd_scmdrdata) != 0) return 7;
    if (load_word(_sceCd_scmdsdata) != 8) return 8;
    std::puts("sceCdMmode passed");
    return 0;
}
