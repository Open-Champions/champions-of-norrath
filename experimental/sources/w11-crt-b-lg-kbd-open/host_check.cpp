// Host comparison only. Not an EE build input. The semaphore calls are recorded.
#include <cstdio>
#include <cstring>
#include <vector>
extern "C" { alignas(8) unsigned char GLibData[0x1000]; }
static std::vector<int> calls;
static int counter_at_signal;
extern "C" int WaitSema(int s) { calls.push_back(s); return 0; }
extern "C" int SignalSema(int s) {
    calls.push_back(-s);
    std::memcpy(&counter_at_signal, GLibData + 88, 4);
    return 0;
}
#include "lg_kbd_open.cpp"
static int failures;
static void put(unsigned off, int v) { std::memcpy(GLibData + off, &v, 4); }
static int get(unsigned off) { int v; std::memcpy(&v, GLibData + off, 4); return v; }
static void reset(int count, int semaphore) {
    std::memset(GLibData, 0, sizeof GLibData);
    put(44, semaphore); put(64, count); put(88, 10);
    calls.clear();
}
static void record(int n, int owner, int state) {
    put(136 + 272 * n + 24, owner); put(136 + 272 * n + 28, state);
}
static void check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++failures; } }
int main() {
    int h = 55;
    reset(3, 9);
    check(lgKbdOpen(-1, &h) == static_cast<int>(0x80000004u) && h == 55, "negative");
    check(lgKbdOpen(4, &h) == static_cast<int>(0x80000004u) && h == 55, "above count");
    check(lgKbdOpen(1, 0) == static_cast<int>(0x80000004u), "null handle");
    check(lgKbdOpen(3, &h) == static_cast<int>(0x80000002u) && h == -1, "index equal to count passes the check, finds nothing");
    reset(3, 9);
    record(0, 1, 4);
    h = 55;
    check(lgKbdOpen(1, &h) == static_cast<int>(0x80000005u) && h == -1 && calls.empty(), "state 4");
    reset(3, 9);
    record(0, 1, 3); record(1, 1, 6);
    check(lgKbdOpen(1, &h) == static_cast<int>(0x80000006u), "state 6");
    reset(3, 9);
    record(0, 1, 0); record(1, 2, 2); record(2, 1, 7);
    check(lgKbdOpen(1, &h) == static_cast<int>(0x80000002u) && h == -1 && get(88) == 10, "no usable record");
    reset(3, 9);
    record(0, 1, 5); record(1, 2, 2); record(2, 1, 2);
    h = 55;
    check(lgKbdOpen(1, &h) == 0 && h == 3, "claim returns position plus one");
    check(get(136 + 272 * 2 + 20) == 3 && get(136 + 272 * 2 + 28) == 4 && get(88) == 11, "record and counter written");
    check(get(136 + 272 * 1 + 28) == 2 && get(136 + 28) == 5, "other records untouched");
    check(calls.size() == 2 && calls[0] == 9 && calls[1] == -9 && counter_at_signal == 11, "counter stored before the semaphore is released");
    if (failures) return 2;
    std::puts("lgKbdOpen cases passed");
    return 0;
}
