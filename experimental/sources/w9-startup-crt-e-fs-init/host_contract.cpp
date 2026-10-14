#define NORRATH_HOST_CONTRACT 1
#include "fs_init.cpp"
#include <cstdio>
#include <cstring>
#include <string>

int _fs_init = 0; int _fs_rcv_bufdbl = 5; int _fs_iob_semid = 77;
alignas(16) unsigned char _iob[512]; alignas(16) unsigned char _cd[40];
alignas(16) unsigned char _rcv_data_cmd[0x880]; alignas(16) unsigned char _rcv_data_rpc[16];
unsigned char _fsversion[4]; alignas(16) unsigned char _sif_FsRcv_Data[64]; alignas(16) unsigned char _sif_FsPoff_Data[64];
unsigned norrath_fs_rcv_adr_view[2];

static std::string trace;
static void t(const char* s) { trace += s; trace += ';'; }
static int dintr_result = 1, bind_calls, bind_negative_at = -1, bind_server_after = 1, rpc_result = 0, spin_marker;
static unsigned reply[2] = {0x01020304u, 2};
static int handler_ids[2], handler_count; static void* handler_fn[2]; static void* handler_arg[2];
static unsigned sent[2]; static unsigned function_seen, mode_seen; static void *send_seen, *recv_seen, *client_seen, *end_seen, *end_arg;
static int send_len, recv_len; static unsigned poff_at_dintr[2]; static int slot_words_at_signal = -1;

extern "C" {
void sceSifInitRpc(int mode) { t(mode == 0 ? "initrpc" : "bad-initrpc"); }
int DIntr() { t("dintr"); std::memcpy(poff_at_dintr, _sif_FsPoff_Data, 8); return dintr_result; }
int EIntr() { t("eintr"); return 0; }
void sceSifAddCmdHandler(int id, void* h, void* a) { t("addcmd"); handler_ids[handler_count] = id; handler_fn[handler_count] = h; handler_arg[handler_count++] = a; }
int sceSifBindRpc(void* c, unsigned server, unsigned mode) {
    ++bind_calls; t("bind");
    if (c != _cd || server != 0x80000001u || mode != 0) return -5;
    if (bind_calls == bind_negative_at) return -1;
    reinterpret_cast<int*>(_cd)[9] = (bind_calls > bind_server_after) ? 1 : 0;
    return 0;
}
int WaitSema(int s) { t(s == 77 ? "wait" : "bad-wait"); return 0; }
int SignalSema(int s) { t(s == 77 ? "signal" : "bad-signal"); return 0; }
void _sceFsIobSemaMK() { t("semamk"); }
void _sceFs_Rcv_Intr() {} void _sceFs_Poff_Intr() {}
int sceSifCallRpc(void* client, unsigned f, unsigned m, void* send, int ss, void* recv, int rs, void* end, void* arg) {
    t("rpc");
    client_seen = client; function_seen = f; mode_seen = m; send_seen = send; send_len = ss; recv_seen = recv; recv_len = rs; end_seen = end; end_arg = arg;
    std::memcpy(sent, send, 8);
    std::memcpy(_rcv_data_rpc, reply, 8);
    return rpc_result;
}
}
static void reset() {
    trace.clear(); handler_count = 0; bind_calls = 0; bind_negative_at = -1; bind_server_after = 1; dintr_result = 1; rpc_result = 0;
    std::memset(_iob, 0xff, sizeof _iob); std::memset(_sif_FsPoff_Data, 0x77, 64); std::memset(_fsversion, 0, 4);
    _fs_init = 0; _fs_rcv_bufdbl = 5; reply[0] = 0x01020304u; reply[1] = 2; _cd[36] = 0;
}
int main() {
    reset();
    if (sceFsInit() != 0) return 1;
    if (trace != "initrpc;dintr;addcmd;addcmd;eintr;bind;bind;semamk;wait;signal;rpc;") return 2;
    if (poff_at_dintr[0] != 0 || poff_at_dintr[1] != 0) return 3;
    if (handler_ids[0] != static_cast<int>(0x80000011u) || handler_arg[0] != _sif_FsRcv_Data || handler_fn[0] != reinterpret_cast<void*>(_sceFs_Rcv_Intr) ||
        handler_ids[1] != static_cast<int>(0x80000013u) || handler_arg[1] != _sif_FsPoff_Data || handler_fn[1] != reinterpret_cast<void*>(_sceFs_Poff_Intr)) return 4;
    for (int i = 0; i < 32; ++i) {
        const unsigned* slot = reinterpret_cast<const unsigned*>(_iob + i * 16);
        if (slot[0] != 0xffffffffu || slot[1] != 0 || slot[2] != 0xffffffffu || slot[3] != 0xffffffffu) return 5;
    }
    const unsigned cmd = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_rcv_data_cmd));
    if (sent[0] != cmd || sent[1] != cmd + 0x440 || send_seen != norrath_fs_rcv_adr_view || send_len != 8) return 6;
    if (client_seen != _cd || function_seen != 255 || mode_seen != 0 || recv_seen != _rcv_data_rpc || recv_len != 8 || end_seen || end_arg) return 7;
    if (_fsversion[0] != 4 || _fsversion[1] != 3 || _fsversion[2] != 2 || _fsversion[3] != 1) return 8;
    if (_fs_init != 1 || _fs_rcv_bufdbl != 1) return 9;
    // interrupts were not enabled before: no EIntr; second word not 2 clears the flag
    reset(); dintr_result = 0; reply[1] = 3; bind_server_after = 0;
    if (sceFsInit() != 0 || trace.find("eintr") != std::string::npos || _fs_rcv_bufdbl != 0 || bind_calls != 1) return 10;
    // many empty bind answers are retried without limit
    reset(); bind_server_after = 40;
    if (sceFsInit() != 0 || bind_calls != 41) return 11;
    // bind error returns -1 before the semaphore work
    reset(); bind_negative_at = 2;
    if (sceFsInit() != -1 || trace.find("semamk") != std::string::npos || _fs_init != 0) return 12;
    // rpc failure
    reset(); rpc_result = -3;
    if (sceFsInit() != static_cast<int>(0xfffeffffu) || _fs_init != 0 || _fsversion[0] != 0 || _fs_rcv_bufdbl != 5) return 13;
    std::puts("sceFsInit contract passed");
    return 0;
}
