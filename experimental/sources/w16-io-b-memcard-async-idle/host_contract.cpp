#include "async_idle.cpp"

#include <cstdio>
#include <vector>

int sAsyncMode, sAsyncFd;

struct Step { int ret; int value; };
static std::vector<Step> steps;
static unsigned next_step;
static int syncs, flushes, closes, last_flush_fd, last_close_fd;
static std::vector<int> modes;

extern "C" int sceMcSync(int mode, int* command, int* result) {
    modes.push_back(mode);
    *command = 1;
    Step s = next_step < steps.size() ? steps[next_step] : Step{1, 0};
    ++next_step;
    ++syncs;
    *result = s.value;
    return s.ret;
}
extern "C" int sceMcFlush(int fd) { ++flushes; last_flush_fd = fd; return 0; }
extern "C" int sceMcClose(int fd) { ++closes; last_close_fd = fd; return 0; }

static int run(std::vector<Step> s, int fd, int mode) {
    steps = s;
    next_step = 0;
    syncs = flushes = closes = 0;
    modes.clear();
    sAsyncFd = fd;
    sAsyncMode = mode;
    return VIMemCardAsyncIdle__Fv();
}

int main() {
    // Still running or no command: only the poll runs.
    if (run({{-1, 5}}, 7, 0) != 1 || syncs != 1 || modes[0] != 1 || sAsyncFd != 7) return 1;
    if (run({{0, 5}}, 7, 0) != 0 || syncs != 1 || sAsyncFd != 7) return 2;
    // Negative result: descriptor cleared.
    if (run({{1, -4}}, 7, 0) != -1 || sAsyncFd != -1 || closes != 0) return 3;
    // No descriptor open.
    if (run({{1, 9}}, -1, 0) != 1 || closes != 0 || sAsyncFd != -1) return 4;
    // Mode zero: result becomes the return value, zero becomes -1.
    if (run({{1, 9}, {1, 0}}, 7, 0) != 9 || closes != 1 || last_close_fd != 7 || flushes != 0 || sAsyncFd != -1) return 5;
    if (modes.size() != 2 || modes[0] != 1 || modes[1] != 0) return 6;
    if (run({{1, 0}, {1, 0}}, 7, 0) != -1 || sAsyncFd != -1) return 7;
    // Mode nonzero: flush and blocking sync, pending value stays 1.
    if (run({{1, 9}, {1, 4}, {1, 0}}, 7, 2) != 1 || flushes != 1 || last_flush_fd != 7 || closes != 1 || syncs != 3) return 8;
    if (modes[1] != 0 || modes[2] != 0) return 9;
    // Mode nonzero, flush sync reports an error: no close.
    if (run({{1, 9}, {1, -2}}, 7, 2) != -1 || closes != 0 || sAsyncFd != -1) return 10;
    // Close sync reports an error.
    if (run({{1, 9}, {1, -2}}, 7, 0) != -1 || closes != 1 || sAsyncFd != -1) return 11;
    std::puts("memcard async-idle passed");
    return 0;
}
