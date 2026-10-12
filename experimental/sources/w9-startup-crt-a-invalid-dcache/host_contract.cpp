#define NORRATH_HOST_CONTRACT 1
#include "invalid_dcache.cpp"

#include <cstdio>
#include <cstring>

extern "C" unsigned norrath_host_status_word = 0;
static char log_text[8];
static int log_used;
static unsigned seen_first;
static unsigned seen_second;

static void mark(char step) {
    if (log_used < 7) log_text[log_used++] = step;
    log_text[log_used] = 0;
}

extern "C" int DIntr() { mark('D'); return 1; }
extern "C" int EIntr() { mark('E'); return 1; }
extern "C" void _sceIDC(unsigned first, unsigned second) {
    mark('I');
    seen_first = first;
    seen_second = second;
}

static void reset(unsigned status) {
    norrath_host_status_word = status;
    log_used = 0;
    log_text[0] = 0;
}

int main() {
    reset(0u);
    InvalidDCache(0x1000u, 0x2000u);
    if (std::strcmp(log_text, "I") != 0 || seen_first != 0x1000u || seen_second != 0x2000u) return 1;
    reset(0x10000u);
    InvalidDCache(0x12345u, 0x987654fu);
    if (std::strcmp(log_text, "DIE") != 0 || seen_first != 0x12340u || seen_second != 0x9876540u) return 2;
    reset(0xfffeffffu);
    InvalidDCache(0x7fu, 0xbfu);
    if (std::strcmp(log_text, "I") != 0 || seen_first != 0x40u || seen_second != 0x80u) return 3;
    reset(0x10000u);
    InvalidDCache(0xffffffffu, 0x3fu);
    if (seen_first != 0xffffffc0u || seen_second != 0u) return 4;
    std::puts("invalid dcache contract passed");
    return 0;
}
