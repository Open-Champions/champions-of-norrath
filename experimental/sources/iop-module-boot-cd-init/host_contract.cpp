#define NORRATH_HOST_CONTRACT 1
#include "cd_init.cpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" int SCE_CD_debug = 0;
extern "C" int cb_semid = -1;
extern "C" int _ncmd_bind = -1;
extern "C" int _sf_bind = -1;
extern "C" int _scmd_bind = -1;
extern "C" int my_thid = 0;
extern "C" unsigned char norrath_archive_cd_command_count_view[4] = {};
extern "C" unsigned _sceCd_ee_read_mode = 0;
extern "C" int _sceCd_ncmd_semid = 0;
extern "C" int _sceCd_scmd_semid = 0;
extern "C" unsigned char _sceCd_scmdrdata[16] = {};
int norrath_cd_init_wait_steps;

static int sync_result;
static int sync_mode;
static int thread_id = 15;
static int inits;
static int prints;
static char printed[80];
static int printed_left;
static int printed_right;
static int binds;
static int bind_results[4];
static int plant;
static int rpcs;
static int rpc_result;
static void* rpc_send;
static int rpc_send_size;
static int rpc_receive_size;
static int exits;
static int sem_inits;
static int power_offs;
static int sem_during_exit_n;
static int sem_during_exit_s;
static int sem_during_exit_c;

extern "C" int sceCdSyncS(int mode) {
    sync_mode = mode;
    return sync_result;
}
extern "C" void sceSifInitRpc(int mode) {
    if (mode != 0) std::exit(20);
    ++inits;
}
extern "C" int GetThreadId() { return thread_id; }
extern "C" int scePrintf(const char* text, ...) {
    ++prints;
    std::snprintf(printed, sizeof printed, "%s", text);
    va_list args;
    va_start(args, text);
    if (std::strcmp(text, "Libcdvd bind err %d CD_Init %d\n") == 0) {
        printed_left = va_arg(args, int);
        printed_right = va_arg(args, int);
    }
    va_end(args);
    return 1;
}
extern "C" int sceSifBindRpc(void* client, unsigned server, int mode) {
    if (server != 0x80000592u || mode != 0 || client != cdit) std::exit(21);
    unsigned value = 0x44u;
    if (plant == 2 && binds == 0) value = 0;
    if (plant == 3) value = 0;
    cdit[36] = static_cast<unsigned char>(value);
    cdit[37] = 0;
    cdit[38] = 0;
    cdit[39] = 0;
    const int result = bind_results[binds];
    ++binds;
    return result;
}
extern "C" void sceSifWriteBackDCache(void* address, int bytes) {
    if (address != &_i_mode || bytes != 4) std::exit(22);
}
extern "C" int sceSifCallRpc(void* client, int operation, int mode, void* send, int send_size,
                             void* receive, int receive_size, void*, void*) {
    ++rpcs;
    if (client != cdit || operation != 0 || mode != 0 || receive != _sceCd_scmdrdata) std::exit(23);
    rpc_send = send;
    rpc_send_size = send_size;
    rpc_receive_size = receive_size;
    return rpc_result;
}
extern "C" void cdvd_exit() {
    ++exits;
    sem_during_exit_n = _sceCd_ncmd_semid;
    sem_during_exit_s = _sceCd_scmd_semid;
    sem_during_exit_c = cb_semid;
}
extern "C" void cmd_sem_init() { ++sem_inits; }
extern "C" void PowerOffCB() { ++power_offs; }

static void reset() {
    sync_result = 0;
    inits = 0;
    prints = 0;
    binds = 0;
    plant = 1;
    rpcs = 0;
    rpc_result = 0;
    exits = 0;
    sem_inits = 0;
    power_offs = 0;
    norrath_cd_init_wait_steps = 0;
    SCE_CD_debug = 0;
    Init_seq = 7;
    cb_semid = 3;
    _ncmd_bind = 3;
    _icmd_bind = 3;
    _sf_bind = 3;
    _scmd_dr_bind = 3;
    _scmd_bind = 3;
    _it_bind = 3;
    my_thid = 0;
    _sceCd_ee_read_mode = 5;
    _sceCd_ncmd_semid = 4;
    _sceCd_scmd_semid = 6;
    store_word(norrath_archive_cd_command_count_view, 10);
    std::memset(_sceCd_scmdrdata, 0, sizeof _sceCd_scmdrdata);
    bind_results[0] = 1;
    bind_results[1] = 1;
}

int main() {
    if (sizeof cdit != 40) return 1;
    reset();
    sync_result = 1;
    Init_seq = 5;
    if (sceCdInit(0) != 0 || sync_mode != 1 || inits != 0 || load_word(norrath_archive_cd_command_count_view) != 10) return 2;
    if (Init_seq != 5) return 3;

    reset();
    store_word(_sceCd_scmdrdata + 12, 255);
    if (sceCdInit(0) != 1 || my_thid != 15 || Init_seq != 0 || _it_bind != 0) return 4;
    if (_i_mode != 0 || _sceCd_ee_read_mode != 0 || load_word(norrath_archive_cd_command_count_view) != 11) return 5;
    if (_ncmd_bind != -1 || _icmd_bind != -1 || _sf_bind != -1 || _scmd_bind != -1 || _scmd_dr_bind != -1) return 6;
    if (sem_inits != 1 || power_offs != 1 || exits != 0 || rpcs != 1 || rpc_send != &_i_mode || rpc_send_size != 4) return 7;
    if (rpc_receive_size != 16) return 8;

    reset();
    SCE_CD_debug = 2;
    bind_results[0] = -8;
    bind_results[1] = 1;
    store_word(_sceCd_scmdrdata + 4, 0);
    if (sceCdInit(2) != 2 || prints != 1 || printed_left != -8 || printed_right != 11) return 9;
    if (std::strcmp(printed, "Libcdvd bind err %d CD_Init %d\n") != 0) return 10;
    if (norrath_cd_init_wait_steps != 0x100001 || binds != 2 || sem_inits != 1) return 11;

    reset();
    plant = 2;
    store_word(_sceCd_scmdrdata + 4, 512);
    store_word(_sceCd_scmdrdata + 8, 100);
    if (sceCdInit(1) != 2 || norrath_cd_init_wait_steps != 0x100001 || prints != 0) return 12;

    reset();
    store_word(_sceCd_scmdrdata + 4, 512);
    store_word(_sceCd_scmdrdata + 8, 512);
    store_word(_sceCd_scmdrdata + 12, 1);
    if (sceCdInit(3) != 1) return 13;

    reset();
    store_word(_sceCd_scmdrdata + 4, static_cast<unsigned>(-1));
    if (sceCdInit(4) != 2) return 14;

    reset();
    store_word(_sceCd_scmdrdata + 12, 254);
    SCE_CD_debug = 0;
    if (sceCdInit(5) != 1 || SCE_CD_debug != 1 || prints != 1) return 15;
    if (std::strcmp(printed, "Libcdvd Exit\n") != 0 || exits != 1 || sem_inits != 0 || power_offs != 0) return 16;
    if (sem_during_exit_n != 4 || sem_during_exit_s != 6 || sem_during_exit_c != 3) return 17;
    if (_sceCd_ncmd_semid != -1 || _sceCd_scmd_semid != -1 || cb_semid != -1) return 18;

    reset();
    SCE_CD_debug = 0;
    store_word(_sceCd_scmdrdata + 12, 255);
    rpc_result = -1;
    if (sceCdInit(0) != 0 || Init_seq != 0 || _it_bind != 0 || sem_inits != 0 || exits != 0) return 19;
    std::puts("sceCdInit passed");
    return 0;
}
