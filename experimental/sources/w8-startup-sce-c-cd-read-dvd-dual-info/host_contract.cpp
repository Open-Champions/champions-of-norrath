#include <cstdio>
#define NORRATH_HOST_CONTRACT 1
#include "read_dvd_dual_info.cpp"
static int prechk = 1, rpc_result = 0, signals = 0, rpc_calls = 0, bad_args = 0;
static unsigned function_seen, mode_seen;
extern "C" int _sceCd_scmd_semid = 6;
extern "C" alignas(4) unsigned char _sceCd_scmdrdata[16] = {};
extern "C" unsigned char _sceCd_cd_scmd[4] = {};
extern "C" int _sceCd_scmd_prechk(int command) { return command == 39 ? prechk : -1; }
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                             void* receive, int receive_size, void* end, void* end_param) {
    ++rpc_calls; function_seen = function; mode_seen = mode;
    if (client != _sceCd_cd_scmd || send != nullptr || send_size != 0 || receive != _sceCd_scmdrdata ||
        receive_size != 12 || end != nullptr || end_param != nullptr) bad_args = 1;
    return rpc_result;
}
extern "C" int norrath_player_signal_semaphore(int semaphore) {
    if (semaphore == 6) ++signals;
    return 0;
}
int main() {
    unsigned dual = 111, layer = 222;
    prechk = 0;
    if (sceCdReadDvdDualInfo(&dual, &layer) != 0u || rpc_calls != 0 || signals != 0 || dual != 111) return 1;
    prechk = 1; rpc_result = -1;
    if (sceCdReadDvdDualInfo(&dual, &layer) != 0u || signals != 1 || rpc_calls != 1 || dual != 111 || layer != 222) return 2;
    if (function_seen != 39u || mode_seen != 0u || bad_args) return 3;
    rpc_result = 0;
    reinterpret_cast<unsigned*>(_sceCd_scmdrdata)[0] = 0xabcdu;
    reinterpret_cast<unsigned*>(_sceCd_scmdrdata)[1] = 1u;
    reinterpret_cast<unsigned*>(_sceCd_scmdrdata)[2] = 0x1234u;
    if (sceCdReadDvdDualInfo(&dual, &layer) != 0xabcdu || dual != 1u || layer != 0x1234u || signals != 2) return 4;
    std::puts("sceCdReadDvdDualInfo contract passed");
    return 0;
}
