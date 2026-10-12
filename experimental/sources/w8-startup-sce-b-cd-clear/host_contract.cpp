#include "cd_clear.cpp"

#include <cstdio>

unsigned char cdBusy;
unsigned char streaming;
unsigned char streamPaused;

static int waits;
static int stops;
static unsigned char busy_at_wait;
static bool order_ok = true;

extern "C" void cdWait__Fv() {
    ++waits;
    busy_at_wait = cdBusy;
    if (stops != 0) order_ok = false;
}
extern "C" int sceCdStStop() { ++stops; return 1; }

static bool run(unsigned char busy, unsigned char stream, unsigned char paused, int expected_stops) {
    waits = 0;
    stops = 0;
    cdBusy = busy;
    streaming = stream;
    streamPaused = paused;
    cdClear__Fv();
    return waits == 1 && busy_at_wait == busy && stops == expected_stops && cdBusy == 0 &&
           streaming == 0 && streamPaused == 0 && order_ok;
}

int main() {
    if (!run(0, 0, 0, 0)) return 1;
    if (!run(1, 0, 0, 0)) return 2;
    if (!run(0, 1, 0, 1)) return 3;
    if (!run(1, 0, 1, 1)) return 4;
    if (!run(1, 1, 1, 1)) return 5;
    if (!run(0, 0x40, 0x7f, 1)) return 6;
    std::puts("cdClear__Fv passed");
    return 0;
}
