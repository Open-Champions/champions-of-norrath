#include "read.cpp"

#include <cstdio>

int norrath_cdstream_pending, norrath_cdstream_status;
int norrath_cdstream_send[16];
void* last_buffer;
static int flush_calls, flush_arg, flush_result;
static int seen[5], last_at_flush;

int FlushIop__Fi(int argument) {
    ++flush_calls;
    flush_arg = argument;
    for (int i = 0; i < 5; ++i) seen[i] = norrath_cdstream_send[i];
    last_at_flush = last_buffer != nullptr;
    return flush_result;
}

int main() {
    static char buffer[8];
    norrath_cdstream_pending = 1;
    norrath_cdstream_status = 99;
    if (VICdStreamRead__FUiUiPvi(10, 20, buffer, 2) != -1 || norrath_cdstream_status != 2) return 1;
    if (flush_calls != 0 || last_buffer != nullptr || norrath_cdstream_send[0] != 0) return 2;
    norrath_cdstream_pending = 0;
    flush_result = -1;
    if (VICdStreamRead__FUiUiPvi(10, 20, buffer, 2) != -1 || norrath_cdstream_status != 1) return 3;
    if (norrath_cdstream_pending != 0 || flush_calls != 1 || flush_arg != 0) return 4;
    flush_result = 0;
    if (VICdStreamRead__FUiUiPvi(0x12345, 0x40, buffer, 3) != 0) return 5;
    if (norrath_cdstream_status != 0 || norrath_cdstream_pending != 1) return 6;
    if (seen[0] != 4 || seen[1] != 0x12345 || seen[2] != 0x40 || seen[3] != 3) return 7;
    if (seen[4] != static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(buffer))) return 8;
    if (last_buffer != buffer || !last_at_flush) return 9;
    std::puts("VICdStreamRead contract passed");
    return 0;
}
