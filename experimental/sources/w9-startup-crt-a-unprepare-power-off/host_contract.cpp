#include "unprepare_power_off.cpp"

#include <cstdio>

int tid = 0;
static char log_text[4];
static int log_used;
static int seen_thread = -1;
static void (*seen_handler)(void*) = reinterpret_cast<void (*)(void*)>(1);
static int seen_argument = -1;

extern "C" int TerminateThread(int thread) {
    log_text[log_used++] = 'T';
    seen_thread = thread;
    return 0;
}
extern "C" void sceCdPOffCallback(void (*handler)(void*), int argument) {
    log_text[log_used++] = 'C';
    seen_handler = handler;
    seen_argument = argument;
}

int main() {
    tid = 0x1234;
    UnpreparePowerOff__Fv();
    if (log_used != 2 || log_text[0] != 'T' || log_text[1] != 'C') return 1;
    if (seen_thread != 0x1234 || seen_handler != nullptr || seen_argument != 0) return 2;
    if (tid != 0x1234) return 3;
    std::puts("unprepare power off contract passed");
    return 0;
}
