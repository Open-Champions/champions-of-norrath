#include "start_timer_counter.cpp"

#include <cstdio>

static int log[8];
static int log_count;
static int dintr_result;
static int inner_result;
static int seen_counter;

extern "C" int DIntr() { log[log_count++] = 1; return dintr_result; }
extern "C" int EIntr() { log[log_count++] = 3; return 0; }
extern "C" int iStartTimerCounter(int counter) {
    log[log_count++] = 2;
    seen_counter = counter;
    return inner_result;
}

int main() {
    dintr_result = 1;
    inner_result = 9;
    if (StartTimerCounter(4) != 9 || seen_counter != 4 || log_count != 3 ||
        log[0] != 1 || log[1] != 2 || log[2] != 3) return 1;
    log_count = 0;
    dintr_result = 0;
    inner_result = -3;
    if (StartTimerCounter(6) != -3 || seen_counter != 6 || log_count != 2 ||
        log[0] != 1 || log[1] != 2) return 2;
    std::puts("start timer counter contract passed");
    return 0;
}
