// Observed: MCA_LastDirCount at 0x00485c0c is 4 bytes, initial value 0, in
// .sdata. It is not the local lastDirCount word cleared by MCA_Init.
// Inferred: nothing.
// Unknown: which command stores the directory count.
extern "C" {
int MCA_LastDirCount;
}
