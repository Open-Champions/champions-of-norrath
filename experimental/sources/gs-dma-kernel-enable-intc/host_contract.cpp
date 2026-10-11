#define NORRATH_HOST_CONTRACT 1
#include "enable_intc.cpp"

#include <cstdio>
#include <cstring>

extern "C" unsigned norrath_host_status_word = 0;
static char log_text[8];
static int log_used;
static int seen_service;
static int seen_cause;
static unsigned long long service_result;

static void mark(char step) {
    if (log_used < 7) log_text[log_used++] = step;
    log_text[log_used] = 0;
}

extern "C" int DIntr() {
    mark('D');
    return 1;
}
extern "C" int EIntr() {
    mark('E');
    return 1;
}
extern "C" unsigned long long norrath_host_intc_service(int service, int cause) {
    mark('S');
    seen_service = service;
    seen_cause = cause;
    return service_result;
}
extern "C" void norrath_host_pipeline_sync() { mark('Y'); }

static void reset(unsigned status, unsigned long long result) {
    norrath_host_status_word = status;
    service_result = result;
    log_used = 0;
    log_text[0] = 0;
    seen_service = 0;
    seen_cause = -1;
}

int main() {
    reset(0u, 9ull);
    if (EnableIntc(2) != 9ull || std::strcmp(log_text, "SY") != 0) return 1;
    if (seen_service != 20 || seen_cause != 2) return 2;
    reset(0x10000u, 0x11ull);
    if (EnableIntc(11) != 0x11ull || std::strcmp(log_text, "DSYE") != 0) return 3;
    if (seen_cause != 11) return 4;
    reset(0x10001u, 4ull);
    if (EnableIntc(0) != 4ull || std::strcmp(log_text, "DSYE") != 0 || seen_cause != 0) return 5;
    reset(1u, 7ull);
    if (EnableIntc(-3) != 7ull || std::strcmp(log_text, "SY") != 0 || seen_cause != -3) return 6;
    std::puts("enable intc contract passed");
    return 0;
}
