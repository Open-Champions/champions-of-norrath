/* Host contract. Not a plan input. The kernel calls are recording stubs. */
#define NORRATH_HOST_CONTRACT 1
#include "init_ee_timer.cpp"
#include <cstdio>
#include <vector>
extern "C" { int timer_handle = 0; volatile unsigned norrath_host_timer0_count = 0xffff; volatile unsigned norrath_host_timer0_mode = 0; }
static std::vector<long> events;
extern "C" int counter_overflow_handler() { return 0; }
extern "C" int AddIntcHandler(int cause, int (*handler)(), int next) {
    events.push_back(1); events.push_back(cause); events.push_back(handler == counter_overflow_handler); events.push_back(next);
    return 0x4242;
}
extern "C" int EnableIntc(int cause) {
    events.push_back(2); events.push_back(cause);
    events.push_back(timer_handle == 0x4242 && norrath_host_timer0_count == 0 && norrath_host_timer0_mode == 0x282);
    return 77;
}
int main() {
    if (init_ee_timer() != 77 || events != std::vector<long>{1, 9, 1, 0, 2, 9, 1}) { std::puts("init_ee_timer wrong"); return 1; }
    std::puts("init_ee_timer contract passed");
    return 0;
}
