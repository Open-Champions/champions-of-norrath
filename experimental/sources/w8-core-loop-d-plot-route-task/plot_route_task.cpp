// Observed plotRouteTask__Fv at 0x002d0dc0, 36 bytes (local symbol). The signed
// byte cutMode (0x00484318) is read. When it is nonzero nothing else happens.
// When it is zero worldPlotRouteProcess__Fi (0x0017d500) is called with 0.
// Unknown: the meaning of the argument and of cutMode values beyond zero/nonzero.
extern "C" signed char cutMode;
extern "C" void worldPlotRouteProcess__Fi(int);

extern "C" void plotRouteTask__Fv() {
    if (cutMode == 0) worldPlotRouteProcess__Fi(0);
}
