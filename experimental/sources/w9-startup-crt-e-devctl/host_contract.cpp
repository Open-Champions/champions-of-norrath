#define NORRATH_HOST_CONTRACT 1
#include "devctl.cpp"

#include <cstdio>
#include <cstring>
#include <string>

int _fs_init;
alignas(16) unsigned char _send_data[2080];
alignas(16) unsigned char _rcv_data_rpc[16];
alignas(16) unsigned char _cd[40];

static std::string trace;
static void t(const char* s) { trace += s; trace += ';'; }
static volatile int* result_slot;
static int rpc_result = 0, reply_word = 1, server_result = 5, init_calls, wait_flag = -1, created = 9;
static int param_words[6], wb_size = -1;
static void *client_seen, *send_seen, *recv_seen, *end_seen, *end_arg_seen, *wb_addr;
static unsigned function_seen, mode_seen; static int send_len_seen, recv_len_seen;
static unsigned char send_copy[2080];
static unsigned word_at_wb;

void norrath_host_note_result(volatile int* result) { result_slot = result; }

extern "C" {
int _sceFsWaitS(int flag) { t("wait"); wait_flag = flag; return 0; }
void _sceFsSigSema() { t("sig"); }
int sceFsInit() { t("init"); ++init_calls; _fs_init = 1; return 0; }
int CreateSema(void* parameters) { t("create"); std::memcpy(param_words, parameters, sizeof param_words); return created; }
int DeleteSema(int s) { t(s == created ? "delete" : "bad-del"); return 0; }
int WaitSema(int s) { t(s == created ? "waitsema" : "bad-wait"); return 0; }
void sceSifWriteBackDCache(void* a, int n) { t("wb"); wb_addr = a; wb_size = n; word_at_wb = load_word(_send_data); }
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int ss, void* recv, int rs, void* end, void* arg) {
    t("rpc");
    client_seen = client; function_seen = function; mode_seen = mode; send_seen = send; send_len_seen = ss;
    recv_seen = recv; recv_len_seen = rs; end_seen = end; end_arg_seen = arg;
    std::memcpy(send_copy, send, 2076);
    *result_slot = server_result;
    _rcv_data_rpc[0] = static_cast<unsigned char>(reply_word);
    return rpc_result;
}
}

static unsigned w(const unsigned char* p) { return load_word(p); }
static void reset() {
    trace.clear();
    std::memset(_send_data, 0xcc, sizeof _send_data);
    rpc_result = 0; reply_word = 1; server_result = 5; _fs_init = 1; init_calls = 0;
}

int main() {
    unsigned char out[8];
    const unsigned char in[3] = {7, 8, 9};
    reset();
    if (sceDevctl("host0:", 0x4321, in, 3, out, 8) != 5) return 1;
    if (trace != "wait;create;wb;rpc;sig;waitsema;delete;") return 2;
    if (wait_flag != 23 || init_calls != 0) return 3;
    if (client_seen != _cd || function_seen != 23 || mode_seen != 0 || send_seen != _send_data || send_len_seen != 2076 ||
        recv_seen != _rcv_data_rpc || recv_len_seen != 4 || end_seen || end_arg_seen) return 4;
    if (wb_addr != _send_data || wb_size != 2076 || word_at_wb != 9u) return 5;
    if (std::strcmp(reinterpret_cast<const char*>(send_copy + 12), "host0:") != 0) return 6;
    if (send_copy[1036] != 7 || send_copy[1037] != 8 || send_copy[1038] != 9 || send_copy[1039] != 0xcc) return 7;
    if (w(send_copy + 2060) != 0x4321 || w(send_copy + 2064) != 3 || w(send_copy + 2072) != 8 ||
        w(send_copy + 2068) != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(out)) || w(send_copy) != 9 ||
        w(send_copy + 8) != 4u || w(send_copy + 4) != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(result_slot))) return 8;
    if (param_words[1] != 1 || param_words[2] != 0 || param_words[5] != 0) return 9;
    // sceFsInit is run when the library is not yet initialised
    reset(); _fs_init = 0;
    sceDevctl("a", 1, nullptr, 0, nullptr, 0);
    if (init_calls != 1 || trace.compare(0, 10, "wait;init;") != 0) return 10;
    // oversize lengths release and fail after the name copy
    reset();
    if (sceDevctl("abc", 1, in, 1025, out, 8) != -22 || trace != "wait;sig;") return 11;
    if (std::strcmp(reinterpret_cast<const char*>(_send_data + 12), "abc") != 0) return 12;
    reset();
    if (sceDevctl("abc", 1, in, 8, out, 1025) != -22 || trace != "wait;sig;") return 13;
    reset();
    if (sceDevctl("abc", 1, in, 1024, out, 1024) != 5) return 14;
    // a 1024-byte name is cut at offset 12 + 1023
    reset();
    std::string longname(1100, 'q');
    sceDevctl(longname.c_str(), 1, nullptr, 0, nullptr, 0);
    if (_send_data[12 + 1022] != 'q' || _send_data[12 + 1023] != 0) return 15;
    // failed call
    reset(); rpc_result = -1;
    if (sceDevctl("a", 1, nullptr, 0, nullptr, 0) != -11 || trace != "wait;create;wb;rpc;delete;sig;") return 16;
    // zero reply
    reset(); reply_word = 0;
    if (sceDevctl("a", 1, nullptr, 0, nullptr, 0) != -11 || trace != "wait;create;wb;rpc;sig;delete;") return 17;
    std::puts("sceDevctl contract passed");
    return 0;
}
