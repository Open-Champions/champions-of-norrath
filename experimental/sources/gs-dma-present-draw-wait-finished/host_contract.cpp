#include "draw_wait_finished.cpp"

#include <cstdio>

extern "C" int endDrawSemaphore = 0;
static int waits;
static int seen_id;

extern "C" void WaitSema(int identifier) {
    waits += 1;
    seen_id = identifier;
}

int main() {
    endDrawSemaphore = 0x186a0;
    waits = 0;
    drawWaitFinished__Fv();
    if (waits != 1 || seen_id != 0x186a0) return 1;
    endDrawSemaphore = -4;
    drawWaitFinished__Fv();
    if (waits != 2 || seen_id != -4) return 2;
    std::puts("draw wait finished contract passed");
    return 0;
}
