#define NORRATH_HOST_CONTRACT 1
#include "fs_read.cpp"

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
static void* wb_addr[2];
static int wb_size[2], wbs;
static int q_at_rpc[32];

void norrath_host_note_result(volatile int* result) { result_slot = result; }

extern "C" {
unsigned char* get_iob(unsigned d) { t("get_iob"); descriptor_seen = d; return found_iob; }
int _sceFsWaitS(int flag) { t("wait"); wait_flag_seen = static_cast<unsigned>(flag); return 0; }
void _sceFsSigSema() { t("sig"); }
int CreateSema(void* p) { t("create"); std::memcpy(param_words, p, sizeof param_words); return created; }
int DeleteSema(int s) { t("delete"); if (s != created) t("bad-del"); return 0; }
int WaitSema(int s) { t(s == created ? "waitsema" : (s == 88 ? "waitq" : "bad-wait")); return 0; }
int SignalSema(int s) { t(s == 88 ? "signalq" : "bad-signal"); return 0; }
void sceSifWriteBackDCache(void* a, int s) { t("writeback"); if (wbs < 2) { wb_addr[wbs] = a; wb_size[wbs] = s; } wbs += 1; }
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
    wbs = 0;
    std::memset(_send_data, 0xcc, sizeof _send_data);
    std::memset(_iob, 0, sizeof _iob);
    found_iob = _iob + 16 * 2;
    store_word(found_iob, 0x1234u);
    store_word(found_iob + 4, flags);
    for (int& e : _sceFs_q) e = -1;
    _fs_init = 1; rpc_result = 0; reply_word = 1; server_result = 321;
}

int main() {
    char buffer[64];
    /* synchronous read */
    reset(0x1);
    if (sceRead(2, buffer, 64) != 321) return 1;
    if (trace != "get_iob;wait;create;writeback;writeback;rpc;sig;waitsema;delete;") return 2;
    if (descriptor_seen != 2 || wait_flag_seen != 2) return 3;
    if (wb_addr[0] != buffer || wb_size[0] != 64 || wb_addr[1] != _send_data || wb_size[1] != 32) return 4;
    if (client_seen != _cd || function_seen != 2 || mode_seen != 0 || send_seen != _send_data || send_len != 32 ||
        recv_seen != _rcv_data_rpc || recv_len != 4 || end_seen || arg_seen)
        return 5;
    if (w(send_copy) != 9u || w(send_copy + 8) != 4u || w(send_copy + 12) != 0x1234u ||
        w(send_copy + 16) != pointer_bits(buffer) || w(send_copy + 20) != 64u || w(send_copy + 28) != 2u)
        return 6;
    if (param_words[1] != 1 || param_words[2] != 0 || param_words[5] != 0) return 7;
    for (int e : q_at_rpc) if (e != -1) return 8; /* queue untouched without the async flag */
    /* no write-back when the flag is set */
    reset(0x20000001u);
    if (sceRead(2, buffer, 64) != 321 || trace != "get_iob;wait;create;writeback;rpc;sig;waitsema;delete;") return 9;
    if (wb_addr[0] != _send_data) return 10;
    /* async read: queue entry, negated id, result 0 */
    reset(0x8000u);
    _sceFs_q[0] = 4; _sceFs_q[1] = 6;
    if (sceRead(2, buffer, 16) != 0) return 11;
    if (trace != "get_iob;wait;create;waitq;signalq;writeback;writeback;rpc;sig;delete;") return 12;
    if (q_at_rpc[0] != 4 || q_at_rpc[1] != 6 || q_at_rpc[2] != 9) return 13;
    if (static_cast<int>(w(send_copy)) != -9) return 14;
    /* async with a full queue leaves the id positive */
    reset(0x8000u);
    for (int& e : _sceFs_q) e = 7;
    if (sceRead(2, buffer, 16) != 0 || static_cast<int>(w(send_copy)) != 9) return 15;
    /* async: first free entry is entry 0 */
    reset(0x8000u);
    if (sceRead(2, buffer, 16) != 0 || q_at_rpc[0] != 9 || q_at_rpc[1] != -1 || static_cast<int>(w(send_copy)) != -9) return 16;
    /* last entry */
    reset(0x8000u);
    for (int i = 0; i < 31; ++i) _sceFs_q[i] = i;
    if (sceRead(2, buffer, 16) != 0 || q_at_rpc[31] != 9) return 17;
    /* uninitialised library */
    reset(0x1); _fs_init = 0;
    if (sceRead(2, buffer, 4) != -1 || trace != "get_iob;wait;sig;") return 18;
    /* null slot, or slot with zero flags */
    reset(0x1); found_iob = nullptr;
    if (sceRead(2, buffer, 4) != -9 || trace != "get_iob;wait;sig;") return 19;
    reset(0x0);
    if (sceRead(2, buffer, 4) != -9 || trace != "get_iob;wait;sig;") return 20;
    /* rpc failure */
    reset(0x1); rpc_result = -1;
    if (sceRead(2, buffer, 4) != -11 || trace != "get_iob;wait;create;writeback;writeback;rpc;delete;sig;") return 21;
    /* zero reply */
    reset(0x1); reply_word = 0;
    if (sceRead(2, buffer, 4) != -11 || trace != "get_iob;wait;create;writeback;writeback;rpc;sig;delete;") return 22;
    /* negative server result is returned as is */
    reset(0x1); server_result = -5;
    if (sceRead(2, buffer, 4) != -5) return 23;
    std::puts("fs read contract passed");
    return 0;
}
