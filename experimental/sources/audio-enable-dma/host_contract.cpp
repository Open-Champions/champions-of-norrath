#define NORRATH_HOST_CONTRACT 1
#include "enable_dma.cpp"

#include <cstdio>
#include <cstring>

extern "C" unsigned norrath_host_status_word = 0;
static char log_text[8];
static int log_used;
static int seen_service;
static int seen_channel;
static unsigned long long service_result;

static void mark(char step) {
    if (log_used < 7) log_text[log_used++] = step;
    log_text[log_used] = 0;
}

extern "C" int norrath_sound_disable_interrupts() {
    mark('D');
    return 1;
}
extern "C" int norrath_sound_enable_interrupts() {
    mark('E');
    return 1;
}
extern "C" unsigned long long norrath_host_enable_dma_service(int service, int channel) {
    mark('S');
    seen_service = service;
    seen_channel = channel;
    return service_result;
}
extern "C" void norrath_host_pipeline_sync() { mark('Y'); }

static void reset(unsigned status, unsigned long long result) {
    norrath_host_status_word = status;
    service_result = result;
    log_used = 0;
    log_text[0] = 0;
    seen_service = 0;
    seen_channel = 0;
}

int main() {
    reset(0u, 9ull);
    if (norrath_sound_enable_dma(5) != 9ull || std::strcmp(log_text, "SY") != 0) return 1;
    if (seen_service != 22 || seen_channel != 5) return 2;
    reset(0x10000u, 0x11ull);
    if (norrath_sound_enable_dma(5) != 0x11ull || std::strcmp(log_text, "DSYE") != 0) return 3;
    reset(0x10001u, 4ull);
    if (norrath_sound_enable_dma(-3) != 4ull || std::strcmp(log_text, "DSYE") != 0 || seen_channel != -3) return 4;
    reset(0x1u, 7ull);
    if (norrath_sound_enable_dma(0) != 7ull || std::strcmp(log_text, "SY") != 0 || seen_channel != 0) return 5;
    std::puts("dma enable orchestration passed");
    return 0;
}
