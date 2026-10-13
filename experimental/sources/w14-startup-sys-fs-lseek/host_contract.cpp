// Host contract for sceLseek. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// The library helpers are scripted; the trace records the call order. Mirrors the contract of sceRead.
#define NORRATH_HOST_CONTRACT 1
#include "fs_lseek.cpp"

#include <cstdio>
#include <cstring>
#include <string>

int _fs_init;
int _fs_fsq_semid = 88;
int _sceFs_q[32];
alignas(16) unsigned char _iob[16 * 8];
alignas(16) unsigned char _send_data[64];
alignas(16) unsigned char _rcv_data_rpc[16];
alignas(16) unsigned char _cd[40];

static std::string trace;
static void t(const char* s) { trace += s; trace += ';'; }
static volatile int* result_slot;
static int rpc_result, reply_word, server_result, created = 9;
static unsigned char* found_iob;
static unsigned descriptor_seen, wait_flag_seen;
static int param_words[6];
static unsigned char send_copy[64];
static void *client_seen, *send_seen, *recv_seen, *end_seen, *arg_seen;
static unsigned function_seen, mode_seen;
static int send_len, recv_len;
static int q_at_rpc[32];
static int writebacks;

void norrath_host_note_result(volatile int* result) { result_slot = result; }

extern "C" {
unsigned char* get_iob(unsigned d) { t("get_iob"); descriptor_seen = d; return found_iob; }
int _sceFsWaitS(int flag) { t("wait"); wait_flag_seen = static_cast<unsigned>(flag); return 0; }
void _sceFsSigSema() { t("sig"); }
int CreateSema(void* p) { t("create"); std::memcpy(param_words, p, sizeof param_words); return created; }
int DeleteSema(int s) { t("delete"); if (s != created) t("bad-del"); return 0; }
int WaitSema(int s) { t(s == created ? "waitsema" : (s == 88 ? "waitq" : "bad-wait")); return 0; }
int SignalSema(int s) { t(s == 88 ? "signalq" : "bad-signal"); return 0; }
void sceSifWriteBackDCache(void*, int) { t("writeback"); ++writebacks; }
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* end_function, void* end_parameter) {
    t("rpc");
    client_seen = client; function_seen = function; mode_seen = mode; send_seen = send; send_len = send_size;
    recv_seen = receive; recv_len = receive_size; end_seen = end_function; arg_seen = end_parameter;
    std::memcpy(send_copy, send, 32);
    std::memcpy(q_at_rpc, _sceFs_q, sizeof q_at_rpc);
    *result_slot = server_result;
    _rcv_data_rpc[0] = static_cast<unsigned char>(reply_word);
    return rpc_result;
}
}

static unsigned w(const unsigned char* p) { return load_word(p); }

static void reset(unsigned flags) {
    trace.clear();
    writebacks = 0;
    std::memset(_send_data, 0xcc, sizeof _send_data);
    std::memset(_iob, 0, sizeof _iob);
    found_iob = _iob + 16 * 2;
    store_word(found_iob, 0x1234u);
    store_word(found_iob + 4, flags);
    for (int& e : _sceFs_q) e = -1;
    _fs_init = 1; rpc_result = 0; reply_word = 1; server_result = 321;
}

int main() {
    /* synchronous seek */
    reset(0x1);
    if (sceLseek(2, -100, 2) != 321) return 1;
    if (trace != "get_iob;wait;create;rpc;sig;waitsema;delete;") return 2;
    if (descriptor_seen != 2 || wait_flag_seen != 4) return 3;
    if (client_seen != _cd || function_seen != 4 || mode_seen != 0 || send_seen != _send_data || send_len != 28 ||
        recv_seen != _rcv_data_rpc || recv_len != 4 || end_seen || arg_seen)
        return 5;
    if (w(send_copy) != 9u || w(send_copy + 8) != 4u || w(send_copy + 12) != 0x1234u ||
        static_cast<int>(w(send_copy + 16)) != -100 || w(send_copy + 20) != 2u || w(send_copy + 24) != 2u)
        return 6;
    if (param_words[1] != 1 || param_words[2] != 0 || param_words[5] != 0) return 7;
    for (int e : q_at_rpc) if (e != -1) return 8; /* queue untouched without the async flag */
    /* no cache write-back is made */
    if (writebacks != 0) return 9;
    /* async: queue entry, negated id, result 0 */
    reset(0x8000u);
    _sceFs_q[0] = 4; _sceFs_q[1] = 6;
    if (sceLseek(2, 0, 0) != 0) return 11;
    if (trace != "get_iob;wait;create;waitq;signalq;rpc;sig;delete;") return 12;
    if (q_at_rpc[0] != 4 || q_at_rpc[1] != 6 || q_at_rpc[2] != 9) return 13;
    if (static_cast<int>(w(send_copy)) != -9) return 14;
    /* async with a full queue leaves the id positive */
    reset(0x8000u);
    for (int& e : _sceFs_q) e = 7;
    if (sceLseek(2, 0, 0) != 0 || static_cast<int>(w(send_copy)) != 9) return 15;
    /* last entry */
    reset(0x8000u);
    for (int i = 0; i < 31; ++i) _sceFs_q[i] = i;
    if (sceLseek(2, 0, 0) != 0 || q_at_rpc[31] != 9) return 17;
    /* uninitialised library */
    reset(0x1); _fs_init = 0;
    if (sceLseek(2, 0, 0) != -1 || trace != "get_iob;wait;sig;") return 18;
    /* null slot, or slot with zero flags */
    reset(0x1); found_iob = nullptr;
    if (sceLseek(2, 0, 0) != -9 || trace != "get_iob;wait;sig;") return 19;
    reset(0x0);
    if (sceLseek(2, 0, 0) != -9 || trace != "get_iob;wait;sig;") return 20;
    /* rpc failure */
    reset(0x1); rpc_result = -1;
    if (sceLseek(2, 0, 0) != -11 || trace != "get_iob;wait;create;rpc;delete;sig;") return 21;
    /* zero reply */
    reset(0x1); reply_word = 0;
    if (sceLseek(2, 0, 0) != -11 || trace != "get_iob;wait;create;rpc;sig;delete;") return 22;
    /* negative server result is returned as is */
    reset(0x1); server_result = -5;
    if (sceLseek(2, 0, 0) != -5) return 23;
    std::puts("fs lseek contract passed");
    return 0;
}
