#include <cstdio>
#include <cstring>
#include "st_pause.cpp"
extern "C" int SCE_CD_debug = 0;
extern "C" int stm_status = 9;
extern "C" alignas(4) unsigned char dum_mode[40] = {};
static unsigned a0 = 1, a1 = 1, cmd; static void* a2 = &a0; static const unsigned char* m; static int status_seen;
static int prints; static char last_text[64]; static int status_at_print = -1;
extern "C" int scePrintf(const char* format, ...) {
    ++prints; std::strncpy(last_text, format, 63); status_at_print = stm_status; return 0;
}
extern "C" unsigned sceCdStream(unsigned x, unsigned y, void* z, unsigned c, const unsigned char* mode) {
    a0 = x; a1 = y; a2 = z; cmd = c; m = mode; status_seen = stm_status; return 0x55u;
}
int main() {
    if (sceCdStPause() != 0x55u || prints != 0 || stm_status != 0) return 1;
    if (a0 != 0 || a1 != 0 || a2 != nullptr || cmd != 7 || m != dum_mode) return 2;
    stm_status = 4; SCE_CD_debug = 1;
    sceCdStPause();
    if (prints != 1 || std::strcmp(last_text, "sceCdStPause call\n") != 0) return 3;
    if (status_at_print != 0 || stm_status != 0) return 4;
    SCE_CD_debug = -1; stm_status = 2;
    sceCdStPause();
    if (prints != 1 || stm_status != 0) return 5;
    std::puts("sceCdStPause contract passed");
    return 0;
}
