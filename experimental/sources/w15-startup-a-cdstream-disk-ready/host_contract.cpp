#include "disk_ready.cpp"

#include <cstdio>

int norrath_cdstream_pending, norrath_cdstream_status;
int norrath_cdstream_send[16], norrath_cdstream_recv[16];
static int flush_calls, flush_arg, flush_result, send0_at_flush;

int FlushIop__Fi(int argument) {
    ++flush_calls;
    flush_arg = argument;
    send0_at_flush = norrath_cdstream_send[0];
    return flush_result;
}

int main() {
    norrath_cdstream_pending = 3;
    norrath_cdstream_status = 99;
    if (VICdStreamDiskReady__Fv() != -1 || norrath_cdstream_status != 2) return 1;
    if (flush_calls != 0 || norrath_cdstream_send[0] != 0) return 2;
    norrath_cdstream_pending = 0;
    flush_result = -4;
    norrath_cdstream_recv[0] = 77;
    if (VICdStreamDiskReady__Fv() != -1 || norrath_cdstream_status != 1) return 3;
    if (flush_calls != 1 || flush_arg != 0 || send0_at_flush != 5) return 4;
    flush_result = 0;
    norrath_cdstream_recv[0] = 0;
    if (VICdStreamDiskReady__Fv() != 0 || norrath_cdstream_status != 0) return 5;
    flush_result = 6;
    norrath_cdstream_recv[0] = 77;
    if (VICdStreamDiskReady__Fv() != -1 || norrath_cdstream_status != 77) return 6;
    norrath_cdstream_recv[0] = -2;
    if (VICdStreamDiskReady__Fv() != -1 || norrath_cdstream_status != -2) return 7;
    std::puts("VICdStreamDiskReady contract passed");
    return 0;
}
