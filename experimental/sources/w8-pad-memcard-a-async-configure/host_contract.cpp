#include "async_configure.cpp"

#include <cstdio>
#include <cstring>

signed char whichPad = 7;
int gameBailFromGameLoop;
static unsigned short fmt[1] = {0x41};
static char key[16];
static unsigned short* fmt_seen;
static unsigned short* buf_seen;
static float args[8];
static int result, run_calls, run_mode, run_extra, mode_calls, mode_arg, mode_keep, pad_at_run;
static unsigned short* run_title;
extern "C" unsigned short* textGetString__FPc(char* k) { std::strcpy(key, k); return fmt; }
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* d, unsigned short* f, float a, float b, float c,
                                         float dd, float e, float ff, float g, float h) {
    buf_seen = d; fmt_seen = f;
    float v[8] = {a, b, c, dd, e, ff, g, h};
    for (int i = 0; i < 8; ++i) args[i] = v[i];
    for (int i = 0; i < 128; ++i) d[i] = 0;
}
extern "C" int runConfigure__FiPUsi(int m, unsigned short* t, int x) {
    ++run_calls; run_mode = m; run_title = t; run_extra = x; pad_at_run = whichPad;
    return result;
}
extern "C" int newMode__Fii(int m, int k) { ++mode_calls; mode_arg = m; mode_keep = k; return 0; }

int main() {
    result = 0;
    asyncConfigure__Fv();
    if (std::strcmp(key, "ConfigTitle") != 0 || fmt_seen != fmt) return 1;
    if (args[0] != 137.0f) return 2;
    for (int i = 1; i < 8; ++i) if (args[i] != 0.0f) return 3;
    if (run_calls != 1 || run_mode != 1 || run_title != buf_seen || run_extra != 0 || pad_at_run != 0) return 4;
    if (mode_calls != 0 || gameBailFromGameLoop != 0) return 5;
    result = 2;
    asyncConfigure__Fv();
    if (mode_calls != 0 || gameBailFromGameLoop != 0) return 6;
    result = 1;
    asyncConfigure__Fv();
    if (mode_calls != 1 || mode_arg != -1 || mode_keep != 0 || gameBailFromGameLoop != 1) return 7;
    gameBailFromGameLoop = 0;
    result = -3;
    asyncConfigure__Fv();
    if (mode_calls != 2 || gameBailFromGameLoop != 1) return 8;
    std::puts("async configure passed");
    return 0;
}
