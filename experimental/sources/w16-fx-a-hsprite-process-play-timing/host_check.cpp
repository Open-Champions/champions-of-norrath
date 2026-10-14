// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "process_play_timing.cpp"
#include <cmath>
#include <cstring>
static long long clockValue;
static int clockCalls, fmodCalls;
extern "C" unsigned char* Animation__7VIScenei(void*, int) { return arena + 0x3000; }
extern "C" long long GetTime__7VIScene(void*) { ++clockCalls; return clockValue; }
extern "C" float fmodf(float x, float y) { ++fmodCalls; return std::fmod(x, y); }
extern "C" long long __fixsfdi(float v) { return static_cast<long long>(v); }
static unsigned& w(unsigned char* base, int off) { return *reinterpret_cast<unsigned*>(base + off); }
static int& i32(unsigned char* base, int off) { return *reinterpret_cast<int*>(base + off); }
static float& f32(unsigned char* base, int off) { return *reinterpret_cast<float*>(base + off); }
static long long& i64(unsigned char* base, int off) { return *reinterpret_cast<long long*>(base + off); }
int main() {
    alignas(8) static unsigned char self[512];
    unsigned char* anim = arena + 0x3000;
    unsigned char* play = arena + 0x4000 + 1 * 104;      // play index 1
    unsigned char* triggers = arena + 0x5000;
    unsigned char* entries = arena + 0x3800;
    w(self, 128) = 0x4000;                                // play pool element base
    w(self, 88) = 1; w(self, 92) = 0x5000;                // trigger table: one record
    i32(anim, 16) = 10; i32(anim, 20) = 1; w(anim, 44) = 0x3800;
    f32(entries, 0) = 2.5f; w(entries, 4) = 0x77;         // node fires at animation time 2.5, trigger id 0x77
    w(triggers, 0) = 0x77;
    // a stopped play (state 3) only restamps the clock
    clockValue = 5000; w(play, 4) = 3;
    if (ProcessPlayTiming__9VIHSpriteiR7VIScene(self, 1, arena) != 0 || i64(play, 64) != 5000 || clockCalls != 1) return 1;
    // forward play: 1.0 frame per tick unit, crosses the node time
    w(play, 4) = 0; w(play, 12) = 0;                      // looping, no timed end
    f32(play, 40) = 1.0f; f32(play, 72) = 1.0f; f32(play, 76) = 1.0f;
    i32(play, 44) = 2; f32(play, 56) = 0.25f; f32(play, 52) = 0.0f; i64(play, 64) = 5000;
    clockValue = 5001; clockCalls = 0;
    if (ProcessPlayTiming__9VIHSpriteiR7VIScene(self, 1, arena) != 0) return 2;
    // dt = 1 -> acc = 1.0 -> 1 whole frame: frame 3, acc 0, fraction 0
    if (i32(play, 44) != 3 || i32(play, 48) != 4 || f32(play, 52) != 0.0f || f32(play, 56) != 0.0f) return 3;
    // node time 2.5 lies in (2.25, 3.0]: fired, amount 0.5 -> 0 ticks, stamp = clock
    if (w(triggers, 4) != 1 || i64(triggers, 8) != 5001 || i64(play, 64) != 5001) return 4;
    // clamped play running off the end finishes (state 3) and zeroes the accumulator
    w(triggers, 4) = 0;
    w(play, 4) = 1; i32(play, 44) = 9; f32(play, 52) = 0.0f; f32(play, 56) = 0.0f; i64(play, 64) = 5001; clockValue = 5003;
    ProcessPlayTiming__9VIHSpriteiR7VIScene(self, 1, arena);
    if (w(play, 4) != 3 || i32(play, 44) != 9 || i32(play, 48) != 9 || f32(play, 52) != 0.0f) return 5;
    // backward looping play wraps below zero
    w(play, 4) = 0; f32(play, 40) = -1.0f; i32(play, 44) = 0; f32(play, 52) = 0.0f; i64(play, 64) = 5003; clockValue = 5005;
    i32(anim, 20) = 0;
    ProcessPlayTiming__9VIHSpriteiR7VIScene(self, 1, arena);
    if (i32(play, 44) != 8 || i32(play, 48) != 7) return 6;
    // timed play: mode 1 computes progress and ends when the duration has passed
    w(play, 12) = 1; i64(play, 16) = 5000; i64(play, 24) = 10; f32(play, 72) = 0.0f; clockValue = 5004; i64(play, 64) = 5004;
    ProcessPlayTiming__9VIHSpriteiR7VIScene(self, 1, arena);
    if (f32(play, 32) != 0.4f || w(play, 12) != 1) return 7;
    clockValue = 5011;
    ProcessPlayTiming__9VIHSpriteiR7VIScene(self, 1, arena);
    if (w(play, 12) != 0) return 8;
    w(play, 12) = 0; w(play, 32) = 99;
    ProcessPlayTiming__9VIHSpriteiR7VIScene(self, 1, arena);
    if (w(play, 32) != 0) return 9;
    return 0;
}
