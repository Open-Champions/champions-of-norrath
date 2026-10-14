#include "fs_wait_s.cpp"

int _fs_semid = -1;
static int order[4];
static int steps;
static int waited;
extern "C" void _sceFsSemInit() {
    order[steps++] = 1;
    if (_fs_semid == -1) _fs_semid = 9;   // the real routine creates the semaphore
}
extern "C" int WaitSema(int semaphore) {
    order[steps++] = 2;
    waited = semaphore;
    return -77;
}

int main() {
    // The semaphore is created before it is waited on, so the wait sees the new id.
    if (_sceFsWaitS(0) != 0) return 1;
    if (steps != 2 || order[0] != 1 || order[1] != 2) return 2;
    if (waited != 9) return 3;
    // A different argument changes nothing, and the result is 0 despite WaitSema's -77.
    _fs_semid = 4;
    if (_sceFsWaitS(123) != 0) return 4;
    if (waited != 4 || steps != 4) return 5;
    return 0;
}
