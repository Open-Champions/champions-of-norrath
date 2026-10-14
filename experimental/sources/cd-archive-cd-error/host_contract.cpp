#include <cstdio>
#include <cstdint>
extern "C" int _sceCd_scmd_semid = 9;
extern "C" unsigned char _sceCd_cd_scmd[40] = {};
extern "C" unsigned char _sceCd_scmdrdata[16] = {};
static int prechk_result;
static int prechk_command;
static int rpc_result;
static int signals;
static unsigned rpc_function;
static void* rpc_client;
static void* rpc_receive;
static int rpc_receive_size;
static void* rpc_send;
extern "C" int _sceCd_scmd_prechk(int command) {
    prechk_command = command;
    return prechk_result;
}
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned, void* send, int,
                             void* receive, int receive_size, void*, void*) {
    rpc_client = client;
    rpc_function = function;
    rpc_send = send;
    rpc_receive = receive;
    rpc_receive_size = receive_size;
    return rpc_result;
}
extern "C" int norrath_player_signal_semaphore(int semaphore) {
    if (semaphore != 9) return 1;
    ++signals;
    return 0;
}
extern "C" int norrath_archive_cd_error();
static void store(unsigned value) {
    _sceCd_scmdrdata[0] = static_cast<unsigned char>(value);
    _sceCd_scmdrdata[1] = static_cast<unsigned char>(value >> 8);
    _sceCd_scmdrdata[2] = static_cast<unsigned char>(value >> 16);
    _sceCd_scmdrdata[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    prechk_result = 0;
    if (norrath_archive_cd_error() != -1 || prechk_command != 3 || signals != 0) return 1;
    prechk_result = 1;
    rpc_result = -4;
    if (norrath_archive_cd_error() != -1 || signals != 1 || rpc_function != 4u) return 2;
    if (rpc_client != _sceCd_cd_scmd || rpc_send != nullptr || rpc_receive != _sceCd_scmdrdata) return 3;
    if (rpc_receive_size != 4) return 4;
    rpc_result = 0;
    store(0x22u);
    if (norrath_archive_cd_error() != 0x22 || signals != 2) return 5;
    store(0u);
    if (norrath_archive_cd_error() != 0 || signals != 3) return 6;
    std::puts("cd error contract passed");
    return 0;
}
