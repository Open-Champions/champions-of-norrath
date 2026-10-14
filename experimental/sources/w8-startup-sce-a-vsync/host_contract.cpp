#define NORRATH_HOST_CONTRACT 1
#include "vsync.cpp"

#include <cstdio>
#include <cstring>

static unsigned intc;
static int enabled_next = 1;
static int polls;
static int polls_until_set;
static char log_text[24];
static int log_len;
static void mark(char c) { if (log_len < 23) { log_text[log_len++] = c; log_text[log_len] = 0; } }

extern "C" int DIntr() { mark('D'); return enabled_next; }
extern "C" int EIntr() { mark('E'); return 7; }
extern "C" unsigned norrath_host_intc_read() {
    mark('r');
    if (++polls >= polls_until_set) intc |= 4u;
    return intc;
}
extern "C" void norrath_host_intc_write(unsigned value) { mark('w'); mark(static_cast<char>('0' + value)); intc &= ~value; }
extern "C" void norrath_host_pipeline_sync() { mark('Y'); }

int main() {
    intc = 4u; polls = 0; polls_until_set = 3; log_len = 0; log_text[0] = 0; enabled_next = 1;
    if (VSync(99) != 7) return 1;
    if (std::strcmp(log_text, "Dw4YErrrDw4YE") != 0) return 2;
    log_len = 0; log_text[0] = 0; polls = 0; enabled_next = 0; intc = 4u;
    if (VSync(0) != 0) return 3;
    if (std::strcmp(log_text, "Dw4YrrrDw4Y") != 0) return 4;
    std::puts("vsync contract passed");
    return 0;
}
