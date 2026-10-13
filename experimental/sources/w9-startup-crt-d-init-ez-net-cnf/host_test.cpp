// Host comparison only. Not an EE build input.
#include <cstdio>
#include <cstring>
#define printf norrath_host_printf
#include "init_ez_net_cnf.cpp"
#undef printf

static int search_calls, bind_calls, print_calls;
static int search_result;
static int bind_fail_until, answer_after;
static char first_print[80], second_print[80];
static const char* searched;
static void* bound_client;
static unsigned bound_server;
static int bound_mode;

extern "C" int sceSifSearchModuleByName(const char* name) { ++search_calls; searched = name; return search_result; }
extern "C" int sceSifBindRpc(void* client, unsigned server, int mode) {
    ++bind_calls; bound_client = client; bound_server = server; bound_mode = mode;
    if (bind_calls >= answer_after) static_cast<unsigned*>(client)[9] = 1;
    return bind_calls < bind_fail_until ? -1 : 0;
}
extern "C" int norrath_host_printf(const char* format, ...) {
    __builtin_va_list ap; __builtin_va_start(ap, format);
    char* out = print_calls == 0 ? first_print : second_print;
    ++print_calls;
    std::vsnprintf(out, 80, format, ap);
    __builtin_va_end(ap);
    return 0;
}

int main() {
    search_result = 5; answer_after = 1; bind_fail_until = 0;
    initEzNetCnf();
    if (search_calls != 1 || std::strcmp(searched, "eznetcnf") != 0) return 1;
    if (bind_calls != 1 || bound_client != m_rpcCd || bound_server != 0x75499128u || bound_mode != 0) return 2;
    if (m_rpcRunning != 1 || print_calls != 0) return 3;
    m_rpcRunning = 0; m_rpcCd[9] = 0; bind_calls = 0; search_calls = 0;
    search_result = -7;
    initEzNetCnf();
    if (print_calls != 1 || std::strcmp(first_print, "\"eznetcnf\" module not found (-7)\n") != 0) return 4;
    if (bind_calls != 1 || m_rpcRunning != 1) return 5;
    m_rpcRunning = 0; m_rpcCd[9] = 0; bind_calls = 0; print_calls = 0;
    search_result = 0; answer_after = 4; bind_fail_until = 3;
    initEzNetCnf();
    if (bind_calls != 4 || print_calls != 2 || m_rpcRunning != 1) return 6;
    if (std::strcmp(first_print, "could not bind to eznetcnf service\n") != 0) return 7;
    std::puts("initEzNetCnf passed");
    return 0;
}
