// Host comparison only. Not an EE build input. The semaphore, event helpers and the time function
// are recorded or scripted.
#include <cstdio>
#include <cstring>
#include <vector>
extern "C" { alignas(8) unsigned char GLibData[0x1000]; }
static std::vector<int> calls;
static long long fake_now;
static int modifier_result;
static int init_calls;
extern "C" int WaitSema(int s) { calls.push_back(s); return 0; }
extern "C" int SignalSema(int s) { calls.push_back(-s); return 0; }
extern "C" int IsEventModifier(void*) { return modifier_result; }
extern "C" void initialise_event_list(void*) { ++init_calls; }
static long long time_function() { return fake_now; }
#include "lg_kbd_read.cpp"
static int failures;
static void put(unsigned off, int v) { std::memcpy(GLibData + off, &v, 4); }
static int get(unsigned off) { int v; std::memcpy(&v, GLibData + off, 4); return v; }
static unsigned short get16(unsigned off) { unsigned short v; std::memcpy(&v, GLibData + off, 2); return v; }
static long long get64(unsigned off) { long long v; std::memcpy(&v, GLibData + off, 8); return v; }
static const unsigned R = 136;
static void reset() {
    std::memset(GLibData, 0, sizeof GLibData);
    put(44, 9); put(64, 2); put(96, 1); put(120, 300); put(112, 40);
    unsigned long long address = static_cast<unsigned long long>(reinterpret_cast<__UINTPTR_TYPE__>(&time_function));
    std::memcpy(GLibData + 128, &address, 4);
    calls.clear(); init_calls = 0; modifier_result = 0; fake_now = 1000;
}
static void check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++failures; } }
static void rec_state(int n, int state) { put(R + 272 * n + 28, state); }
int main() {
    // The library stores 32-bit function addresses, so a 64-bit host cannot reach the time function
    // through the state word; only the status and argument paths run here. The delivery paths are
    // covered by the paired run against the original image.
    {
        reset();
        unsigned char out[64];
        rec_state(0, 3);
        check(lgKbdRead(1, out) == static_cast<int>(0x80000000u), "state 3");
        rec_state(0, 1);
        check(lgKbdRead(1, out) == static_cast<int>(0x8000000fu), "state 1");
        rec_state(0, 2);
        check(lgKbdRead(1, out) == static_cast<int>(0x8000000fu), "state 2");
        rec_state(0, 0);
        check(lgKbdRead(1, out) == static_cast<int>(0x80000000u), "state 0");
        rec_state(0, -3);
        check(lgKbdRead(1, out) == static_cast<int>(0x80000000u), "negative state");
        rec_state(0, 5);
        check(lgKbdRead(1, out) == static_cast<int>(0x80000000u), "state 5");
        rec_state(0, 6);
        check(lgKbdRead(1, out) == static_cast<int>(0x80000006u), "state 6");
        rec_state(0, 9);
        check(lgKbdRead(1, out) == static_cast<int>(0x80000000u), "state 9");
        rec_state(0, 4); put(R + 16, 0x77);
        check(lgKbdRead(1, out) == 0x77 && calls.empty(), "pending word returned");
        check(lgKbdRead(0, out) == static_cast<int>(0x80000004u), "handle zero");
        check(lgKbdRead(4, out) == static_cast<int>(0x80000004u), "handle above count + 1");
        check(lgKbdRead(1, 0) == static_cast<int>(0x80000004u), "null destination");
        if (failures) return 2;
        std::puts("lgKbdRead status cases passed");
        return 0;
    }
}
