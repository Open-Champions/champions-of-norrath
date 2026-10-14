// Host comparison only. Not an EE build input.
#include <cstdio>
#include <vector>
#include "i_set_timer_alarm.cpp"

AlarmRecord* g_pFreeAlarm;
static int next_counter;
static std::vector<int> events;   // 1 alloc, 2 set handler, 3 start
static int set_counter; static unsigned set_time; static void* set_arg; static void (*set_handler)();
static int start_counter;
extern "C" int iAllocTimerCounter() { events.push_back(1); return next_counter; }
extern "C" int iSetTimerHandler(int c, unsigned t, void (*h)(), void* a) { events.push_back(2); set_counter = c; set_time = t; set_handler = h; set_arg = a; return 0; }
extern "C" int iStartTimerCounter(int c) { events.push_back(3); start_counter = c; return 0; }
extern "C" void AlarmHandler() {}

int main() {
    int failures = 0;
    AlarmRecord a{}, b{};
    a.next = &b; b.next = nullptr;
    // Zero callback: refuse before touching the list.
    g_pFreeAlarm = &a;
    if (iSetTimerAlarm(5, 0, 7) != 0x80000016u || g_pFreeAlarm != &a || !events.empty()) { std::printf("zero callback wrong\n"); ++failures; }
    // Empty list.
    g_pFreeAlarm = nullptr;
    if (iSetTimerAlarm(5, 1, 7) != 0x80008005u || !events.empty()) { std::printf("empty list wrong\n"); ++failures; }
    // Counter allocation failure returns the record and the error.
    g_pFreeAlarm = &a; a.next = &b; a.counter = 99;
    next_counter = static_cast<int>(0x80008003u);
    if (iSetTimerAlarm(5, 1, 7) != 0x80008003u || g_pFreeAlarm != &a || a.next != &b || a.counter != 0) { std::printf("alloc failure wrong\n"); ++failures; }
    if (events != std::vector<int>{1}) { std::printf("failure must stop after alloc\n"); ++failures; }
    // Success.
    events.clear(); next_counter = 0x1b;
    unsigned r = iSetTimerAlarm(1234, 0xc0de, 0xbeef);
    unsigned want = (static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(&a)) << 4) | (0x1bu & 0xfeu) | 1u;
    if (r != want) { std::printf("handle %x want %x\n", r, want); ++failures; }
    if (g_pFreeAlarm != &b) { std::printf("free list head must advance\n"); ++failures; }
    if (a.callback != 0xc0de || a.parameter != 0xbeef || a.counter != 0x1b) { std::printf("record fields wrong\n"); ++failures; }
    if (events != std::vector<int>{1, 2, 3} || set_counter != 0x1b || set_time != 1234 || set_arg != &a || set_handler != AlarmHandler || start_counter != 0x1b) { std::printf("timer calls wrong\n"); ++failures; }
    if (failures) return 2;
    std::printf("iSetTimerAlarm cases passed\n");
    return 0;
}
