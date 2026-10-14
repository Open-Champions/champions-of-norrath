#include "set_timer_handler.cpp"

#include <cstdio>

static int log[8];
static int log_count;
static int dintr_result;
static int inner_result;
static int seen_counter;
static unsigned long long seen_clocks;
static void* seen_handler;
static void* seen_argument;

extern "C" int DIntr() { log[log_count++] = 1; return dintr_result; }
extern "C" int EIntr() { log[log_count++] = 3; return 0; }
extern "C" int iSetTimerHandler(int counter, unsigned long long clocks,
                                int (*handler)(int, int, int, void*), void* argument) {
    log[log_count++] = 2;
    seen_counter = counter;
    seen_clocks = clocks;
    seen_handler = reinterpret_cast<void*>(handler);
    seen_argument = argument;
    return inner_result;
}

static int handler(int, int, int, void*) { return 0; }

int main() {
    int node;
    dintr_result = 1;
    inner_result = 11;
    if (SetTimerHandler(2, 0x123456789abcull, handler, &node) != 11) return 1;
    if (seen_counter != 2 || seen_clocks != 0x123456789abcull ||
        seen_handler != reinterpret_cast<void*>(handler) || seen_argument != &node) return 2;
    if (log_count != 3 || log[0] != 1 || log[1] != 2 || log[2] != 3) return 3;
    log_count = 0;
    dintr_result = 0;
    inner_result = -5;
    if (SetTimerHandler(3, 7, handler, nullptr) != -5 || seen_counter != 3 || seen_clocks != 7) return 4;
    if (log_count != 2 || log[0] != 1 || log[1] != 2) return 5;
    std::puts("set timer handler contract passed");
    return 0;
}
