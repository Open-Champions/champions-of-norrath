/* Host contract for SetNextComp. Not an EE plan input. Alarm links are 32-bit
 * offsets into a host arena. */
#define NORRATH_HOST_CONTRACT 1
#include "set_next_comp.cpp"

#include <cstdio>
#include <cstring>
#include <string>

alignas(16) static unsigned char arena[4096];
extern "C" unsigned char* norrath_host_arena = arena;
extern "C" TimerWords g_Timer = {0, -1, 1, 0, 0, 0, -1};

static unsigned count_value;
static unsigned mode_value;
static std::string log_text;

extern "C" unsigned norrath_host_timer2_count() {
    log_text += "C;";
    return count_value;
}
extern "C" unsigned norrath_host_timer2_mode() {
    log_text += "M;";
    return mode_value;
}
extern "C" void SetT2_COMP(unsigned value) { log_text += "P" + std::to_string(value) + ";"; }
extern "C" void SetT2_MODE(unsigned mode) { log_text += "Q" + std::to_string(mode) + ";"; }

// Alarm k lives at offset 0x100 * (k + 1). Its time is a + b - c.
static Alarm& slot(int k) { return *reinterpret_cast<Alarm*>(arena + 0x100 * (k + 1)); }
static int link_of(int k) { return 0x100 * (k + 1); }
static void make(int k, unsigned long long time, bool last) {
    slot(k).word_32 = time;          // time = word_32 + word_16 - word_24
    slot(k).word_16 = 0;
    slot(k).word_24 = 0;
    slot(k).next = last ? 0 : link_of(k + 1);
}

static std::string run(unsigned long long sample, unsigned mode, unsigned count = 0x1000) {
    mode_value = mode;
    count_value = count;
    log_text.clear();
    SetNextComp(sample);
    return log_text;
}

static std::string comp_then_mode(unsigned comp, unsigned mode) {
    return "P" + std::to_string(comp) + ";Q" + std::to_string(mode) + ";";
}
static std::string mode_then_comp(unsigned mode, unsigned comp) {
    return "Q" + std::to_string(mode) + ";P" + std::to_string(comp) + ";";
}

int main() {
    // A nonnegative word at offset 28 returns before any register read.
    g_Timer.first_alarm = link_of(0);
    make(0, 100000, true);
    g_Timer.field_28 = 0;
    if (run(0, 0x0) != "") return 1;
    g_Timer.field_28 = 5;
    if (run(0, 0x0) != "") return 2;
    g_Timer.field_28 = -1;

    // Empty chain: compare 0, then the mode with 0x800 cleared.
    g_Timer.first_alarm = 0;
    if (run(0, 0xffffffffu) != "M;" + comp_then_mode(0, 0xfffff7ffu)) return 3;
    if (run(0, 0x00000c83u) != "M;" + comp_then_mode(0, 0x00000483u)) return 4;

    // One far alarm: mode first, then the shifted time. Divider 0 and 2.
    g_Timer.first_alarm = link_of(0);
    make(0, 0x123456789ull, true);
    if (run(0, 0x0) != "M;" + mode_then_comp(0, 0x23456789u)) return 5;
    if (run(0, 0x802) != "M;" + mode_then_comp(0x002u, static_cast<unsigned>(0x123456789ull >> 8))) return 6;
    if (run(0, 0x3) != "M;" + mode_then_comp(0x3u, static_cast<unsigned>(0x123456789ull >> 12))) return 7;
    if (run(0, 0x1) != "M;" + mode_then_comp(0x1u, static_cast<unsigned>(0x123456789ull >> 4))) return 8;

    // One near alarm: the count word plus 29491 shifted by 0, 4, 8, 12.
    make(0, 50000, true);
    if (run(40000, 0x0, 0x1000) != "M;C;" + comp_then_mode(0x1000u + 29491u, 0u)) return 9;
    if (run(40000, 0x1, 0x1000) != "M;C;" + comp_then_mode(0x1000u + (29491u >> 4), 1u)) return 10;
    if (run(40000, 0x802, 0x1000) != "M;C;" + comp_then_mode(0x1000u + (29491u >> 8), 2u)) return 11;
    if (run(40000, 0x3, 0x1000) != "M;C;" + comp_then_mode(0x1000u + (29491u >> 12), 3u)) return 12;

    // Boundary: time - sample of 29490 is near, 29491 is far.
    make(0, 100000, true);
    if (run(100000 - 29490, 0).substr(0, 4) != "M;C;") return 13;
    if (run(100000 - 29491, 0) != "M;" + mode_then_comp(0, 100000u)) return 14;
    // A time in the past is a negative signed difference, so it is near.
    if (run(200000, 0).substr(0, 4) != "M;C;") return 15;
    // A time far in the past, beyond 2^63 apart, is still compared signed.
    make(0, 5, true);
    if (run(0x8000000000000000ull + 1, 0).substr(0, 4) != "M;C;") return 16;

    // Alarm time is word_32 + word_16 - word_24.
    make(0, 0, true);
    slot(0).word_32 = 70000;
    slot(0).word_16 = 40000;
    slot(0).word_24 = 5000;
    if (run(0, 0) != "M;" + mode_then_comp(0, 105000u)) return 17;

    // Cluster: each next alarm joins while its time is below the current time + 29491.
    g_Timer.first_alarm = link_of(0);
    make(0, 100000, false);
    make(1, 100000 + 29490, false);        // joins: below 100000 + 29491
    make(2, 100000 + 29490 + 29490, false); // joins: below the new current time + 29491
    make(3, 100000 + 29490 + 29490 + 29491, true);  // equal to current + 29491: stops
    if (run(0, 0) != "M;" + mode_then_comp(0, 100000u + 29490u + 29490u)) return 18;
    // An alarm earlier than the current time still joins (unsigned compare below the window).
    make(0, 100000, false);
    make(1, 90000, false);
    make(2, 95000, true);
    if (run(0, 0) != "M;" + mode_then_comp(0, 95000u)) return 19;
    // The first gap beyond the window ends the walk even if a later alarm is close.
    make(0, 100000, false);
    make(1, 200000, false);
    make(2, 200001, true);
    if (run(0, 0) != "M;" + mode_then_comp(0, 100000u)) return 20;
    // The decision uses the cluster's final time against the sample.
    make(0, 100000, false);
    make(1, 110000, true);
    if (run(100000 + 10000 - 29490, 0).substr(0, 4) != "M;C;") return 21;
    if (run(100000 + 10000 - 29491, 0) != "M;" + mode_then_comp(0, 110000u)) return 22;
    // The shifted time is cut to its low 32 bits. A time with bit 63 set makes
    // the signed difference negative, which takes the near path instead.
    g_Timer.first_alarm = link_of(0);
    make(0, 0x7edcba9876543210ull, true);
    if (run(0, 0) != "M;" + mode_then_comp(0, 0x76543210u)) return 23;
    if (run(0, 0x2) != "M;" + mode_then_comp(0x2u, static_cast<unsigned>(0x7edcba9876543210ull >> 8))) return 24;
    make(0, 0xfedcba9876543210ull, true);
    if (run(0, 0).substr(0, 4) != "M;C;") return 25;
    std::puts("set next comp contract passed");
    return 0;
}
