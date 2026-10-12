// Host comparison only. Not an EE build input. The system call is replaced by a
// scripted current-thread id; the ring is compared with an independent model.
#include <cstdio>
#include <cstring>
#include <vector>
#define W9_HOST_CHECK 1
static int current_thread;
#define W9_CURRENT_THREAD_ID(thread) (current_thread)
extern "C" {
int topId = 1;
int topSema = 77;
alignas(8) unsigned char topArg[1080];
}
static std::vector<int> signals;
static std::vector<int> delegated;
extern "C" int iSignalSema(int s) { signals.push_back(s); return 0; }
extern "C" int _iWakeupThread(int t) { delegated.push_back(t); return 1000 + t; }
#include "i_wakeup_thread.cpp"

int main() {
    int failures = 0;
    std::memset(topArg, 0, sizeof topArg);
    // Another thread: delegate, no ring effect.
    current_thread = 3;
    if (iWakeupThread(9) != 1009 || delegated != std::vector<int>{9} || !signals.empty()) { std::printf("delegate wrong\n"); ++failures; }
    unsigned char zeros[1080] = {0};
    if (std::memcmp(topArg, zeros, sizeof zeros) != 0) { std::printf("delegate touched ring\n"); ++failures; }
    // Same thread, topId zero or id >= 256: -1, nothing queued.
    topId = 0; current_thread = 3;
    if (iWakeupThread(3) != -1 || !signals.empty()) { std::printf("zero topId must refuse\n"); ++failures; }
    topId = 1; current_thread = 256;
    if (iWakeupThread(256) != -1 || !signals.empty()) { std::printf("id 256 must refuse\n"); ++failures; }
    // Same thread, queue 1100 wakes with distinct ids; model the ring.
    unsigned char model[1080] = {0};
    int cursor = 0;
    for (int n = 0; n < 1100; ++n) {
        int id = 1 + n % 255;
        current_thread = id;
        int got = iWakeupThread(id);
        int idx = cursor & 0x1ff;
        cursor = idx + 1;
        model[8 + idx * 2] = 0; model[9 + idx * 2] = static_cast<unsigned char>(id);
        if (got != id) { std::printf("return wrong at %d\n", n); ++failures; break; }
    }
    int stored; std::memcpy(&stored, topArg + 4, 4);
    if (stored != cursor) { std::printf("cursor %d want %d\n", stored, cursor); ++failures; }
    model[4] = static_cast<unsigned char>(cursor); model[5] = static_cast<unsigned char>(cursor >> 8);
    if (std::memcmp(topArg, model, sizeof model) != 0) { std::printf("ring contents differ\n"); ++failures; }
    if (signals.size() != 1100 || signals[0] != 77) { std::printf("signal count/id wrong\n"); ++failures; }
    // After 512 entries the cursor wraps to 1 rather than 0: the 513th write goes to index 1.
    if (cursor == 0) { std::printf("model self-check\n"); ++failures; }
    if (failures) return 2;
    std::printf("iWakeupThread cases passed\n");
    return 0;
}
