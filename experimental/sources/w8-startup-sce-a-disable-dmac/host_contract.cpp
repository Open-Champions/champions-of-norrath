#define NORRATH_HOST_CONTRACT 1
#include "disable_dmac.cpp"

#include <cstdio>
#include <cstring>

extern "C" unsigned norrath_host_status_word = 0;
static char log_text[8];
static int log_used;
static int seen_channel;
static int service_result;

static void mark(char step) {
    if (log_used < 7) log_text[log_used++] = step;
    log_text[log_used] = 0;
}

extern "C" int DIntr() { mark('D'); return 1; }
extern "C" int EIntr() { mark('E'); return 1; }
extern "C" int _DisableDmac(int channel) {
    mark('S');
    seen_channel = channel;
    return service_result;
}
extern "C" void norrath_host_pipeline_sync() { mark('Y'); }

static void reset(unsigned status, int result) {
    norrath_host_status_word = status;
    service_result = result;
    log_used = 0;
    log_text[0] = 0;
}

int main() {
    reset(0u, 3);
    if (DisableDmac(2) != 3 || std::strcmp(log_text, "SY") != 0 || seen_channel != 2) return 1;
    reset(0x10000u, 8);
    if (DisableDmac(5) != 8 || std::strcmp(log_text, "DSYE") != 0 || seen_channel != 5) return 2;
    reset(0xfffeffffu, 1);
    if (DisableDmac(0) != 1 || std::strcmp(log_text, "SY") != 0) return 3;
    std::puts("disable dmac contract passed");
    return 0;
}
