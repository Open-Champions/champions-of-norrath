/* Host contract for InsertAlarm. Not an EE plan input. Alarm links are 32-bit
 * offsets into a host arena. */
#define NORRATH_HOST_CONTRACT 1
#include "insert_alarm.cpp"

#include <cstdio>
#include <vector>

alignas(16) static unsigned char arena[8192];
extern "C" unsigned char* norrath_host_arena = arena;
extern "C" {
TimerWords g_Timer = {0, -1, 1, 0, 0, 0, -1};
}

// Alarm k lives at offset 0x100 * (k + 1).
static unsigned at(int k) { return 0x100u * (k + 1); }
static Alarm& slot(int k) { return *reinterpret_cast<Alarm*>(arena + at(k)); }
static void make(int k, unsigned long long time, unsigned long long w16 = 0, unsigned long long w24 = 0) {
    slot(k) = Alarm{};
    slot(k).word_32 = time;
    slot(k).word_16 = w16;
    slot(k).word_24 = w24;
    slot(k).next = 0x7777;      // stale values must be overwritten
    slot(k).previous = 0x7777;
}

// The chain order as alarm numbers, with every link checked on the way.
static bool order(std::vector<int>& result) {
    result.clear();
    unsigned expected_previous = 0;
    for (unsigned word = static_cast<unsigned>(g_Timer.first_alarm); word != 0;) {
        if (result.size() > 20) return false;
        const int k = static_cast<int>(word / 0x100u) - 1;
        if (static_cast<unsigned>(slot(k).previous) != expected_previous) return false;
        result.push_back(k);
        expected_previous = word;
        word = static_cast<unsigned>(slot(k).next);
    }
    return true;
}

int main() {
    std::vector<int> chain;
    g_Timer.first_alarm = 0;
    // Empty chain.
    make(0, 500);
    InsertAlarm(&slot(0));
    if (!order(chain) || chain != std::vector<int>{0}) return 1;
    // Before the head.
    make(1, 100);
    InsertAlarm(&slot(1));
    if (!order(chain) || chain != std::vector<int>{1, 0}) return 2;
    // After the tail.
    make(2, 900);
    InsertAlarm(&slot(2));
    if (!order(chain) || chain != std::vector<int>{1, 0, 2}) return 3;
    // Middle.
    make(3, 700);
    InsertAlarm(&slot(3));
    if (!order(chain) || chain != std::vector<int>{1, 0, 3, 2}) return 4;
    // An equal time goes after the alarm already there.
    make(4, 500);
    InsertAlarm(&slot(4));
    if (!order(chain) || chain != std::vector<int>{1, 0, 4, 3, 2}) return 5;
    make(5, 100);
    InsertAlarm(&slot(5));
    if (!order(chain) || chain != std::vector<int>{1, 5, 0, 4, 3, 2}) return 6;
    // The time is word_32 + word_16 - word_24, compared unsigned.
    make(6, 400, 250, 100);        // time 550: between 500 and 700
    InsertAlarm(&slot(6));
    if (!order(chain) || chain != std::vector<int>{1, 5, 0, 4, 6, 3, 2}) return 7;
    // A time that wraps below zero is a huge unsigned value and goes last.
    make(7, 0, 0, 1);
    InsertAlarm(&slot(7));
    if (!order(chain) || chain != std::vector<int>{1, 5, 0, 4, 6, 3, 2, 7}) return 8;
    // The walk looks at the stored words of passed alarms, not their order of arrival.
    make(8, 0x100000000ull + 5);
    InsertAlarm(&slot(8));
    if (!order(chain) || chain != std::vector<int>{1, 5, 0, 4, 6, 3, 2, 8, 7}) return 9;
    // Other words of the timer record are untouched.
    if (g_Timer.head != 0 || g_Timer.handler != -1 || g_Timer.field_28 != -1) return 10;
    std::puts("insert alarm contract passed");
    return 0;
}
