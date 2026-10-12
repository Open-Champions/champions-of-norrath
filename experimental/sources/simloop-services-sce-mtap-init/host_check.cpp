#include <cstdarg>
#include <cstdio>
#include <cstring>
extern "C" {
unsigned char norrath_libmtap_port_open_client_view[40] = {};
unsigned char norrath_libmtap_port_close_client_view[40] = {};
unsigned char norrath_libmtap_get_connection_client_view[40] = {};
unsigned char norrath_libmtap_change_priority_client_view[40] = {};
unsigned char norrath_libmtap_module_version_client_view[40] = {};
}
static unsigned char* const clients[5] = {norrath_libmtap_port_open_client_view, norrath_libmtap_port_close_client_view,
                                          norrath_libmtap_get_connection_client_view,
                                          norrath_libmtap_change_priority_client_view,
                                          norrath_libmtap_module_version_client_view};
static int init_calls = 0, binds = 0, dprints = 0, prints = 0;
static unsigned order[32];
static int ready_after[5] = {1, 2, 1, 1, 3};
static int attempts[5];
static int fail_on_client = -1;
static int version = 0x0300 | 2;
static char text[2][128], dtext[64];
extern "C" int sceSifInitRpc(unsigned mode) { init_calls += 1; return mode; }
extern "C" int sceSifBindRpc(void* client, unsigned id, unsigned mode) {
    if (mode != 0) return -50;
    int index = -1;
    for (int i = 0; i < 5; ++i) if (client == clients[i]) index = i;
    if (index < 0 || id != 0x80000901u + index) return -50;
    order[binds++] = id;
    attempts[index] += 1;
    if (index == fail_on_client) return -1;
    if (attempts[index] >= ready_after[index]) static_cast<unsigned char*>(client)[36] = 1;
    return 0;
}
extern "C" int sceMtapGetModVersion() { return version; }
extern "C" int scePrintf(const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    if (prints < 2) std::vsnprintf(text[prints], 128, format, args);
    va_end(args);
    prints += 1;
    return 0;
}
extern "C" void DPRINT(const char* format, ...) { dprints += 1; std::snprintf(dtext, sizeof dtext, "%s", format); }
extern "C" int sceMtapInit();
int main() {
    if (sceMtapInit() != 1 || init_calls != 1 || binds != 8 || prints != 0 || dprints != 0) return 1;
    unsigned expected[8] = {0x80000901u, 0x80000902u, 0x80000902u, 0x80000903u, 0x80000904u, 0x80000905u, 0x80000905u, 0x80000905u};
    for (int i = 0; i < 8; ++i) if (order[i] != expected[i]) return 2;
    version = 0x0200 | 9;
    if (sceMtapInit() != 0 || prints != 2) return 3;
    if (std::strcmp(text[0], "libmtap: Module version mismatch ") != 0) return 4;
    if (std::strcmp(text[1], "[libmtap.a = 3.0, mtapman.irx = 2.9]\n") != 0) return 5;
    version = 0x0300;
    binds = 0; fail_on_client = 2;
    for (int i = 0; i < 5; ++i) std::memset(clients[i], 0, 40);
    for (int i = 0; i < 5; ++i) attempts[i] = 0;
    if (sceMtapInit() != 0 || dprints != 1 || std::strcmp(dtext, "libmtap: bind failed\n") != 0) return 6;
    if (binds != 4 || attempts[3] != 0) return 7;
    std::puts("mtap init contract passed");
    return 0;
}
