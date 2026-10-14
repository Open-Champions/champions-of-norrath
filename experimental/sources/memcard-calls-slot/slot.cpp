// Observed: MCA_Slot at 0x00485c04 is 4 bytes, initial value 0, in .sdata.
// Inferred: nothing.
// Unknown: which startup store first publishes a slot other than the 0
// written by MCA_Init.
extern "C" {
int MCA_Slot;
}
