#define NORRATH_HOST_CONTRACT 1
#include "vsync2.cpp"

#include <cstdio>
#include <cstring>

static unsigned intc;
static int enabled_next = 1;
static int polls;
static int set_bit_after;      // polls before bit 4 appears, 0 = never
static int set_flag_after;     // polls before the kernel sets the flag, 0 = never
static volatile unsigned* flag_ptr;
static volatile unsigned long long* value_ptr;
static char log_text[32];
static int log_len;
static void mark(char c) { if (log_len < 31) { log_text[log_len++] = c; log_text[log_len] = 0; } }

extern "C" void SetVSyncFlag(volatile unsigned* flag, volatile unsigned long long* value) {
    mark('F');
    flag_ptr = flag;
    value_ptr = value;
    if (*flag != 0u) mark('!');
    *value = 0x1122334455667788ull;
}
extern "C" int DIntr() { mark('D'); return enabled_next; }
extern "C" int EIntr() { mark('E'); return 7; }
extern "C" unsigned norrath_host_intc_read() {
    mark('r');
    ++polls;
    if (set_bit_after != 0 && polls >= set_bit_after) intc |= 4u;
    if (set_flag_after != 0 && polls >= set_flag_after) *flag_ptr = 1u;
    return intc;
}
extern "C" void norrath_host_intc_write(unsigned value) { mark('w'); mark(static_cast<char>('0' + value)); intc &= ~value; }
extern "C" void norrath_host_pipeline_sync() { mark('Y'); }

static void reset(int enabled, int bit_after, int flag_after) {
    intc = 0; polls = 0; log_len = 0; log_text[0] = 0;
    enabled_next = enabled; set_bit_after = bit_after; set_flag_after = flag_after;
}

int main() {
    reset(1, 3, 0);
    if (VSync2(5) != 0x1122334455667788ull) return 1;
    if (std::strcmp(log_text, "FDw4YErrrDw4YE") != 0) return 2;
    reset(0, 0, 2);   // the flag, not the register, ends the wait
    if (VSync2(0) != 0x1122334455667788ull) return 3;
    if (std::strcmp(log_text, "FDw4YrrDw4Y") != 0) return 4;
    std::puts("vsync2 contract passed");
    return 0;
}
