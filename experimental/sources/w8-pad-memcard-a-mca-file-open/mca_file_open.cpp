// Observed: MCA_FileOpen at 0x00485c08 is 4 bytes, a word of zero in the executable
// image. The defined object here has that extent and is zero-initialised.
// Inferred: nothing.
// Unknown: the exact meaning of nonzero values (the stop-card-query routine only tests it for zero).

extern "C" {
int MCA_FileOpen;
}
