#include <cstdio>
#define printf norrath_host_printf
#include "init_ez_net_ctl.cpp"
#undef printf

#include <cstring>

static int search_calls, bind_calls, print_calls;
static int search_result;
static int bind_fail_until;      // binds below this count return -1 and leave the word clear
static int answer_after;         // bind number on which the server word becomes nonzero
static char first_print[80], second_print[80];
static const char* searched;
static void* bound_client;
static unsigned bound_server;
static int bound_mode;

extern "C" int sceSifSearchModuleByName(const char* name) {
    ++search_calls;
    searched = name;
    return search_result;
}
extern "C" int sceSifBindRpc(void* client, unsigned server, int mode) {
    ++bind_calls;
    bound_client = client;
    bound_server = server;
    bound_mode = mode;
    if (bind_calls >= answer_after) static_cast<unsigned*>(client)[9] = 1;
    return bind_calls < bind_fail_until ? -1 : 0;
}
extern "C" int norrath_host_printf(const char* format, ...) {
    __builtin_va_list ap;
    __builtin_va_start(ap, format);
    char* out = print_calls == 0 ? first_print : second_print;
    ++print_calls;
    std::vsnprintf(out, 80, format, ap);
    __builtin_va_end(ap);
    return 0;
}

int main() {
    // Everything works first time.
    search_result = 5; answer_after = 1; bind_fail_until = 0;
    initEzNetCtl();
    if (search_calls != 1 || std::strcmp(searched, "eznetctl") != 0) return 1;
    if (bind_calls != 1 || bound_client != m_cd || bound_server != 0x75488909u || bound_mode != 0) return 2;
    if (m_rpcRunning != 1 || print_calls != 0) return 3;

    // Missing module is reported but the bind still happens.
    m_rpcRunning = 0; m_cd[9] = 0; bind_calls = 0; search_calls = 0;
    search_result = -7;
    initEzNetCtl();
    if (print_calls != 1 || std::strcmp(first_print, "\"eznetctl\" module not found (-7)\n") != 0) return 4;
    if (bind_calls != 1 || m_rpcRunning != 1) return 5;

    // Server answers on the fourth bind; the first two binds also report failure.
    m_rpcRunning = 0; m_cd[9] = 0; bind_calls = 0; print_calls = 0;
    search_result = 0; answer_after = 4; bind_fail_until = 3;
    initEzNetCtl();
    if (bind_calls != 4 || print_calls != 2 || m_rpcRunning != 1) return 6;
    if (std::strcmp(first_print, "could not bind to eznetctl service\n") != 0) return 7;
    std::puts("initEzNetCtl passed");
    return 0;
}
