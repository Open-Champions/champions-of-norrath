// Host contract for worldPlotRouteSetEnable__Fi. Not an EE plan input.
#include <cstdio>
unsigned char routeEnable = 7;
extern "C" void worldPlotRouteSetEnable__Fi(int);
int main() {
    worldPlotRouteSetEnable__Fi(0);
    if (routeEnable != 0) return 1;
    worldPlotRouteSetEnable__Fi(1);
    if (routeEnable != 1) return 2;
    worldPlotRouteSetEnable__Fi(0x1ff);              // only the low byte is kept
    if (routeEnable != 0xff) return 3;
    std::printf("world plot route set enable contract passed\n");
    return 0;
}
