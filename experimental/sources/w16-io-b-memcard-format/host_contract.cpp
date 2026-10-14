#include "format.cpp"

#include <cstdio>

static int format_calls, sync_calls, format_port, format_slot, sync_mode;
static int next_result;
static int* seen_cmd;
static int* seen_result;

extern "C" int sceMcFormat(int port, int slot) {
    ++format_calls;
    format_port = port;
    format_slot = slot;
    return 123;
}

extern "C" int sceMcSync(int mode, int* cmd, int* result) {
    ++sync_calls;
    sync_mode = mode;
    seen_cmd = cmd;
    seen_result = result;
    *cmd = 9;
    *result = next_result;
    return -77;
}

int main() {
    const int results[] = {0, 1, -1, -5, 0x10000, 0x7fffffff};
    for (int r : results) {
        format_calls = sync_calls = 0;
        next_result = r;
        int got = VIMemCardFormat__Fv();
        if (got != (r == 0 ? 0 : -1)) return 1;
        if (format_calls != 1 || sync_calls != 1) return 2;
        if (format_port != 0 || format_slot != 0 || sync_mode != 0) return 3;
        if (seen_cmd == nullptr || seen_result == nullptr || seen_cmd == seen_result) return 4;
    }
    std::puts("memcard format passed");
    return 0;
}
