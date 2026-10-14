#include <cstdio>
#define NORRATH_HOST_CONTRACT 1
#include "power_off.cpp"
extern "C" int _sceCd_scmd_semid = 7;
extern "C" alignas(4) unsigned char _sceCd_cd_scmd[40] = {};
extern "C" alignas(4) unsigned char _sceCd_scmdrdata[0x420] = {};
static int prechk_arg = -1, prechk_result = 1, rpc_result = 0, signals = 0, bad_sema = 0;
static unsigned rpc_function, rpc_mode; static int word0_at_signal = -1;
extern "C" int _sceCd_scmd_prechk(int command) { prechk_arg = command; return prechk_result; }
extern "C" int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size,
                             void* receive, int receive_size, void* end_function, void* end_parameter) {
    rpc_function = function; rpc_mode = mode;
    if (client != _sceCd_cd_scmd || send != nullptr || send_size != 0 || receive != _sceCd_scmdrdata ||
        receive_size != 8 || end_function != nullptr || end_parameter != nullptr) return -9;
    return rpc_result;
}
extern "C" int SignalSema(int semaphore) {
    if (semaphore != 7) bad_sema = 1;
    ++signals;
    // The buffer is handed to another user at the signal; prove the caller already took its values.
    word0_at_signal = *reinterpret_cast<int*>(_sceCd_scmdrdata);
    *reinterpret_cast<int*>(_sceCd_scmdrdata) = 0x7777;
    return 0;
}
int main() {
    int out = 55;
    prechk_result = 0;
    if (sceCdPowerOff(&out) != 0 || prechk_arg != 33 || signals != 0 || out != 55) return 1;
    prechk_result = 1; rpc_result = -1;
    if (sceCdPowerOff(&out) != 0 || signals != 1 || out != 55 || rpc_function != 33u || rpc_mode != 0u) return 2;
    rpc_result = 0;
    *reinterpret_cast<int*>(_sceCd_scmdrdata) = 1234;
    *reinterpret_cast<int*>(_sceCd_scmdrdata + 4) = 99;
    if (sceCdPowerOff(&out) != 1234 || out != 99 || signals != 2 || bad_sema) return 3;
    std::puts("sceCdPowerOff contract passed");
    return 0;
}
