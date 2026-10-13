// Observed worldPlotRouteSetEnable__Fi at 0x0017d5b8, 8 bytes: stores the low byte of the
// argument into routeEnable (one byte at 0x0048418c) and returns nothing.
// Inferred: routeEnable is a switch for route plotting. Unknown: who reads it.
extern unsigned char routeEnable;

extern "C" void worldPlotRouteSetEnable__Fi(int enable) {
    routeEnable = static_cast<unsigned char>(enable);
}
