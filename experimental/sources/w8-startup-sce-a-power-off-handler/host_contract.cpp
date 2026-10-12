#include "power_off_handler.cpp"

#include <cstdio>

unsigned char powerOffMode = 0;
static int ticks_seen, calls, mode_at_call = -1;
static int (*handler_seen)(int, unsigned short, void*);
static void* arg_seen = &calls;

extern "C" int Bounce__FiUsPv(int, unsigned short, void*) { return 0; }
extern "C" int iSetAlarm(int ticks, int (*handler)(int, unsigned short, void*), void* argument) {
    ++calls;
    ticks_seen = ticks;
    handler_seen = handler;
    arg_seen = argument;
    mode_at_call = powerOffMode;
    return 99;
}

int main() {
    PowerOffHandler__FPv(&calls);
    if (calls != 1 || ticks_seen != 1000 || handler_seen != Bounce__FiUsPv || arg_seen != nullptr) return 1;
    if (mode_at_call != 0 || powerOffMode != 1) return 2;   // the store follows the call
    std::puts("power off handler contract passed");
    return 0;
}
