#include "mc_get_info.cpp"

#include <cstdio>
#include <cstring>

int mcClientID[10];
int sifParamOrd[12];
int retval[3];
int mcRunCmdNo;
int semaidRegFunc = 6;
int norrath_libmc_info_type_pointer_word_view[1];
int norrath_libmc_info_free_pointer_word_view[1];
int norrath_libmc_info_format_pointer_word_view[1];
int norrath_libmc_param_next_view[48];

static int poll_result, rpc_result, polls, signals, writebacks, rpcs;
static int poll_id, signal_id;
static int wb_size;
static void* wb_address;
static void *g_client, *g_send, *g_recv, *g_arg;
static unsigned g_cmd, g_mode;
static int g_ssize, g_rsize;
static void (*g_done)(void*);
static int param_at_call[12];

extern "C" {
int PollSema(int id) { polls += 1; poll_id = id; return poll_result; }
int SignalSema(int id) { signals += 1; signal_id = id; return 0; }
void sceSifWriteBackDCache(void* address, int size) { writebacks += 1; wb_address = address; wb_size = size; }
int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void (*done)(void*), void* done_arg) {
    rpcs += 1;
    g_client = client; g_cmd = command; g_mode = mode; g_send = send; g_ssize = send_size;
    g_recv = receive; g_rsize = receive_size; g_done = done; g_arg = done_arg;
    std::memcpy(param_at_call, sifParamOrd, sizeof param_at_call);
    return rpc_result;
}
void mceGetInfoApdx(void*) {}
}

static int low(const void* p) { return word_of(p); }

int main() {
    /* unbound client: nothing else happens */
    mcClientID[9] = 0;
    sifParamOrd[1] = 99;
    if (sceMcGetInfo(0, 1, 0, 0, 0) != -100 || polls != 0 || sifParamOrd[1] != 99) return 1;
    /* semaphore busy */
    mcClientID[9] = 5;
    poll_result = -1;
    if (sceMcGetInfo(0, 1, 0, 0, 0) != -200 || polls != 1 || poll_id != 6 || rpcs != 0 || sifParamOrd[1] != 99) return 2;
    /* success: all three pointers given */
    poll_result = 0; rpc_result = 0;
    sifParamOrd[0] = 0x4242;
    int type, free_clusters, format;
    if (sceMcGetInfo(1, 2, &type, &free_clusters, &format) != 0) return 3;
    if (g_client != mcClientID || g_cmd != 1 || g_mode != 1 || g_send != sifParamOrd || g_ssize != 48) return 4;
    if (g_recv != retval || g_rsize != 4 || g_done != mceGetInfoApdx || g_arg != norrath_libmc_param_next_view) return 5;
    if (param_at_call[0] != 0x4242 || param_at_call[1] != 1 || param_at_call[2] != 2) return 6;
    if (param_at_call[3] != 1 || param_at_call[4] != 1 || param_at_call[5] != 1) return 7;
    if (param_at_call[7] != low(norrath_libmc_param_next_view)) return 8;
    if (norrath_libmc_info_type_pointer_word_view[0] != low(&type) ||
        norrath_libmc_info_free_pointer_word_view[0] != low(&free_clusters) ||
        norrath_libmc_info_format_pointer_word_view[0] != low(&format))
        return 9;
    if (writebacks != 1 || wb_address != norrath_libmc_param_next_view || wb_size != 192) return 10;
    if (mcRunCmdNo != 1 || signals != 0) return 11;
    /* failure: only the first and last pointers are given */
    mcRunCmdNo = 0;
    rpc_result = -3;
    if (sceMcGetInfo(0, 0, &type, 0, &format) != -3) return 12;
    if (param_at_call[5] != 1 || param_at_call[4] != 0 || param_at_call[3] != 1) return 13;
    if (norrath_libmc_info_free_pointer_word_view[0] != 0) return 14;
    if (mcRunCmdNo != 0 || signals != 1 || signal_id != 6) return 15;
    std::puts("mc get info contract passed");
    return 0;
}
