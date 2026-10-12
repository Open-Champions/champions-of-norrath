#include <cstdarg>
#include <cstdio>
#include <cstring>
extern "C" {
unsigned char norrath_libpad_is_init_word[4] = {};
unsigned char norrath_libpad_is_warning_word[4] = {1, 0, 0, 0};
unsigned char norrath_libpad_rpc_clients_view[80] = {};
}
static int binds = 0;
static unsigned bind_ids[16];
static void* bind_clients[16];
static int first_ready_at = 3, second_ready_at = 2;
static int version = 0x0400 | 7;
static int init2_calls = 0, init2_mode = 0;
static char text[2][128];
static int prints = 0;
extern "C" int sceSifBindRpc(void* client, unsigned id, unsigned mode) {
    if (mode != 0) return -99;
    bind_ids[binds] = id;
    bind_clients[binds] = client;
    binds += 1;
    unsigned char* bytes = static_cast<unsigned char*>(client);
    if (id == 0x80000100u && binds >= first_ready_at) bytes[36] = 1;
    if (id == 0x80000101u && binds >= first_ready_at + second_ready_at) bytes[36] = 1;
    return 0;
}
extern "C" int scePadGetModVersion() { return version; }
extern "C" int scePadInit2(int mode) { init2_calls += 1; init2_mode = mode; return 0x1234; }
extern "C" int scePrintf(const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    if (prints < 2) std::vsnprintf(text[prints], 128, format, args);
    va_end(args);
    prints += 1;
    return 0;
}
extern "C" int scePadInit(int mode);
int main() {
    int got = scePadInit(5);
    if (got != 0x1234 || init2_calls != 1 || init2_mode != 5) return 1;
    if (norrath_libpad_is_init_word[0] != 1) return 2;
    if (binds != 5) return 3;
    for (int i = 0; i < 3; ++i) if (bind_ids[i] != 0x80000100u || bind_clients[i] != norrath_libpad_rpc_clients_view) return 4;
    for (int i = 3; i < 5; ++i) if (bind_ids[i] != 0x80000101u || bind_clients[i] != norrath_libpad_rpc_clients_view + 40) return 5;
    if (prints != 0) return 6;
    // mismatch with warning on
    std::memset(norrath_libpad_rpc_clients_view, 0, 80);
    binds = 0;
    version = 0x0301 + 0x0e;
    init2_calls = 0;
    got = scePadInit(0);
    if (got != 0 || init2_calls != 0 || prints != 2) return 7;
    if (std::strcmp(text[0], "libpad: Module version mismatch ") != 0) return 8;
    if (std::strcmp(text[1], "[libpad.a = 4.0, padman.irx = 3.15]\n") != 0) return 9;
    // mismatch with warning off
    std::memset(norrath_libpad_rpc_clients_view, 0, 80);
    norrath_libpad_is_warning_word[0] = 0;
    prints = 0; binds = 0;
    if (scePadInit(0) != 0 || prints != 0) return 10;
    std::puts("pad init contract passed");
    return 0;
}
