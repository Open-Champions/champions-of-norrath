#include <cstdio>
#include <cstring>
#include <string>

#define NORRATH_STARTUP_HANG() throw 1
#define memset norrath_host_memset
#define strcpy norrath_host_strcpy
#define printf norrath_host_printf
#include "unet_startup_thread.cpp"
#undef memset
#undef strcpy
#undef printf

int sLibNetInitialized;
void* net_buf;
unsigned char cd[60];
char hostname[128];

static std::string trace;
static void* alloc_result;
static int reg_result, wait_result, str_result, name_result;
static unsigned alloc_align, alloc_size;
static int init_first, init_second, init_third, rpc_mode, flag_at_alloc;
static void* addr_seen[3];
static char text_given[] = "10.0.0.7";
static char name_given[] = "peer.local";

void sceSifMInitRpc(int mode) { rpc_mode = mode; trace += 'R'; }
void VIInitUSocketExApi__Fv() { trace += 'U'; }
int sceLibnetInitialize(void* client, int a, int b, int c) {
    trace += 'I';
    if (client != cd) trace += '!';
    init_first = a; init_second = b; init_third = c;
    return 0;
}
void* __wrap_memalign(unsigned alignment, unsigned size) {
    trace += 'M';
    alloc_align = alignment; alloc_size = size; flag_at_alloc = sLibNetInitialized;
    return alloc_result;
}
void pf(const char* m) { trace += "[pf:"; trace += m; trace += ']'; }
int norrath_host_printf(const char* f, ...) {
    trace += "[pr:"; trace += f; trace += ']';
    return 0;
}
void* norrath_host_memset(void* d, int v, unsigned n) {
    unsigned char* p = static_cast<unsigned char*>(d);
    for (unsigned i = 0; i < n; ++i) p[i] = static_cast<unsigned char>(v);
    return d;
}
char* norrath_host_strcpy(char* d, const char* s) { std::strcpy(d, s); trace += 'C'; return d; }
int sceLibnetRegisterHandler(void* client, void* buffer) {
    trace += 'H';
    if (client != cd || buffer != net_buf) trace += '!';
    return reg_result;
}
int sceLibnetWaitGetAddress(void*, void*, void* result, int a, void* address, int b) {
    trace += 'W';
    addr_seen[0] = address;
    unsigned char* p = static_cast<unsigned char*>(address);
    for (int i = 0; i < 16; ++i) if (p[i] != 0) trace += 'z';
    if (a != 1 || b != 2 || result == address) trace += '!';
    return wait_result;
}
int sceInetAddress2String(void*, void*, char* text, int length, void* address) {
    trace += 'S';
    if (length != 128 || address != addr_seen[0]) trace += '!';
    std::strcpy(text, text_given);
    return str_result;
}
int sceInetAddress2Name(void*, void*, void* first, char* name, int length, void* address, int b, int c) {
    trace += 'N';
    if (first != nullptr || name != hostname || length != 128 || address != addr_seen[0] || b != 0 || c != 0) trace += '!';
    if (name_result == 0) std::strcpy(name, name_given);
    return name_result;
}

static std::string run(void* alloc, int reg, int wait, int str, int name) {
    trace.clear();
    alloc_result = alloc; reg_result = reg; wait_result = wait; str_result = str; name_result = name;
    net_buf = nullptr; sLibNetInitialized = 0;
    std::memset(hostname, 0, sizeof hostname);
    try {
        unet_startup_thread__FPv(nullptr);
    } catch (int) {
        trace += "<hang>";
    }
    return trace;
}

int main() {
    int block;
    std::string ok = run(&block, 0, 0, 0, 1);
    if (ok != "RUIMHWSNC[pr:My address: %s\n]") return 1;
    if (std::strcmp(hostname, "10.0.0.7") != 0) return 2;
    if (rpc_mode != 0 || init_first != 2048 || init_second != 8192 || init_third != 32) return 3;
    if (alloc_align != 64 || alloc_size != 0x10200 || flag_at_alloc != 1 || sLibNetInitialized != 1 || net_buf != &block) return 4;
    std::string named = run(&block, 5, 0, 0, 0);
    if (named != "RUIMHWSN[pr:My address: %s (%s)\n]" || std::strcmp(hostname, "peer.local") != 0) return 5;
    if (run(nullptr, 0, 0, 0, 0) != "RUIM[pf:malloc failed.\n]<hang>" || net_buf != nullptr || sLibNetInitialized != 1) return 6;
    if (run(&block, -1, 0, 0, 0) != "RUIMH[pf:reg_handler() failed.\n]<hang>") return 7;
    if (run(&block, 0, -1, 0, 0) != "RUIMHW[pr:wait_get_addr() failed.\n]<hang>") return 8;
    if (run(&block, 0, 0, 3, 0) != "RUIMHWS[pr:invalid addr\n]<hang>") return 9;
    std::puts("unet_startup_thread contract passed");
    return 0;
}
