#include <cstdio>
#include "st_stop.cpp"
extern "C" int stm_status = 9;
extern "C" alignas(4) unsigned char dum_mode[40] = {};
static unsigned a0 = 1, a1 = 1, cmd; static void* a2 = &a0; static const unsigned char* m; static int status_seen;
extern "C" unsigned sceCdStream(unsigned x, unsigned y, void* z, unsigned c, const unsigned char* mode) {
    a0 = x; a1 = y; a2 = z; cmd = c; m = mode; status_seen = stm_status; return 0x77u;
}
int main() {
    if (sceCdStStop() != 0x77u) return 1;
    if (a0 != 0 || a1 != 0 || a2 != nullptr || cmd != 3 || m != dum_mode) return 2;
    if (status_seen != 0 || stm_status != 0) return 3;
    std::puts("sceCdStStop contract passed");
    return 0;
}
