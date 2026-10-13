#include "alloc_timer_counter.cpp"

#include <cstdio>

static int log[8];
static int log_count;
static int dintr_result;
static int inner_result;

extern "C" int DIntr() { log[log_count++] = 1; return dintr_result; }
extern "C" int EIntr() { log[log_count++] = 3; return 0; }
extern "C" int iAllocTimerCounter() { log[log_count++] = 2; return inner_result; }

int main() {
    dintr_result = 1;
    inner_result = 5;
    if (AllocTimerCounter() != 5 || log_count != 3 || log[0] != 1 || log[1] != 2 || log[2] != 3) return 1;
    log_count = 0;
    dintr_result = 0;
    inner_result = -7;
    if (AllocTimerCounter() != -7 || log_count != 2 || log[0] != 1 || log[1] != 2) return 2;
    std::puts("alloc timer counter contract passed");
    return 0;
}
