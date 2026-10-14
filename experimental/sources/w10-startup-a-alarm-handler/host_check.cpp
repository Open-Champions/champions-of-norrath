// Host comparison only. Not an EE build input. The callback word is 32 bits on the target, so
// the host check supplies its own call_callback; the callback is scripted and the free list and
// the record are checked after each call.
#include <cstdio>
#define NORRATH_HOST_CHECK 1
static unsigned seen_handle, seen_param, seen_word;
static int seen1, seen2, cb_result, cb_calls;
static inline int call_callback(unsigned word, unsigned handle, int arg1, int arg2, unsigned parameter) {
    ++cb_calls; seen_word = word; seen_handle = handle; seen1 = arg1; seen2 = arg2; seen_param = parameter;
    return cb_result;
}
#include "alarm_handler.cpp"
AlarmRecord* g_pFreeAlarm;
int main() {
    int failures = 0;
    AlarmRecord rec = {reinterpret_cast<AlarmRecord*>(0x1), 77, 0xc0de, 0x1234abcd}, other = {0, 0, 0, 0};
    // nonzero result: returned as it is, record untouched, list untouched
    g_pFreeAlarm = &other; cb_result = 5;
    int r = AlarmHandler(0x37, 3, -4, &rec);
    unsigned expect = ((unsigned)(__UINTPTR_TYPE__)&rec << 4) | (0x37u & 0xfe) | 1u;
    if (r != 5 || seen_handle != expect || seen1 != 3 || seen2 != -4 || seen_param != 0x1234abcd || seen_word != 0xc0de) { std::printf("call wrong\n"); ++failures; }
    if (g_pFreeAlarm != &other || rec.counter != 77 || rec.next != reinterpret_cast<AlarmRecord*>(0x1)) { std::printf("must not free\n"); ++failures; }
    // negative nonzero result is also passed through
    cb_result = -3;
    if (AlarmHandler(0, 0, 0, &rec) != -3 || g_pFreeAlarm != &other) { std::printf("negative result\n"); ++failures; }
    // zero result: record goes to the head of the list, counter cleared, -1 returned
    cb_result = 0;
    r = AlarmHandler(0xff, 0, 0, &rec);
    if (r != -1 || g_pFreeAlarm != &rec || rec.next != &other || rec.counter != 0) { std::printf("free wrong\n"); ++failures; }
    if (seen_handle != (((unsigned)(__UINTPTR_TYPE__)&rec << 4) | 0xfeu | 1u)) { std::printf("mask wrong\n"); ++failures; }
    if (failures) return 2;
    std::printf("alarm_handler cases passed\n");
    return 0;
}
