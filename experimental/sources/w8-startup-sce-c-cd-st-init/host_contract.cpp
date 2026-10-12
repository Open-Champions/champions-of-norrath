#include <cstdio>
#include "st_init.cpp"
extern "C" int stm_status = 9;
extern "C" alignas(4) unsigned char dum_mode[40];
static unsigned a0, a1, cmd; static void* a2; static const unsigned char* m; static int status_seen;
extern "C" unsigned sceCdStream(unsigned x, unsigned y, void* z, unsigned c, const unsigned char* mode) {
    a0 = x; a1 = y; a2 = z; cmd = c; m = mode; status_seen = stm_status; return 0xabcu;
}
int main() {
    int buf;
    if (sceCdStInit(3, 4, &buf) != 0xabcu) return 1;
    if (a0 != 3 || a1 != 4 || a2 != &buf || cmd != 5 || m != dum_mode) return 2;
    if (status_seen != 0 || stm_status != 0) return 3;
    stm_status = 4;
    sceCdStInit(0, 0, nullptr);
    if (stm_status != 0 || a2 != nullptr) return 4;
    std::puts("sceCdStInit contract passed");
    return 0;
}
