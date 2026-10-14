#include "search_file.cpp"

#include <cstdio>
#include <cstring>

int norrath_cdstream_pending, norrath_cdstream_status;
int norrath_cdstream_send[16], norrath_cdstream_recv[16];

static int flush_calls, flush_return, flush_arg;
static char seen_name[32];

extern "C" char* strcpy(char* d, const char* s) { std::memcpy(d, s, std::strlen(s) + 1); return d; }
extern "C" int FlushIop__Fi(int a) {
    ++flush_calls;
    flush_arg = a;
    std::strcpy(seen_name, reinterpret_cast<char*>(&norrath_cdstream_send[1]));
    return flush_return;
}

static void reset() {
    norrath_cdstream_pending = 0;
    norrath_cdstream_status = 77;
    std::memset(norrath_cdstream_send, 0, sizeof norrath_cdstream_send);
    std::memset(norrath_cdstream_recv, 0, sizeof norrath_cdstream_recv);
    flush_calls = 0;
    flush_return = 0;
}

int main() {
    unsigned a = 11, b = 22;
    reset();
    norrath_cdstream_pending = 1;
    if (VICdSearchFile__FPCcPUiT1("X.BIN", &a, &b) != -1 || norrath_cdstream_status != 2) return 1;
    if (flush_calls != 0 || norrath_cdstream_send[0] != 0 || a != 11 || b != 22) return 2;
    reset();
    flush_return = -4;
    if (VICdSearchFile__FPCcPUiT1("X.BIN", &a, &b) != -1 || norrath_cdstream_status != 1) return 3;
    if (flush_calls != 1 || flush_arg != 0 || norrath_cdstream_send[0] != 3 || std::strcmp(seen_name, "X.BIN") != 0) return 4;
    if (a != 11 || b != 22) return 5;
    reset();
    norrath_cdstream_recv[0] = 5;
    norrath_cdstream_recv[1] = 100;
    norrath_cdstream_recv[2] = 200;
    if (VICdSearchFile__FPCcPUiT1("Y", &a, &b) != -1 || norrath_cdstream_status != 5 || a != 11 || b != 22) return 6;
    reset();
    norrath_cdstream_recv[1] = 0x80000001u;
    norrath_cdstream_recv[2] = 200;
    if (VICdSearchFile__FPCcPUiT1("Y", &a, &b) != 0 || norrath_cdstream_status != 0) return 7;
    if (a != 0x80000001u || b != 200 || norrath_cdstream_pending != 0) return 8;
    std::puts("cd search-file passed");
    return 0;
}
