#include <cstdio>
#include <cstdint>
extern "C" unsigned norrath_host_timer2_mode = 0;
struct TimerWords {
    unsigned long long head;
    int handler;
    int untouched;
    int field_16;
    unsigned counter;
};
extern "C" TimerWords g_Timer = {};
extern "C" unsigned char g_CounterBuf[8192] = {};
static int handler_result, previous, count_calls, comp_calls, mode_calls, enable_calls, alarm_calls, added_cause;
static unsigned programmed;
extern "C" void cbTimerHandler() {}
extern "C" void* memset(void* destination, int value, __SIZE_TYPE__ count) {
    auto* bytes = static_cast<unsigned char*>(destination);
    for (__SIZE_TYPE__ i = 0; i < count; ++i) bytes[i] = static_cast<unsigned char>(value);
    return destination;
}
extern "C" void _InitAlarm() { ++alarm_calls; }
extern "C" int AddIntcHandler2(int cause, void (*handler)(), int third, int fourth) {
    added_cause = cause;
    if (handler != cbTimerHandler || third || fourth) return -1;
    return handler_result;
}
extern "C" int DIntr() { return previous; }
extern "C" void EIntr() {}
extern "C" void SetT2_COUNT(unsigned value) { if (value != 0) __builtin_trap(); ++count_calls; }
extern "C" void SetT2_COMP(unsigned value) { if (value != 0xffffu) __builtin_trap(); ++comp_calls; }
extern "C" void SetT2_MODE(unsigned mode) { programmed = mode; ++mode_calls; }
extern "C" void EnableIntc(int cause) { if (cause != 11) __builtin_trap(); ++enable_calls; }
extern "C" unsigned InitTimer(unsigned selector);
int main() {
    g_Timer.handler = 0;
    if (InitTimer(2) != 0x80008001u || alarm_calls != 0) return 1;
    g_Timer.handler = -1;
    handler_result = -4;
    if (InitTimer(2) != 0x80009021u || alarm_calls != 1) return 2;
    g_Timer.handler = -1;
    handler_result = 6;
    previous = 0;
    norrath_host_timer2_mode = 0;
    if (InitTimer(2) != 0 || g_Timer.handler != 6 || g_Timer.head != 0 || g_Timer.field_16 != 0) return 3;
    if (programmed != (2u | 0x300u | 0xc80u) || count_calls != 1 || comp_calls != 1 || enable_calls != 1) return 4;
    unsigned base = static_cast<unsigned>(reinterpret_cast<uintptr_t>(g_CounterBuf));
    if (g_Timer.counter != base) return 5;
    auto word = [](unsigned offset) {
        return *reinterpret_cast<unsigned*>(g_CounterBuf + offset);
    };
    if (word(8128) != 0 || word(0) != base + 64u) return 6;
    std::puts("timer initialization orchestration passed");
}
