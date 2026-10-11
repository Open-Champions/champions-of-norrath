// Observed: MCA_Port at 0x00485c00 is 4 bytes, initial value 0, in .sdata.
// Inferred: nothing.
// Unknown: which startup store first publishes a port other than the 0
// written by MCA_Init.
extern "C" {
int MCA_Port;
}
