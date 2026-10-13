// Host contract for worldPlotRouteProcess__Fi. Not an EE plan input.
#include <cstdio>
#include <cstring>
int frameNm, rqHead, rqTail;
alignas(8) unsigned char routeQueue[128 * 48];
static int plotted = -1, plots = 0;
extern "C" void routePlot__FR10RouteQueue(void* e) { plotted = int((static_cast<unsigned char*>(e) - routeQueue) / 48); ++plots; }
extern "C" void worldPlotRouteProcess__Fi(int);
static void pend(int i) { *reinterpret_cast<int*>(routeQueue + i * 48 + 40) = 1; }
int main() {
    frameNm = 1; rqHead = 5; rqTail = 2; pend(2);
    worldPlotRouteProcess__Fi(0);
    if (plots != 0) return 1;                       // odd frame: nothing happens
    frameNm = 2;
    worldPlotRouteProcess__Fi(0);
    if (plots != 1 || plotted != 2 || rqTail != 2) return 2;
    rqHead = rqTail = 4;
    worldPlotRouteProcess__Fi(0);
    if (plots != 1) return 3;                       // empty queue
    std::memset(routeQueue, 0, sizeof routeQueue);
    rqHead = 6; rqTail = 2; pend(4);
    worldPlotRouteProcess__Fi(0);                   // entry 2 not pending: skip to 4
    if (plots != 2 || plotted != 4 || rqTail != 4) return 4;
    std::memset(routeQueue, 0, sizeof routeQueue);
    rqHead = 5; rqTail = 126;
    worldPlotRouteProcess__Fi(0);                   // none pending: tail runs to head
    if (plots != 2 || rqTail != 5) return 5;
    rqHead = 1; rqTail = 126; pend(0);              // wraps modulo 128
    worldPlotRouteProcess__Fi(0);
    if (plots != 3 || plotted != 0 || rqTail != 0) return 6;
    std::printf("world plot route process contract passed\n");
    return 0;
}
