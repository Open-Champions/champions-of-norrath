// Route queue and route switches.
//   routeEnable (0x0048418c, 1 byte, .sdata, file-backed initial value 1): written by
//     worldPlotRouteSetEnable__Fi; read by the route code. Observed; reader not authored here.
//   rqHead (0x00484194) and rqTail (0x00484198): 4 bytes each in .sdata, file-backed zero. The
//     queue indices used by worldPlotRouteProcess__Fi (mod 128).
//   routeQueue (0x006ea1a0, .bss): ELF size is 0; the next object, shootGoalP, starts 0x1800
//     bytes later, which is 128 entries of 48 bytes (the entry count and size used by the
//     authored queue reader). The extent is inferred from that gap and the reader.
//   nodecount (0x0048655c, 4 bytes, .sdata, file-backed zero): zeroed by routeClear__Fv. Not
//     one of the names the earlier route storage units use; recorded size 4. Meaning beyond
//     "a count of route nodes" is not established.
// Unknown: whether routeEnable's initial value 1 is ever read before the first write.
#if defined(__mips__)
#define NORRATH_LOADED(name) __attribute__((section(name)))
#else
#define NORRATH_LOADED(name)
#endif
extern "C" {
unsigned char routeEnable NORRATH_LOADED(".sdata") = 1;
int rqHead NORRATH_LOADED(".sdata") = 0;
int rqTail NORRATH_LOADED(".sdata") = 0;
unsigned char routeQueue[128 * 48] NORRATH_LOADED(".bss") = {0};
int nodecount NORRATH_LOADED(".sdata") = 0;
}
