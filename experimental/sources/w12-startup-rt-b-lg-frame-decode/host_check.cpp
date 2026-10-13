// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp
// The stand-in callback and min record what the routine does; each case fails on a wrong error code,
// capacity result, frame accumulation, callback position or buffer clearing.
#include <cstdio>
#include <cstring>
#define memset host_memset
#define memcpy host_memcpy
#include "lg_frame_decode.cpp"
#undef memset
#undef memcpy
extern "C" void* host_memset(void* d, int c, unsigned n) { return std::memset(d, c, n); }
extern "C" void* host_memcpy(void* d, const void* s, unsigned n) { return std::memcpy(d, s, n); }
extern "C" int min(int a, int b) { return a < b ? a : b; }
static int fn_calls; static unsigned char fn_frames[8][8]; static long fn_out[8]; static void* fn_obj[8];
static unsigned char* out_base;
static int callback(void* frame, void* out, void* object) {
    std::memcpy(fn_frames[fn_calls], frame, 8); fn_out[fn_calls] = (unsigned char*)out - out_base; fn_obj[fn_calls] = object;
    ((unsigned char*)out)[0] = 0x70 + fn_calls++;
    return 99;
}
static int fails;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static unsigned char in[64], out[128], frame[8]; static int cap, fill, obj;
static void reset(int capacity, int f) {
    for (int i = 0; i < 64; ++i) in[i] = (unsigned char)(i + 1);
    std::memset(out, 0xee, sizeof out); std::memset(frame, 0, sizeof frame);
    cap = capacity; fill = f; fn_calls = 0; out_base = out;
}
// two samples per frame (four output bytes), three input bytes per frame
static int run(int count, const unsigned char* i = in, unsigned char* o = out) {
    return lgFrameDecode(i, count, o, &cap, frame, 0, &fill, 2, 3, callback, &obj);
}
int main() {
    int r;
    reset(8, 0); r = run(6, nullptr); CHECK(r == (int)0x80000004 && cap == 0 && fn_calls == 0 && out[0] == 0xee);
    reset(8, 0); r = run(6, in, nullptr); CHECK(r == (int)0x80000004 && cap == 0);
    reset(9, 0); r = run(6); CHECK(r == (int)0x80000004 && cap == 0 && out[0] == 0xee);      // odd capacity
    reset(8, 0); r = run(0); CHECK(r == 0 && cap == 0 && out[0] == 0xee);
    reset(0, 0); r = run(6); CHECK(r == 0 && cap == 0 && out[0] == 0xee);
    reset(8, 0); r = run(2); CHECK(r == 0 && cap == 0 && out[0] == 0xee && fn_calls == 0 && fill == 0);   // under one frame

    // two whole frames with room for both
    reset(8, 0); r = run(6);
    CHECK(r == 0 && fn_calls == 2 && cap == 8 && fill == 0);
    CHECK(fn_frames[0][0] == 1 && fn_frames[0][2] == 3 && fn_frames[1][0] == 4 && fn_frames[1][2] == 6);
    CHECK(fn_out[0] == 0 && fn_out[1] == 4 && fn_obj[0] == &obj && fn_obj[1] == &obj);
    CHECK(out[0] == 0x70 && out[4] == 0x71 && out[1] == 0 && out[7] == 0 && out[8] == 0xee);

    // the remaining capacity drops by the running total, so a third frame is refused although it fits
    reset(12, 0); r = run(9);
    CHECK(r == 0 && fn_calls == 2 && cap == 8 && fill == 3 && out[8] == 0 && out[11] == 0 && out[12] == 0xee);

    // a trailing partial frame stays in the frame buffer
    reset(8, 0); r = run(7);
    CHECK(r == 0 && fn_calls == 2 && cap == 8 && fill == 1 && frame[0] == 7 && frame[1] == 5);

    // bytes already in the frame buffer are completed first
    reset(4, 2); frame[0] = 90; frame[1] = 91; r = run(3);
    CHECK(r == 0 && fn_calls == 1 && fn_frames[0][0] == 90 && fn_frames[0][1] == 91 && fn_frames[0][2] == 1 && cap == 4 && fill == 0);

    // capacity limits the input that is used
    reset(4, 0); r = run(6);
    CHECK(r == 0 && fn_calls == 1 && cap == 4 && fill == 1 && frame[0] == 4 && out[3] == 0 && out[4] == 0xee);
    reset(2, 0); r = run(6);
    CHECK(r == 0 && fn_calls == 0 && cap == 0 && fill == 0 && out[1] == 0 && out[2] == 0xee);
    if (fails) return 1;
    std::printf("lgFrameDecode cases passed\n");
    return 0;
}
