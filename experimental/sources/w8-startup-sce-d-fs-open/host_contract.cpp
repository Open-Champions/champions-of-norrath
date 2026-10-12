#define NORRATH_HOST_CONTRACT 1
#include "fs_open.cpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

int _fs_init;
int _fs_iob_semid = 77;
alignas(16) unsigned char _iob[16 * 8];
alignas(16) unsigned char _send_data[1048 + 16];
alignas(16) unsigned char _rcv_data_rpc[16];
alignas(16) unsigned char _cd[40];

static std::string trace;
static void t(const char* s) { trace += s; trace += ';'; }
static volatile int* result_slot;
static int version_result, rpc_result = 0, reply_word = 1, rpc_server_result = 5, init_calls, waits_on_iob;
static unsigned char* next_iob;
static unsigned wait_flag_seen = 99;
static int send_len_seen, recv_len_seen, mode_seen;
static void *client_seen, *send_seen, *recv_seen, *end_seen, *end_arg_seen;
static unsigned function_seen;
static int param_words[6];
static unsigned char send_copy[1064];
static int created = 9;

void norrath_host_note_result(volatile int* result) { result_slot = result; }

extern "C" {
unsigned char* new_iob() { t("new_iob"); return next_iob; }
int _sceFsWaitS(int flag) { t("wait"); wait_flag_seen = static_cast<unsigned>(flag); return 0; }
void _sceFsSigSema() { t("sig"); }
int _fs_version() { t("version"); return version_result; }
int sceFsInit() { t("init"); init_calls += 1; _fs_init = 1; return 0; }
int CreateSema(void* parameters) {
    t("create");
    std::memcpy(param_words, parameters, sizeof param_words);
    return created;
}
int DeleteSema(int s) { t("delete"); if (s != created) t("bad-del"); return 0; }
int WaitSema(int s) { t(s == created ? "waitsema" : (s == 77 ? "waitiob" : "bad-wait")); if (s == 77) waits_on_iob += 1; return 0; }
int SignalSema(int s) { t(s == 77 ? "signaliob" : "bad-signal"); return 0; }
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void* end_function, void* end_parameter) {
    t("rpc");
    client_seen = client; function_seen = function; mode_seen = static_cast<int>(mode); send_seen = send;
    send_len_seen = send_size; recv_seen = receive; recv_len_seen = receive_size;
    end_seen = end_function; end_arg_seen = end_parameter;
    std::memcpy(send_copy, send, 1048);
    *result_slot = rpc_server_result;
    _rcv_data_rpc[0] = static_cast<unsigned char>(reply_word);
    return rpc_result;
}
}

static unsigned w(const unsigned char* p) { return load_word(p); }

static void reset() {
    trace.clear();
    std::memset(_send_data, 0xcc, sizeof _send_data);
    std::memset(_iob, 0, sizeof _iob);
    next_iob = _iob + 16 * 3;
    version_result = 0; rpc_result = 0; reply_word = 1; rpc_server_result = 5; _fs_init = 1;
    init_calls = 0; waits_on_iob = 0;
}

int main() {
    /* success */
    reset();
    int index = sceOpen("host0:file.bin", 0x80000011, 0x1a4);
    if (index != 3) return 1;
    if (trace != "wait;version;new_iob;create;rpc;sig;waitsema;delete;waitiob;signaliob;") return 2;
    if (wait_flag_seen != 0 || init_calls != 0) return 3;
    if (client_seen != _cd || function_seen != 0 || mode_seen != 0 || send_seen != _send_data || send_len_seen != 1048 ||
        recv_seen != _rcv_data_rpc || recv_len_seen != 4 || end_seen || end_arg_seen)
        return 4;
    if (std::strcmp(reinterpret_cast<const char*>(send_copy + 20), "host0:file.bin") != 0) return 5;
    if (w(send_copy + 12) != 0x00000011u || w(send_copy + 16) != 0x1a4 || w(send_copy + 1044) != 3 || w(send_copy + 8) != 4 ||
        w(send_copy) != 9u)
        return 6;
    if (param_words[1] != 1 || param_words[2] != 0 || param_words[5] != 0) return 7;
    if (w(_iob + 48) != 5u || w(_iob + 52) != 0x80000011u) return 8; /* flags are stored unmasked */
    /* lazy init */
    reset(); _fs_init = 0;
    if (sceOpen("a", 1, 0) != 3 || init_calls != 1 || trace.compare(0, 10, "wait;init;") != 0) return 9;
    /* version mismatch */
    reset(); version_result = 3;
    if (sceOpen("a", 1, 0) != static_cast<int>(0xfffefffc) || trace != "wait;version;sig;") return 10;
    /* no slot */
    reset(); next_iob = nullptr;
    if (sceOpen("a", 1, 0) != -19 || trace != "wait;version;new_iob;sig;") return 11;
    /* rpc failure */
    reset(); rpc_result = -1;
    if (sceOpen("a", 1, 0) != -11 || trace != "wait;version;new_iob;create;rpc;delete;sig;") return 12;
    /* zero reply */
    reset(); reply_word = 0;
    if (sceOpen("a", 1, 0) != -11 || trace != "wait;version;new_iob;create;rpc;sig;delete;") return 13;
    /* negative server result clears the slot flags and returns the server result */
    reset(); rpc_server_result = -2;
    std::memset(_iob + 48, 0x11, 16);
    if (sceOpen("a", 3, 0) != -2 || trace != "wait;version;new_iob;create;rpc;sig;waitsema;delete;waitiob;signaliob;") return 14;
    if (w(_iob + 52) != 0 || w(_iob + 48) != 0x11111111u) return 15;
    /* empty name */
    reset();
    if (sceOpen("", 0, 0) != 3 || send_copy[20] != 0) return 16;
    /* long name: 1023 characters are copied and the last is forced to zero */
    reset();
    std::string longname(2000, 'x');
    if (sceOpen(longname.c_str(), 0, 0) != 3) return 17;
    for (int i = 0; i < 1023; ++i) if (send_copy[20 + i] != (i == 1023 ? 0 : 'x')) return 18;
    if (send_copy[20 + 1023] != 0) return 19;
    /* exactly 1023 characters plus NUL fits without the forced zero */
    reset();
    std::string name1023(1023, 'y');
    if (sceOpen(name1023.c_str(), 0, 0) != 3 || send_copy[20 + 1022] != 'y' || send_copy[20 + 1023] != 0) return 20;
    std::puts("fs open contract passed");
    return 0;
}
