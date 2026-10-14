// Host check, not a plan input.
#include <cstdio>
#include <cstring>
#include <cstdint>
#define NORRATH_HOST_CONTRACT 1
static unsigned char arena[16];
extern "C" void* norrath_host_buffer(unsigned a) { return arena + a; }
#include "close.cpp"
static int printf_calls, close_calls, seen_h, seen_f, close_ret; static void* seen_ctx; static void* seen_buf;
extern "C" int printf(const char* f, ...) { if (std::strcmp(f, "Socket close\n")) return 99; ++printf_calls; return 0; }
extern "C" int sceInetClose(void* c, void* b, int h, int f) { ++close_calls; seen_ctx = c; seen_buf = b; seen_h = h; seen_f = f; return close_ret; }
int main() {
    unsigned obj[20];
    std::memset(obj, 0, sizeof obj);
    obj[1] = 3; obj[17] = 5; close_ret = 42;
    if (Close__11VIUDPSocket(obj) != 42 || printf_calls != 1 || close_calls != 1) return 1;
    if (seen_ctx != obj + 2 || seen_buf != arena + 3 || seen_h != 5 || seen_f != 0 || obj[17] != 0xffffffffu) return 2;
    if (Close__11VIUDPSocket(obj) != -1 || printf_calls != 2 || close_calls != 1) return 3;
    obj[17] = 0; close_ret = -7;
    if (Close__11VIUDPSocket(obj) != -7 || close_calls != 2 || seen_h != 0 || obj[17] != 0xffffffffu) return 4;
    std::puts("udp socket close check passed");
    return 0;
}
