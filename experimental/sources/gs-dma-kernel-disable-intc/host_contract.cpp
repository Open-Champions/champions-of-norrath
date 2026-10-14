#define NORRATH_HOST_CONTRACT 1
#include "disable_intc.cpp"

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

extern "C" int DIntr() { mark('D'); return 1; }
extern "C" int EIntr() { mark('E'); return 1; }
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
}

int main() {
    reset(0u, 3ull);
    if (DisableIntc(2) != 3ull || std::strcmp(log_text, "SY") != 0) return 1;
    if (seen_service != 21 || seen_cause != 2) return 2;
    reset(0x10000u, 8ull);
    if (DisableIntc(2) != 8ull || std::strcmp(log_text, "DSYE") != 0) return 3;
    reset(0x1u, 1ull);
    if (DisableIntc(0) != 1ull || std::strcmp(log_text, "SY") != 0) return 4;
    std::puts("disable intc contract passed");
    return 0;
}
