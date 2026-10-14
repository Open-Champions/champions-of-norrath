/* Host contract for cbTimerHandler. Not an EE plan input. Alarm links are
 * 32-bit offsets into a host arena, and callback words index a host table. */
#define NORRATH_HOST_CONTRACT 1
#include "timer_handler.cpp"

#include <cstdio>
#include <deque>
#include <string>

alignas(16) static unsigned char arena[8192];
extern "C" unsigned char* norrath_host_arena = arena;
extern "C" TimerWords g_Timer = {0, -1, 1, 0, 0, 0, -1};

static std::deque<unsigned> count_script, mode_script;
static unsigned count_last, mode_last;
static std::string log_text;
static int gp_value;
static int field_28_in_callback;

struct Recorded {
    unsigned identifier;
    unsigned long long value, elapsed;
    int argument, third;
    int gp_at_call;
};
static Recorded recorded[8];
static int recorded_count;
static unsigned long long results[8];

extern "C" unsigned norrath_host_timer2_count() {
    log_text += "C;";
    if (!count_script.empty()) {
        count_last = count_script.front();
        count_script.pop_front();
    }
    return count_last;
}
extern "C" unsigned norrath_host_timer2_mode() {
    log_text += "M;";
    if (!mode_script.empty()) {
        mode_last = mode_script.front();
        mode_script.pop_front();
    }
    return mode_last;
}
extern "C" void norrath_host_set_gp(int value) {
    gp_value = value;
    log_text += "G" + std::to_string(value) + ";";
}
extern "C" void norrath_host_sync_and_enable() { log_text += "Y;"; }
extern "C" void SetT2_MODE(unsigned mode) { log_text += "T" + std::to_string(mode) + ";"; }
extern "C" void SetNextComp(unsigned long long sample) {
    log_text += "N" + std::to_string(sample) + ";";
    if (g_Timer.field_28 != -1) log_text += "!";
}

static unsigned word_of(const Alarm* alarm) {
    return static_cast<unsigned>(reinterpret_cast<const unsigned char*>(alarm) - arena);
}
static Alarm* alarm_at(unsigned word) { return reinterpret_cast<Alarm*>(arena + word); }

extern "C" Alarm* UnlinkAlarm(Alarm* alarm) {
    log_text += "U" + std::to_string(word_of(alarm)) + ";";
    Alarm* following = alarm->next != 0 ? alarm_at(alarm->next) : nullptr;
    if (alarm->previous != 0)
        alarm_at(alarm->previous)->next = alarm->next;
    else
        g_Timer.first_alarm = alarm->next;
    if (following != nullptr) following->previous = alarm->previous;
    alarm->previous = 0;
    return following;
}
extern "C" void InsertAlarm(Alarm* alarm) {
    log_text += "I" + std::to_string(word_of(alarm)) + ";";
}

static unsigned long long callback(unsigned identifier, unsigned long long value, unsigned long long elapsed,
                                   int argument, int third) {
    Recorded& r = recorded[recorded_count];
    r = {identifier, value, elapsed, argument, third, gp_value};
    field_28_in_callback = g_Timer.field_28;
    log_text += "B" + std::to_string(identifier) + ";";
    return results[recorded_count++];
}
extern "C" AlarmCallback norrath_host_callback(unsigned word) {
    return word == 0x77 ? callback : nullptr;
}

// Alarm k lives at offset 0x100 * (k + 1).
static unsigned at(int k) { return 0x100u * (k + 1); }
static Alarm& slot(int k) { return *alarm_at(at(k)); }
static void make(int k, unsigned long long time, bool last, int gp = 0x500) {
    Alarm& a = slot(k);
    a = Alarm{};
    a.word_32 = time;
    a.word_40 = 0x77;
    a.word_44 = gp + k;
    a.word_48 = 0x900 + k;
    a.word_8 = 3;
    a.word_12 = 0xf;
    a.next = last ? 0 : static_cast<int>(at(k + 1));
    a.previous = k == 0 ? 0 : static_cast<int>(at(k - 1));
}

