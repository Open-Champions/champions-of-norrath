/* Host contract for _InitAlarm. Not an EE plan input. Host pointers are wider
 * than the 32-bit words the function stores, so the checks compare against
 * the low 32 bits of the host addresses. */
#include "init_alarm_list.cpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" unsigned g_pFreeAlarm = 0;
extern "C" AlarmSlot g_AlarmBuf[64] = {};

static unsigned low(const void* address) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(address));
}

int main() {
    std::memset(g_AlarmBuf, 0xa5, sizeof g_AlarmBuf);
    g_pFreeAlarm = 0x12345678u;
    if (_InitAlarm() != 0) return 1;
    if (g_pFreeAlarm != low(g_AlarmBuf)) return 2;
    for (int index = 0; index < 63; ++index)
        if (g_AlarmBuf[index].next != low(g_AlarmBuf + index + 1)) return 3;
    if (g_AlarmBuf[63].next != 0) return 4;
    for (int index = 0; index < 64; ++index)
        for (int word = 0; word < 3; ++word)
            if (g_AlarmBuf[index].unwritten[word] != 0xa5a5a5a5u) return 5;
    // The chain visits all 64 slots from the free-list head.
    int visited = 0;
    for (unsigned at = g_pFreeAlarm; at != 0 && visited < 100; ++visited) {
        const AlarmSlot* slot = nullptr;
        for (int index = 0; index < 64; ++index)
            if (low(g_AlarmBuf + index) == at) slot = g_AlarmBuf + index;
        if (slot == nullptr) return 6;
        at = slot->next;
    }
    if (visited != 64) return 7;
    // A second call rebuilds the same chain.
    g_AlarmBuf[10].next = 0;
    g_pFreeAlarm = 0;
    _InitAlarm();
    if (g_AlarmBuf[10].next != low(g_AlarmBuf + 11) || g_pFreeAlarm != low(g_AlarmBuf)) return 8;
    std::puts("init alarm list contract passed");
    return 0;
}
