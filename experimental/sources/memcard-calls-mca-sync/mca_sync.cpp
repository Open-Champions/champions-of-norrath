// Observed: MCA_Sync__FPi at 0x0025c748 is 72 bytes. It calls MCA_Process
// with the same pointer. A nonzero word at that pointer returns 1. A zero
// word repeats while MCA_Process returns nonzero, then returns 0.
// Inferred: nothing about MCA_Process.
// Unknown: which store writes the word.

extern "C" int MCA_Process__FPi(int* error);

extern "C" int MCA_Sync__FPi(int* error) {
    int result;
    do {
        result = MCA_Process__FPi(error);
        if (*error != 0) return 1;
    } while (result != 0);
    return 0;
}
