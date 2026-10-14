#include <cstdio>

// `sync; ei` are EE instructions; the host build replaces the asm statement.
static int ended;
#define NORRATH_END_OF_HANDLER() (ended = ended + 1)
#include "alarm_handler.cpp"

static int signalled, signalled_with, result;

int iSignalSema(int semaphore) {
    ++signalled;
    signalled_with = semaphore;
    return result;
}

int main() {
    result = 0x2a;
    const unsigned back = VIEEFileDelayTh__FiUsPv(7, 0xffff, reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(41)));
    if (signalled != 1 || signalled_with != 41 || back != 0x2au || ended != 1) return 1;
    result = -1;
    if (VIEEFileDelayTh__FiUsPv(0, 0, reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(3))) != 0xffffffffu) return 2;
    if (signalled != 2 || signalled_with != 3) return 3;
    std::puts("VIEEFileDelayTh contract passed");
    return 0;
}
