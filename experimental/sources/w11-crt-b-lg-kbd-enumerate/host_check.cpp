// Host comparison only. Not an EE build input. The semaphore calls are recorded.
#include <cstdio>
#include <cstring>
#include <vector>
extern "C" { alignas(8) unsigned char GLibData[0x1000]; }
static std::vector<int> calls;
extern "C" int WaitSema(int s) { calls.push_back(s); return 0; }
extern "C" int SignalSema(int s) { calls.push_back(-s); return 0; }
#include "lg_kbd_enumerate.cpp"
static int failures;
static void put(unsigned off, int v) { std::memcpy(GLibData + off, &v, 4); }
static int get(unsigned off) { int v; std::memcpy(&v, GLibData + off, 4); return v; }
static void reset(int count, int semaphore, int minimum) {
    std::memset(GLibData, 0, sizeof GLibData);
    put(44, semaphore); put(64, count); put(72, minimum);
    calls.clear();
}
static void record(int n, int owner, int state) {
    put(136 + 272 * n + 24, owner); put(136 + 272 * n + 28, state);
    for (int b = 0; b < 10; ++b) GLibData[136 + 272 * n + 6 + b] = static_cast<unsigned char>(0x30 + n * 16 + b);
}
static void check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++failures; } }
int main() {
    unsigned char out[12];
    reset(3, 9, 10);
    std::memset(out, 0xee, sizeof out);
    check(lgKbdEnumerate(-1, out) == static_cast<int>(0x80000004u) && get(72) == 10 && calls.empty(), "negative index");
    check(lgKbdEnumerate(1, 0) == static_cast<int>(0x80000004u) && get(72) == 10, "null destination");
    // no records: not found
    reset(0, 9, 10);
    check(lgKbdEnumerate(5, out) == static_cast<int>(0x80000002u) && get(72) == 5, "empty set, minimum lowered");
    reset(0, 9, 3);
    check(lgKbdEnumerate(5, out) == static_cast<int>(0x80000002u) && get(72) == 3, "minimum kept");
    // owned records
    reset(3, 9, 10);
    record(0, 2, 0); record(1, 1, 4); record(2, 1, 2);
    check(lgKbdEnumerate(1, out) == static_cast<int>(0x80000005u), "state 4");
    reset(3, 9, 10);
    record(0, 1, 3); record(1, 1, 6);
    check(lgKbdEnumerate(1, out) == static_cast<int>(0x80000006u), "state 6");
    reset(3, 9, 10);
    record(0, 1, 7); record(1, 4, 4); record(2, 1, 2);
    check(lgKbdEnumerate(1, out) == 0 && calls.empty() && get(136 + 272 * 2 + 28) == 2, "state 2 skips unrelated states");
    reset(3, 9, 10);
    record(0, 1, 5); record(1, 0, 0); record(2, 3, 2);
    check(lgKbdEnumerate(1, out) == static_cast<int>(0x80000002u), "none");
    // claim a free record: state 1 wins before the owner test
    reset(3, 9, 10);
    record(0, 7, 4); record(1, 8, 1); record(2, 1, 2);
    std::memset(out, 0xee, sizeof out);
    check(lgKbdEnumerate(1, out) == 0, "claim returns 0");
    check(get(136 + 272 + 28) == 2 && get(136 + 272 + 24) == 1, "claim writes state and owner");
    bool bytes = true;
    for (int b = 0; b < 10; ++b) bytes = bytes && out[b] == 0x30 + 16 + b;
    check(bytes && out[10] == 0xee && out[11] == 0xee, "ten identification bytes only");
    check(calls.size() == 2 && calls[0] == 9 && calls[1] == -9, "semaphore wrapped around the writes");
    reset(3, 9, 10);
    record(0, 1, 4); record(1, 8, 1);
    check(lgKbdEnumerate(1, out) == static_cast<int>(0x80000005u) && calls.empty(), "earlier owner decides first");
    if (failures) return 2;
    std::puts("lgKbdEnumerate cases passed");
    return 0;
}
