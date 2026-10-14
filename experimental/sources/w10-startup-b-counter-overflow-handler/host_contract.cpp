/* Host contract. Not a plan input. */
#define NORRATH_HOST_CONTRACT 1
#include "counter_overflow_handler.cpp"
#include <cstdio>
#include <vector>
extern "C" { unsigned long long rollover_counter = 0; volatile unsigned norrath_host_timer0_mode = 0; }
static std::vector<int> events;
extern "C" void norrath_host_sync_ei() { events.push_back(norrath_host_timer0_mode == 0x3a5u ? 9 : -1); }
int main() {
    norrath_host_timer0_mode = 0x3a5u;
    if (counter_overflow_handler() != 0 || rollover_counter != 1 || norrath_host_timer0_mode != 0x3a5u || events != std::vector<int>{9}) { std::puts("handler wrong"); return 1; }
    rollover_counter = 0xffffffffull;
    counter_overflow_handler();
    if (rollover_counter != 0x100000000ull) { std::puts("64-bit increment wrong"); return 1; }
    std::puts("counter_overflow_handler contract passed");
    return 0;
}
