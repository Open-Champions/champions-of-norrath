// Host contract for the route queue and switch objects. Not an EE plan input.
#include <cstdio>
extern "C" {
extern unsigned char routeEnable;
extern int rqHead, rqTail, nodecount;
extern unsigned char routeQueue[128 * 48];
}
int main() {
    if (routeEnable != 1 || rqHead != 0 || rqTail != 0 || nodecount != 0) return 1;
    for (unsigned i = 0; i != sizeof routeQueue; ++i) if (routeQueue[i] != 0) return 2;
    routeEnable = 0; rqHead = 5; routeQueue[sizeof routeQueue - 1] = 7;
    if (routeEnable != 0 || rqHead != 5 || routeQueue[6143] != 7) return 3;
    std::printf("route state contract passed\n");
    return 0;
}
