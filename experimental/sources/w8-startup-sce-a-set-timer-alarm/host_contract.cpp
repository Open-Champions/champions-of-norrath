#include <cstdint>
#include <cstdio>
#include <cstring>
// Host nodes must be addressable in 28 bits: ids hold the address shifted left by four.
#include "set_timer_alarm.cpp"

AlarmNode* g_pFreeAlarm = nullptr;
static int interrupt_state = 1;
static int log_len;
static char log_text[16];
static int next_counter = 3;
static int stored_counter_arg = -1;
static unsigned long long stored_clocks;
static int (*stored_handler)(int, int, int, void*);
static void* stored_arg;
static int started = -1;
static int handler_result;
static int handler_seen[4];

static void mark(char c) { if (log_len < 15) { log_text[log_len++] = c; log_text[log_len] = 0; } }
extern "C" int DIntr() { mark('D'); return interrupt_state; }
extern "C" int EIntr() { mark('E'); return 1; }
extern "C" int AllocTimerCounter() { mark('A'); return next_counter; }
extern "C" int SetTimerHandler(int c, unsigned long long k, int (*h)(int, int, int, void*), void* a) {
    mark('H');
    stored_counter_arg = c;
    stored_clocks = k;
    stored_handler = h;
    stored_arg = a;
    return 0;
}
extern "C" int StartTimerCounter(int c) { mark('S'); started = c; return 0; }
static int user(int a, int b, int c, int d) {
    handler_seen[0] = a; handler_seen[1] = b; handler_seen[2] = c; handler_seen[3] = d;
    return handler_result;
}
static void reset() { log_len = 0; log_text[0] = 0; started = -1; }

int main() {
    static AlarmNode pool[2];
    pool[0].next = &pool[1];
    pool[1].next = nullptr;
    g_pFreeAlarm = &pool[0];

    reset();
    if (static_cast<unsigned>(SetTimerAlarm(5, nullptr, 0)) != 0x80000016u || log_len != 0) return 1;

    // Success with interrupts on.
    reset();
    next_counter = 3;
    const int id = SetTimerAlarm(0x100000001ull, user, 77);
    if (std::strcmp(log_text, "DAHSE") != 0) return 2;
    if (g_pFreeAlarm != &pool[1]) return 3;
    if (pool[0].counter != 3 || pool[0].handler != user || pool[0].argument != 77) return 4;
    if (stored_counter_arg != 3 || stored_clocks != 0x100000001ull || stored_arg != &pool[0] || started != 3) return 5;
    const unsigned expect = (static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(&pool[0])) << 4) | 2u | 1u;
    if (static_cast<unsigned>(id) != expect) return 6;

    // Handler returning nonzero keeps the node and passes the result through.
    handler_result = 9;
    if (stored_handler(0x1ff, 11, 12, stored_arg) != 9) return 7;
    const unsigned masked = (static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(&pool[0])) << 4) | 0xfeu | 1u;
    if (static_cast<unsigned>(handler_seen[0]) != masked || handler_seen[1] != 11 || handler_seen[2] != 12 || handler_seen[3] != 77) return 8;
    if (g_pFreeAlarm != &pool[1] || pool[0].counter != 3) return 9;

    // Handler returning zero frees the node and yields -1.
    handler_result = 0;
    if (stored_handler(3, 0, 0, stored_arg) != -1) return 10;
    if (g_pFreeAlarm != &pool[0] || pool[0].next != &pool[1] || pool[0].counter != 0) return 11;

    // Interrupts off: no EIntr.
    reset();
    interrupt_state = 0;
    if (SetTimerAlarm(1, user, 1) == 0 || std::strcmp(log_text, "DAHS") != 0) return 12;

    // Counter allocation failure restores the list.
    reset();
    interrupt_state = 1;
    next_counter = -4;
    if (SetTimerAlarm(1, user, 1) != -4 || std::strcmp(log_text, "DAE") != 0) return 13;
    if (g_pFreeAlarm != &pool[1] || pool[1].counter != 0) return 14;

    // Exhausted list.
    reset();
    g_pFreeAlarm = nullptr;
    if (static_cast<unsigned>(SetTimerAlarm(1, user, 1)) != 0x80008005u || std::strcmp(log_text, "DE") != 0) return 15;
    std::puts("set timer alarm contract passed");
    return 0;
}
