// Host contract for sceWrite. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// The library helpers are scripted; the trace records the call order. Mirrors the contract of sceRead; the message carries the unaligned head of the data.
#define NORRATH_HOST_CONTRACT 1
#include "fs_write.cpp"

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
static void* wb_addr;
static int wb_size;

void norrath_host_note_result(volatile int* result) { result_slot = result; }

extern "C" {
unsigned char* get_iob(unsigned d) { t("get_iob"); descriptor_seen = d; return found_iob; }
int _sceFsWaitS(int flag) { t("wait"); wait_flag_seen = static_cast<unsigned>(flag); return 0; }
void _sceFsSigSema() { t("sig"); }
int CreateSema(void* p) { t("create"); std::memcpy(param_words, p, sizeof param_words); return created; }
int DeleteSema(int s) { t("delete"); if (s != created) t("bad-del"); return 0; }
int WaitSema(int s) { t(s == created ? "waitsema" : (s == 88 ? "waitq" : "bad-wait")); return 0; }
int SignalSema(int s) { t(s == 88 ? "signalq" : "bad-signal"); return 0; }
void sceSifWriteBackDCache(void* a, int n) { t("writeback"); wb_addr = a; wb_size = n; }
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* end_function, void* end_parameter) {
    t("rpc");
    client_seen = client; function_seen = function; mode_seen = mode; send_seen = send; send_len = send_size;
    recv_seen = receive; recv_len = receive_size; end_seen = end_function; arg_seen = end_parameter;
    std::memcpy(send_copy, send, 48);
    std::memcpy(q_at_rpc, _sceFs_q, sizeof q_at_rpc);
    *result_slot = server_result;
    _rcv_data_rpc[0] = static_cast<unsigned char>(reply_word);
    return rpc_result;
}
}

static unsigned w(const unsigned char* p) { return load_word(p); }

static void reset(unsigned flags) {
    trace.clear();
    wb_addr = nullptr; wb_size = 0;
    std::memset(_send_data, 0xcc, sizeof _send_data);
    std::memset(_iob, 0, sizeof _iob);
    found_iob = _iob + 16 * 2;
    store_word(found_iob, 0x1234u);
    store_word(found_iob + 4, flags);
    for (int& e : _sceFs_q) e = -1;
    _fs_init = 1; rpc_result = 0; reply_word = 1; server_result = 321;
}

int main() {
    alignas(16) unsigned char buffer[64];
    for (int i = 0; i < 64; ++i) buffer[i] = static_cast<unsigned char>(0xa0 + i);
    /* synchronous write of an aligned buffer: no head bytes, cache write-back of the buffer */
    reset(0x1);
    if (sceWrite(2, buffer, 40) != 321) return 1;
    if (trace != "get_iob;wait;create;writeback;rpc;sig;waitsema;delete;") return 2;
    if (descriptor_seen != 2 || wait_flag_seen != 3) return 3;
    if (client_seen != _cd || function_seen != 3 || mode_seen != 0 || send_seen != _send_data || send_len != 48 ||
        recv_seen != _rcv_data_rpc || recv_len != 4 || end_seen || arg_seen)
        return 5;
    if (w(send_copy) != 9u || w(send_copy + 8) != 4u || w(send_copy + 12) != 0x1234u ||
        w(send_copy + 16) != pointer_bits(buffer) || w(send_copy + 20) != 40u || w(send_copy + 24) != 0u)
        return 6;
    if (param_words[1] != 1 || param_words[2] != 0 || param_words[5] != 0) return 7;
    for (int e : q_at_rpc) if (e != -1) return 8;
    if (wb_addr != buffer || wb_size != 40) return 9;
    /* unaligned buffers: head up to the next multiple of 16, copied from byte 28 of the message */
    for (int skew = 1; skew < 16; ++skew) {
        reset(0x1);
        if (sceWrite(2, buffer + skew, 40) != 321) return 10;
        if (w(send_copy + 24) != static_cast<unsigned>(16 - ((pointer_bits(buffer) + skew) & 15))) return 11;
        const unsigned head = w(send_copy + 24);
        for (unsigned i = 0; i < head; ++i) if (send_copy[28 + i] != buffer[skew + i]) return 12;
        if (send_copy[28 + head] != 0xcc) return 13; /* nothing copied past the head */
    }
    /* the head never exceeds the size */
    reset(0x1);
    if (sceWrite(2, buffer + 1, 3) != 321 || w(send_copy + 24) != 3u) return 14;
    reset(0x1);
    if (sceWrite(2, buffer + 1, 0) != 321 || w(send_copy + 24) != 0u) return 15;
    /* no write-back when the flag is set */
    reset(0x20000001u);
    if (sceWrite(2, buffer, 64) != 321 || trace != "get_iob;wait;create;rpc;sig;waitsema;delete;") return 16;
    /* async: queue entry, negated id, result 0 */
    reset(0x8000u);
    _sceFs_q[0] = 4; _sceFs_q[1] = 6;
    if (sceWrite(2, buffer, 16) != 0) return 17;
    if (trace != "get_iob;wait;create;waitq;signalq;writeback;rpc;sig;delete;") return 18;
    if (q_at_rpc[0] != 4 || q_at_rpc[1] != 6 || q_at_rpc[2] != 9 || static_cast<int>(w(send_copy)) != -9) return 19;
    /* async with a full queue leaves the id positive */
    reset(0x8000u);
    for (int& e : _sceFs_q) e = 7;
    if (sceWrite(2, buffer, 16) != 0 || static_cast<int>(w(send_copy)) != 9) return 20;
    /* uninitialised library, null slot, zero flags */
    reset(0x1); _fs_init = 0;
    if (sceWrite(2, buffer, 4) != -1 || trace != "get_iob;wait;sig;") return 21;
    reset(0x1); found_iob = nullptr;
    if (sceWrite(2, buffer, 4) != -9 || trace != "get_iob;wait;sig;") return 22;
    reset(0x0);
    if (sceWrite(2, buffer, 4) != -9 || trace != "get_iob;wait;sig;") return 23;
    /* rpc failure and zero reply */
    reset(0x1); rpc_result = -1;
    if (sceWrite(2, buffer, 4) != -11 || trace != "get_iob;wait;create;writeback;rpc;delete;sig;") return 24;
    reset(0x1); reply_word = 0;
    if (sceWrite(2, buffer, 4) != -11 || trace != "get_iob;wait;create;writeback;rpc;sig;delete;") return 25;
    reset(0x1); server_result = -5;
    if (sceWrite(2, buffer, 4) != -5) return 26;
    std::puts("fs write contract passed");
    return 0;
}
