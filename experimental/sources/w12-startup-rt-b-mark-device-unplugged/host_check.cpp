// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp
#include <cstdio>
#include <cstring>
#include <initializer_list>
#include "mark_device_unplugged.cpp"
alignas(8) unsigned char GLibData[4096];
static int waits, signals, wait_id, signal_id; static int order[4]; static int steps;
extern "C" int WaitSema(int s) { ++waits; wait_id = s; order[steps++] = 1; return 0; }
extern "C" int SignalSema(int s) { ++signals; signal_id = s; order[steps++] = 2; return 0; }
static int fails;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static int& w(int off) { return *reinterpret_cast<int*>(GLibData + off); }
static short& h(int off) { return *reinterpret_cast<short*>(GLibData + off); }
static unsigned char snapshot[4096];
static void reset() {
    std::memset(GLibData, 0x11, sizeof GLibData);
    w(64) = 3; w(68) = 2; w(44) = 321; w(100) = 0; w(104) = 0;
    for (int i = 0; i < 3; ++i) { h(128 + 272 * i + 12) = (short)(10 + i); w(128 + 272 * i + 36) = 0; w(128 + 272 * i + 24) = 0; }
    for (int i = 0; i < 2; ++i) { h(1216 + 104 * i + 12) = (short)(20 + i); w(1216 + 104 * i + 36) = 0; w(1216 + 104 * i + 24) = 0; }
    waits = signals = steps = 0;
    std::memcpy(snapshot, GLibData, sizeof snapshot);
}
static bool only_changed(std::initializer_list<int> offsets) {   // every changed byte lies in a 4-byte word at these offsets
    for (int i = 0; i < 4096; ++i) {
        if (GLibData[i] == snapshot[i]) continue;
        bool ok = false;
        for (int o : offsets) if (i >= o && i < o + 4) ok = true;
        if (!ok) return false;
    }
    return true;
}
int main() {
    // table one, second record
    reset(); mark_device_unplugged(11);
    CHECK(waits == 1 && signals == 1 && wait_id == 321 && signal_id == 321 && order[0] == 1 && order[1] == 2);
    int r = 128 + 272;
    CHECK(w(r + 24) == (int)0x80000006 && h(r + 12) == -1 && w(r + 36) == 6 && w(100) == 1 && w(104) == 0);
    CHECK(h(128 + 12) == 10 && h(128 + 544 + 12) == 12 && only_changed({r + 24, r + 12, r + 36, 100}));
    // table two, second record
    reset(); mark_device_unplugged(21);
    r = 1216 + 104;
    CHECK(waits == 1 && signals == 1 && w(r + 24) == (int)0x80000006 && h(r + 12) == -1 && w(r + 36) == 6 && w(104) == 1 && w(100) == 0);
    CHECK(only_changed({r + 24, r + 12, r + 36, 104}));
    // no match: nothing at all
    reset(); mark_device_unplugged(99);
    CHECK(waits == 0 && signals == 0 && std::memcmp(GLibData, snapshot, sizeof snapshot) == 0);
    // an already-6 record: found, left alone, no semaphore use, and the search does not go on
    reset(); w(128 + 36) = 6; h(1216 + 12) = 10; std::memcpy(snapshot, GLibData, sizeof snapshot); mark_device_unplugged(10);
    CHECK(waits == 0 && signals == 0 && std::memcmp(GLibData, snapshot, sizeof snapshot) == 0);
    // the first match wins; table one before table two
    reset(); h(1216 + 12) = 12; mark_device_unplugged(12);
    CHECK(h(128 + 544 + 12) == -1 && h(1216 + 12) == 12 && w(100) == 1 && w(104) == 0);
    reset(); h(128 + 272 + 12) = 10; mark_device_unplugged(10);
    CHECK(h(128 + 12) == -1 && h(128 + 272 + 12) == 10);
    // counts bound the search: a matching record past the count is ignored
    reset(); w(64) = 1; w(68) = 0; mark_device_unplugged(11);
    CHECK(waits == 0 && signals == 0);
    reset(); w(64) = 0; w(68) = 1; mark_device_unplugged(21);
    CHECK(waits == 0 && signals == 0);
    reset(); w(64) = 0; w(68) = 2; mark_device_unplugged(21);
    CHECK(waits == 1 && w(1216 + 104 + 36) == 6);
    // the halfword compare is signed: a freed slot (-1) matches device -1 but not 65535
    reset(); h(128 + 12) = -1; mark_device_unplugged(65535);
    CHECK(waits == 0);
    reset(); h(128 + 12) = -1; mark_device_unplugged(-1);
    CHECK(waits == 1 && w(128 + 36) == 6 && w(100) == 1);
    if (fails) return 1;
    std::printf("mark_device_unplugged cases passed\n");
    return 0;
}
