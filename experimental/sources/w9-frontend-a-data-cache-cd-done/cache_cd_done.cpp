// Observed: cacheCdDone at 0x004845be is one byte of initialized data holding 0. A call-site in
// the authored sources reads it as a signed byte after cdWait__Fv.
// Inferred: nothing.
// Unknown: who writes it besides the original cdWait path.

extern "C" {
signed char cacheCdDone = 0;
}
