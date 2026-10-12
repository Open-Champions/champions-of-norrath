// Observed: MC_LastModeSuccessful at 0x00485bdf is 1 byte, initial value 0,
// in .sdata.
// Inferred: nothing.
// Unknown: which card path stores a nonzero success byte.
extern "C" {
unsigned char MC_LastModeSuccessful;
}
