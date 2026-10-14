#include "fs_sig_sema.cpp"

int _fs_semid = 12;
static int signalled = -1;
static int signal_result = 0;
extern "C" int SignalSema(int semaphore) {
    signalled = semaphore;
    return signal_result;
}

int main() {
    signal_result = 0;
    if (_sceFsSigSema() != 0 || signalled != 12) return 1;
    // The id is read at call time, and the result of SignalSema is passed through.
    _fs_semid = -1;
    signal_result = -5;
    if (_sceFsSigSema() != -5 || signalled != -1) return 2;
    return 0;
}
