#define NORRATH_HOST_CONTRACT 1
#include "timer_handler.cpp"

#include <cstdio>
#include <cstring>

unsigned long long norrath_vi_timer_system_time;
int norrath_vi_timer_mode;
static char log_text[16];
static unsigned stored_address, stored_value;
static unsigned long long time_at_store;
extern "C" void norrath_host_store32(unsigned a, unsigned v) {
    std::strcat(log_text, "S");
    stored_address = a; stored_value = v;
    time_at_store = norrath_vi_timer_system_time;
}
extern "C" void norrath_host_sync() { std::strcat(log_text, "Y"); }
extern "C" void norrath_host_ei() { std::strcat(log_text, "E"); }

int main() {
    norrath_vi_timer_system_time = 10;
    norrath_vi_timer_mode = 1473;
    if (Handler__7VITimeri(9) != 0) return 1;
    if (norrath_vi_timer_system_time != 12ull) return 2;
    if (stored_address != 0x10000010u || stored_value != 1473u) return 3;
    if (std::strcmp(log_text, "SYE") != 0) return 4;
    if (time_at_store != 12ull) return 5;
    norrath_vi_timer_system_time = 0xffffffffull;       // carries into the high word
    Handler__7VITimeri(0);
    if (norrath_vi_timer_system_time != 0x100000001ull) return 6;
    norrath_vi_timer_mode = 0;
    Handler__7VITimeri(0);
    if (stored_value != 0u) return 7;
    std::puts("vi timer handler contract passed");
    return 0;
}
