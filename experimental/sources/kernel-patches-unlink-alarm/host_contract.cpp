/* Host contract for UnlinkAlarm. Not an EE plan input. Alarm links are 32-bit
 * offsets into a host arena. */
#define NORRATH_HOST_CONTRACT 1
#include "unlink_alarm.cpp"

#include <cstdio>

alignas(16) static unsigned char arena[4096];
extern "C" unsigned char* norrath_host_arena = arena;
extern "C" {
TimerWords g_Timer = {0, -1, 1, 0, 0, 0, -1};
}

static unsigned at(int k) { return 0x100u * (k + 1); }
static Alarm& slot(int k) { return *reinterpret_cast<Alarm*>(arena + at(k)); }

// Build alarms 0..count-1 as a chain.
static void chain(int count) {
    for (int k = 0; k < count; ++k) {
        slot(k).next = k + 1 < count ? static_cast<int>(at(k + 1)) : 0;
        slot(k).previous = k > 0 ? static_cast<int>(at(k - 1)) : 0;
    }
    g_Timer.first_alarm = count > 0 ? static_cast<int>(at(0)) : 0;
}

int main() {
    // Middle element: neighbours are joined, the return value is the old next,
    // the alarm's previous word is cleared and its next word is kept.
    chain(3);
    Alarm* returned = UnlinkAlarm(&slot(1));
    if (returned != &slot(2)) return 1;
    if (slot(0).next != static_cast<int>(at(2)) || slot(2).previous != static_cast<int>(at(0))) return 2;
    if (slot(1).previous != 0 || slot(1).next != static_cast<int>(at(2))) return 3;
    if (g_Timer.first_alarm != static_cast<int>(at(0))) return 4;
    // First element: g_Timer offset 24 takes the next word.
    chain(3);
    returned = UnlinkAlarm(&slot(0));
    if (returned != &slot(1)) return 5;
    if (g_Timer.first_alarm != static_cast<int>(at(1)) || slot(1).previous != 0) return 6;
    if (slot(0).previous != 0 || slot(0).next != static_cast<int>(at(1))) return 7;
    // Last element: the previous alarm's next word becomes zero, and 0 is returned.
    chain(3);
    returned = UnlinkAlarm(&slot(2));
    if (returned != nullptr || slot(1).next != 0) return 8;
    if (g_Timer.first_alarm != static_cast<int>(at(0))) return 9;
    // Only element: the head becomes zero.
    chain(1);
    returned = UnlinkAlarm(&slot(0));
    if (returned != nullptr || g_Timer.first_alarm != 0 || slot(0).previous != 0 || slot(0).next != 0) return 10;
    // Words outside the two link fields and g_Timer offset 24 are untouched.
    chain(2);
    g_Timer.head = 99;
    g_Timer.field_28 = 7;
    UnlinkAlarm(&slot(0));
    if (g_Timer.head != 99 || g_Timer.field_28 != 7 || g_Timer.handler != -1) return 11;
    std::puts("unlink alarm contract passed");
    return 0;
}