static void reset(const std::deque<unsigned>& modes, const std::deque<unsigned>& counts, unsigned long long head = 0) {
    mode_script = modes;
    count_script = counts;
    mode_last = modes.empty() ? 0 : modes.back();
    count_last = counts.empty() ? 0 : counts.back();
    log_text.clear();
    recorded_count = 0;
    gp_value = -1;
    field_28_in_callback = 12345;
    g_Timer.head = head;
    g_Timer.field_28 = -1;
    g_Timer.counter = 0;
    for (auto& r : results) r = 0;
}

int main() {
    // Compare flag clear: no alarm work; the tail runs once with the time from
    // a fresh count and mode, then a second pair of reads, sync and ei.
    make(0, 0, true);
    g_Timer.first_alarm = static_cast<int>(at(0));
    reset({0x000, 0x000, 0x000}, {0x1234, 0x1234, 0x1234}, 7);
    if (cbTimerHandler(1, 2, 3) != 0) return 1;
    if (log_text != "M;C;M;N" + std::to_string((7ull << 16) | 0x1234) + ";C;M;Y;") return 2;
    if (g_Timer.field_28 != -1) return 3;
    // The time uses four times the low two mode bits as the shift.
    reset({0x001, 0x001, 0x001}, {0x1234, 0x1234, 0x1234}, 7);
    cbTimerHandler(1, 2, 3);
    if (log_text != "M;C;M;N" + std::to_string(((7ull << 16) | 0x1234) << 4) + ";C;M;Y;") return 4;
    reset({0x003, 0x003, 0x003}, {0x1234, 0x1234, 0x1234}, 7);
    cbTimerHandler(1, 2, 3);
    if (log_text != "M;C;M;N" + std::to_string(((7ull << 16) | 0x1234) << 12) + ";C;M;Y;") return 5;
    // Compare flag set but nothing is queued: same tail, only the first read.
    g_Timer.first_alarm = 0;
    reset({0x400, 0x400, 0x400}, {5, 5, 5});
    cbTimerHandler(1, 2, 3);
    if (log_text != "M;C;M;N" + std::to_string(5ull) + ";C;M;Y;") return 6;
    g_Timer.first_alarm = static_cast<int>(at(0));

    // One due alarm whose callback returns 0.
    make(0, 1000, true);
    reset({0x400, 0x400, 0x400, 0x400}, {2000, 2000, 2000, 2000}, 0);
    results[0] = 0;
    g_Timer.first_alarm = static_cast<int>(at(0));
    slot(0).word_16 = 40;
    slot(0).word_24 = 15;
    cbTimerHandler(11, 22, 33);
    const unsigned id0 = (at(0) << 4) | 3u;
    if (log_text != "M;C;M;U" + std::to_string(at(0)) + ";G" + std::to_string(0x500) + ";B" + std::to_string(id0) +
                    ";C;M;N2000;C;M;Y;") return 7;
    if (recorded_count != 1 || recorded[0].identifier != id0 || recorded[0].value != 1000) return 8;
    if (recorded[0].elapsed != 2000 + 15 - 40) return 9;
    if (recorded[0].argument != 0x900 || recorded[0].third != 33 || recorded[0].gp_at_call != 0x500) return 10;
    if (field_28_in_callback != static_cast<int>(id0)) return 11;
    if (slot(0).word_12 != 0xd) return 12;                // bit 2 cleared
    if (g_Timer.first_alarm != 0 || g_Timer.field_28 != -1) return 13;

    // A callback result of all ones returns the alarm to the g_Timer offset 20 chain.
    make(0, 1000, true);
    reset({0x400, 0x400, 0x400, 0x400}, {2000}, 0);
    g_Timer.first_alarm = static_cast<int>(at(0));
    g_Timer.counter = 0xabc;
    results[0] = ~0ull;
    cbTimerHandler(0, 0, 0);
    if (g_Timer.counter != at(0) || slot(0).next != 0xabc) return 14;
    if (slot(0).word_8 != 0 || slot(0).word_12 != 0) return 15;
    if (log_text.find('I') != std::string::npos) return 16;

    // Other results re-insert after adding max(result, 14745) to the alarm's word at 32.
    const unsigned long long cases[][2] = {{1, 14745}, {14744, 14745}, {14745, 14745}, {14746, 14746}, {50000, 50000},
                                           {0x100000000ull, 0x100000000ull}};
    for (auto& c : cases) {
        make(0, 1000, true);
        reset({0x400, 0x400, 0x400, 0x400}, {2000}, 0);
        g_Timer.first_alarm = static_cast<int>(at(0));
        results[0] = c[0];
        cbTimerHandler(0, 0, 0);
        if (slot(0).word_32 != 1000 + c[1]) return 17;
        if (log_text.find("I" + std::to_string(at(0)) + ";") == std::string::npos) return 18;
        if (slot(0).word_12 != 0xf || slot(0).word_8 != 3) return 19;
    }

    // Not yet due: no unlink, no callback. Equal to due counts as due.
    make(0, 5000, true);
    reset({0x400, 0x400, 0x400, 0x400}, {4999}, 0);
    g_Timer.first_alarm = static_cast<int>(at(0));
    cbTimerHandler(0, 0, 0);
    if (log_text != "M;C;M;C;M;N4999;C;M;Y;") return 20;
    make(0, 5000, true);
    reset({0x400, 0x400, 0x400, 0x400}, {5000}, 0);
    g_Timer.first_alarm = static_cast<int>(at(0));
    cbTimerHandler(0, 0, 0);
    if (log_text.find('U') == std::string::npos) return 21;

    // Two due alarms: both run in order; each pass re-reads count and mode.
    make(0, 1000, false);
    make(1, 1500, true);
    reset({0x400, 0x400, 0x400, 0x400, 0x400, 0x400}, {2000}, 0);
    g_Timer.first_alarm = static_cast<int>(at(0));
    results[0] = 0;
    results[1] = 0;
    cbTimerHandler(0, 0, 9);
    const unsigned id1 = (at(1) << 4) | 3u;
    if (log_text != "M;C;M;U" + std::to_string(at(0)) + ";G" + std::to_string(0x500) + ";B" + std::to_string(id0) +
                    ";C;M;U" + std::to_string(at(1)) + ";G" + std::to_string(0x501) + ";B" + std::to_string(id1) +
                    ";C;M;N2000;C;M;Y;") return 22;
    if (recorded[1].identifier != id1 || recorded[1].value != 1500 || recorded[1].argument != 0x901) return 23;
    if (recorded[1].third != 9 || recorded[1].gp_at_call != 0x501) return 24;
    // The second alarm is not yet due: the loop stops after the first.
    make(0, 1000, false);
    make(1, 2500, true);
    reset({0x400, 0x400, 0x400, 0x400, 0x400, 0x400}, {2000}, 0);
    g_Timer.first_alarm = static_cast<int>(at(0));
    cbTimerHandler(0, 0, 9);
    if (recorded_count != 1 || g_Timer.first_alarm != static_cast<int>(at(1))) return 25;

    // Overflow flag inside the loop: head gains 1, SetT2_MODE gets the mode with
    // 0x400 cleared, count is read again, and the new head and count are used.
    make(0, (1ull << 16) | 77, true);
    reset({0x400, 0xc00 | 0x1, 0x400}, {0xffff, 77}, 0);
    g_Timer.first_alarm = static_cast<int>(at(0));
    results[0] = 0;
    cbTimerHandler(0, 0, 0);
    // now = ((1 << 16) | 77) << 4; due = (1 << 16) | 77: due.
    const std::string overflow_prefix = "M;C;M;T" + std::to_string(0x801u) + ";C;U";
    if (log_text.substr(0, overflow_prefix.size()) != overflow_prefix) {
        std::fprintf(stderr, "%s\n", log_text.c_str());
        return 26;
    }
    if (g_Timer.head != 1) return 27;

    // Tail overflow handling: the first overflow check updates the time passed
    // on, and the second check reads the count word again after SetT2_MODE.
    g_Timer.first_alarm = 0;
    reset({0x000, 0x800, 0x800}, {10, 20, 30, 40}, 3);
    cbTimerHandler(0, 0, 0);
    if (log_text != "M;C;M;T" + std::to_string(0x800u) + ";C;N" + std::to_string((4ull << 16) | 20) + ";C;M;T" +
                    std::to_string(0x800u) + ";C;Y;") {
        std::fprintf(stderr, "%s\n", log_text.c_str());
        return 28;
    }
    if (g_Timer.head != 5) return 29;
    std::puts("timer handler contract passed");
    return 0;
}
