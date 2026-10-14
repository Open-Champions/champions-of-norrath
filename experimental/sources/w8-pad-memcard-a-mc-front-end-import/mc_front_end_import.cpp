// Observed: MC_FrontEndImport at 0x00485be1 is 1 bytes, one signed byte of zero in the executable
// image. The defined object here has that extent and is zero-initialised.
// Inferred: nothing.
// Unknown: the meaning of values other than zero.

extern "C" {
signed char MC_FrontEndImport;
}
