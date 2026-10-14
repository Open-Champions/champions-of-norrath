#define NORRATH_HOST_CONTRACT 1
#include "reset_graph.cpp"

#include <cstdio>
#include <cstring>

extern "C" unsigned long long norrath_host_gs_csr = 0;
extern "C" unsigned long long norrath_host_gs_written = 0;
static unsigned char param[16];
static char log_text[16];
static int log_used;
static int seen_mask;
static int seen_cause;
static int seen_handler;
static int crt_interlace;
static int crt_frame;
static int crt_version;

static void mark(char step) {
    if (log_used < 15) log_text[log_used++] = step;
    log_text[log_used] = 0;
}

extern "C" unsigned char* sceGsGetGParam() { return param; }
extern "C" void GsPutIMR(int mask) { mark('I'); seen_mask = mask; }
extern "C" unsigned long long DisableIntc(int cause) { mark('D'); seen_cause = cause; return 0; }
extern "C" int RemoveIntcHandler(int cause, int handler) {
    mark('R');
    seen_cause = cause;
    seen_handler = handler;
    return 0;
}
extern "C" void SetGsCrt(int interlace, int frame, int version) {
    mark('C');
    crt_interlace = interlace;
    crt_frame = frame;
    crt_version = version;
}

static int half_at(int offset) {
    return param[offset] | (param[offset + 1] << 8);
}
static int word_at(int offset) {
    return param[offset] | (param[offset + 1] << 8) | (param[offset + 2] << 16) | (param[offset + 3] << 24);
}

static void wipe() {
    std::memset(param, 0x5a, sizeof param);
    log_used = 0;
    log_text[0] = 0;
    norrath_host_gs_csr = 0x00ab0000ull;
    norrath_host_gs_written = 0;
}

int main() {
    wipe();
    sceGsResetGraph(1, 9, 9, 9);
    if (norrath_host_gs_written != 0x100ull || norrath_host_gs_csr != 0x00ab0000ull
        || log_used != 0 || param[0] != 0x5a) return 1;

    wipe();
    sceGsResetGraph(3, 1, 2, 1);
    if (norrath_host_gs_written != 0 || norrath_host_gs_csr != 0x00ab0000ull
        || log_used != 0 || param[0] != 0x5a) return 2;
    sceGsResetGraph(-1, 1, 2, 1);
    if (log_used != 0) return 3;

    wipe();
    norrath_host_gs_csr = 0x12cd0000ull;
    std::memset(param + 8, 0, 8);
    sceGsResetGraph(0, 1, 2, 1);
    if (std::strcmp(log_text, "IC") != 0) return 4;
    if (norrath_host_gs_written != 0x200ull || norrath_host_gs_csr != 0x12cd0000ull) return 5;
    if (half_at(0) != 1 || half_at(2) != 2 || half_at(4) != 1 || half_at(6) != 0xcd) return 6;
    if (seen_mask != 0xff00 || crt_interlace != 1 || crt_frame != 2 || crt_version != 1) return 7;

    wipe();
    std::memset(param + 8, 0, 8);
    param[8] = 4;
    param[12] = 9;
    sceGsResetGraph(0, 0x10001, 0x102, 2);
    if (std::strcmp(log_text, "IDRC") != 0) return 8;
    if (seen_cause != 2 || seen_handler != 9) return 9;
    if (word_at(8) != 0 || word_at(12) != 0) return 10;
    if (half_at(0) != 1 || half_at(2) != 0x102 || half_at(4) != 1) return 11;
    if (crt_interlace != 1 || crt_frame != 2 || crt_version != 0) return 12;

    wipe();
    param[8] = 7;
    norrath_host_gs_csr = 0x00440000ull;
    sceGsResetGraph(5, 3, 0x155, 0);
    if (std::strcmp(log_text, "C") != 0) return 13;
    if (norrath_host_gs_written != 0 || norrath_host_gs_csr != 0x00440000ull) return 14;
    if (half_at(0) != 3 || half_at(2) != 0x155 || half_at(4) != 0 || half_at(6) != 0x44) return 15;
    if (param[8] != 7) return 16;
    if (crt_interlace != 1 || crt_frame != 0x55 || crt_version != 0) return 17;
    std::puts("reset graph contract passed");
    return 0;
}
