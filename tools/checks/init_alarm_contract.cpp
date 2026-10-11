#include <cstdio>
extern "C" unsigned norrath_host_timer3_mode = 0;
static int setups, copies, entries;
static unsigned lens[2], dests[2], ids[8];
extern "C" unsigned kernel_alarm_sysentry[16] = {};
extern "C" unsigned char srcfile[1856] = {}, eenull[40] = {};
extern "C" void kernel_alarm_setup(unsigned, unsigned) { ++setups; }
extern "C" void kernel_alarm_copy(unsigned dest, const void* source, unsigned bytes) {
    dests[copies] = dest;
    lens[copies] = bytes;
    if (copies == 0 && source != srcfile) __builtin_trap();
    if (copies == 1 && source != eenull) __builtin_trap();
    ++copies;
}
extern "C" unsigned kernel_alarm_get_entry(unsigned id) { ids[entries++] = id; return 1; }
extern "C" int FlushCache(int) { return 0; }
extern "C" void InitAlarm();
int main() {
    norrath_host_timer3_mode = 0x100;
    InitAlarm();
    if (setups || copies || entries) return 1;
    norrath_host_timer3_mode = 0;
    for (int i = 0; i < 16; ++i) kernel_alarm_sysentry[i] = 50 + i;
    InitAlarm();
    if (copies != 2 || dests[0] != 0x80076000u || lens[0] != 1856 || dests[1] != 0x82000u || lens[1] != 40) return 2;
    if (entries != 6 || ids[0] != 54 || ids[5] != 64) return 3;
    if (setups != 2 + 6) return 4;
    std::puts("alarm patch orchestration passed");
}
