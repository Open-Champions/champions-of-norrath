// Observed: MCA_Reset__Fv at 0x0025c6e8 is 24 bytes. It stores zero in
// queueEnd, -1 in clearBytes, zero in queueStart and zero in checkResults,
// each as a word. It has no arguments and no return value.
// Inferred: nothing.
// Unknown: what the four words mean to the card-access queue beyond their names.

extern "C" int queueStart;
extern "C" int queueEnd;
extern "C" int checkResults;
extern "C" int clearBytes;

extern "C" void MCA_Reset__Fv() {
    queueEnd = 0;
    clearBytes = -1;
    queueStart = 0;
    checkResults = 0;
}
