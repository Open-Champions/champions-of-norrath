// Host contract for plotRouteTask__Fv. Not an EE plan input.
extern "C" signed char cutMode = 0;
static int calls, last = -1;
extern "C" void worldPlotRouteProcess__Fi(int a) { ++calls; last = a; }
#include "plot_route_task.cpp"
int main() {
    cutMode = 1; plotRouteTask__Fv();
    if (calls != 0) return 1;
    cutMode = -1; plotRouteTask__Fv();
    if (calls != 0) return 2;
    cutMode = 0; plotRouteTask__Fv();
    if (calls != 1 || last != 0) return 3;
    return 0;
}
