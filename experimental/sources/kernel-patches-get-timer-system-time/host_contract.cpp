/* Host contract for iGetTimerSystemTime. Not an EE plan input. */
#define NORRATH_HOST_CONTRACT 1
#include "get_timer_system_time.cpp"

#include <cstdio>
#include <cstring>

extern "C" TimerWords g_Timer = {0, -1, 1, 0, 0, 0, -1};
static unsigned count_values[2];
static unsigned mode_value;
static char log_text[8];
static int log_used;
static int count_reads;

static void mark(char step) {
    if (log_used < 7) log_text[log_used++] = step;
    log_text[log_used] = 0;
}
extern "C" unsigned norrath_host_timer2_count() {
    mark('C');
    return count_values[count_reads++ == 0 ? 0 : 1];
}
extern "C" unsigned norrath_host_timer2_mode() {
    mark('M');
    return mode_value;
}

static unsigned long long run(unsigned long long head, unsigned first, unsigned second, unsigned mode) {
    g_Timer.head = head;
    count_values[0] = first;
    count_values[1] = second;
    mode_value = mode;
    log_used = 0;
    log_text[0] = 0;
    count_reads = 0;
    return iGetTimerSystemTime();
}

int main() {
    if (run(5, 0x1234, 0xbad, 0) != ((5ull << 16) | 0x1234) || std::strcmp(log_text, "CM") != 0) return 1;
    if (run(5, 0x1234, 0xbad, 1) != (((5ull << 16) | 0x1234) << 4)) return 2;
    if (run(5, 0x1234, 0xbad, 2) != (((5ull << 16) | 0x1234) << 8)) return 3;
    if (run(5, 0x1234, 0xbad, 3) != (((5ull << 16) | 0x1234) << 16)) return 4;
    // Overflow flag: the count is read again, the local overflow word gains 1,
    // and g_Timer is not written.
    if (run(5, 0xfff0, 0x0004, 0x800) != ((6ull << 16) | 0x0004) || std::strcmp(log_text, "CMC") != 0) return 5;
    if (g_Timer.head != 5) return 6;
    if (run(0xffff, 0x10, 0x20, 0x802) != (((0x10000ull << 16) | 0x20) << 8)) return 7;
    // Only the low two mode bits and bit 0x800 matter.
    if (run(1, 7, 8, 0x7fffu & ~0x803u) != ((1ull << 16) | 7)) return 8;
    // The count word is zero-extended, not sign-extended.
    if (run(0, 0x80000000u, 0, 0) != 0x80000000ull) return 9;
    // The result is a full 64-bit value.
    if (run(0x7fffffffffffull, 1, 0, 3) != ((((0x7fffffffffffull) << 16) | 1) << 16)) return 10;
    std::puts("get timer system time contract passed");
    return 0;
}
