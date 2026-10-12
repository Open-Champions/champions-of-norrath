#define NORRATH_HOST_CONTRACT 1
#include "gs_int_handler.cpp"

#include <cstdio>
#include <cstring>

static char log_text[8];
static int log_used;
static int seen_cause;

static void mark(char step) {
    if (log_used < 7) log_text[log_used++] = step;
    log_text[log_used] = 0;
}

extern "C" void reloadDMA__Fi(int cause) {
    mark('R');
    seen_cause = cause;
}
extern "C" void norrath_host_pipeline_sync() { mark('Y'); }
extern "C" void norrath_host_enable_interrupts() { mark('I'); }

int main() {
    log_used = 0;
    seen_cause = 0;
    if (gsIntHandler__Fi(7) != 1) return 1;
    if (seen_cause != -2 || std::strcmp(log_text, "RYI") != 0) return 2;
    log_used = 0;
    if (gsIntHandler__Fi(-2) != 1 || seen_cause != -2) return 3;
    std::puts("gs int handler contract passed");
    return 0;
}
