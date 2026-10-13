// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp
// The stand-in printf, min and callback record what the routine does; each case below fails on a
// wrong error code, capacity result, frame/output arithmetic, callback order or buffer clearing.
#include <cstdio>
#include <cstring>
#include <cstdarg>
#define printf host_printf
#define memset host_memset
#define memcpy host_memcpy
#include "lg_frame_encode.cpp"
#undef printf
#undef memset
#undef memcpy
static char said[128]; static int said_a, said_b, said_count;
extern "C" int host_printf(const char* f, ...) {
    va_list v; va_start(v, f); std::snprintf(said, sizeof said, "%s", f); said_a = va_arg(v, int); said_b = va_arg(v, int); va_end(v);
    ++said_count; return 0;
}
extern "C" void* host_memset(void* d, int c, unsigned n) { return std::memset(d, c, n); }
extern "C" void* host_memcpy(void* d, const void* s, unsigned n) { return std::memcpy(d, s, n); }
extern "C" int min(int a, int b) { return a < b ? a : b; }
static int fn_calls; static short fn_frames[8][8]; static long fn_out[8]; static void* fn_obj[8]; static int fn_ret = 3;
static unsigned char* out_base;
static int callback(void* frame, void* out, void* object) {
    std::memcpy(fn_frames[fn_calls], frame, 8); fn_out[fn_calls] = (unsigned char*)out - out_base; fn_obj[fn_calls] = object;
    ((unsigned char*)out)[0] = 0x70 + fn_calls;
    return fn_ret + fn_calls++;
}
static int fails;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
static short samples[32]; static unsigned char out[128]; static short frame[8]; static int cap, fill, obj;
static void reset(int capacity, int f) {
    for (int i = 0; i < 32; ++i) samples[i] = (short)(i + 1);
    std::memset(out, 0xee, sizeof out); std::memset(frame, 0, sizeof frame);
    cap = capacity; fill = f; fn_calls = 0; said_count = 0; out_base = out;
}
int main() {
    int r;
    reset(100, 0);
    r = lgFrameEncode(nullptr, 20, out, &cap, frame, 8, &fill, 4, 3, callback, &obj); CHECK(r == (int)0x80000004 && cap == 0 && fn_calls == 0 && out[0] == 0xee);
    reset(100, 0);
    r = lgFrameEncode(samples, 20, nullptr, &cap, frame, 8, &fill, 4, 3, callback, &obj); CHECK(r == (int)0x80000004 && cap == 0);
    reset(100, 0);
    r = lgFrameEncode(samples, 21, out, &cap, frame, 8, &fill, 4, 3, callback, &obj); CHECK(r == (int)0x80000004 && cap == 0 && out[0] == 0xee);
    reset(100, 0);
    r = lgFrameEncode(samples, 0, out, &cap, frame, 8, &fill, 4, 3, callback, &obj); CHECK(r == 0 && cap == 0 && out[0] == 0xee);
    reset(0, 0);
    r = lgFrameEncode(samples, 20, out, &cap, frame, 8, &fill, 4, 3, callback, &obj); CHECK(r == 0 && cap == 0 && out[0] == 0xee && fn_calls == 0);

    // two whole frames: ten samples, four per frame, three output bytes per frame
    reset(100, 0);
    r = lgFrameEncode(samples, 20, out, &cap, frame, 8, &fill, 4, 3, callback, &obj);
    CHECK(r == 0 && fn_calls == 2 && cap == 3 + 4 && fill == 2);
    CHECK(fn_frames[0][0] == 1 && fn_frames[0][3] == 4 && fn_frames[1][0] == 5 && fn_frames[1][3] == 8);
    CHECK(fn_out[0] == 0 && fn_out[1] == 3 && fn_obj[0] == &obj && fn_obj[1] == &obj);
    CHECK(out[0] == 0x70 && out[3] == 0x71 && out[1] == 0 && out[2] == 0 && out[99] == 0 && out[100] == 0xee);
    CHECK(frame[0] == 9 && frame[1] == 10 && frame[2] == 0 && frame[3] == 0);   // cleared after each callback
    CHECK(said_count == 0);

    // one frame pending: the sample run only tops it up
    reset(100, 3);
    frame[0] = 91; frame[1] = 92; frame[2] = 93;
    r = lgFrameEncode(samples, 4, out, &cap, frame, 8, &fill, 4, 3, callback, &obj);
    // cap % 3 == 1 with a non-zero fill adds a frame to the estimate (0 + 1 = 1 frame = 3 bytes, fits)
    CHECK(r == 0 && fn_calls == 1 && fn_frames[0][0] == 91 && fn_frames[0][2] == 93 && fn_frames[0][3] == 1);
    CHECK(fill == 1 && frame[0] == 2 && frame[1] == 0 && cap == 3);

    // capacity that does not fit: message, error, nothing cleared or called
    reset(5, 0);
    r = lgFrameEncode(samples, 20, out, &cap, frame, 8, &fill, 4, 3, callback, &obj);
    CHECK(r == (int)0x80000004 && cap == 0 && fn_calls == 0 && out[0] == 0xee && said_count == 1);
    CHECK(std::strcmp(said, "ERROR: bytes provided = %d. Need=%d\n") == 0 && said_a == 5 && said_b == 6);
    reset(6, 0);
    r = lgFrameEncode(samples, 20, out, &cap, frame, 8, &fill, 4, 3, callback, &obj);
    CHECK(r == 0 && said_count == 0 && fn_calls == 2);
    // non-zero fill and a capacity that is not a multiple of the frame bytes adds one frame
    reset(7, 2);
    r = lgFrameEncode(samples, 8, out, &cap, frame, 8, &fill, 4, 3, callback, &obj);  // 4 samples: 1 frame + 1 = 2 frames = 6 <= 7
    CHECK(said_count == 0);
    reset(6, 2);
    r = lgFrameEncode(samples, 8, out, &cap, frame, 8, &fill, 4, 3, callback, &obj);  // 6 % 3 == 0: 1 frame
    CHECK(said_count == 0 && fn_calls == 1);
    reset(4, 2);
    r = lgFrameEncode(samples, 8, out, &cap, frame, 8, &fill, 4, 3, callback, &obj);  // 4 % 3 != 0: 2 frames = 6 > 4
    CHECK(r == (int)0x80000004 && said_count == 1 && said_a == 4 && said_b == 6);
    // output buffer is cleared for the full capacity before the first callback writes
    reset(9, 0);
    r = lgFrameEncode(samples, 4, out, &cap, frame, 8, &fill, 4, 3, callback, &obj);
    CHECK(r == 0 && fn_calls == 0 && out[8] == 0 && out[9] == 0xee && fill == 2 && cap == 0);
    if (fails) return 1;
    std::printf("lgFrameEncode cases passed\n");
    return 0;
}
