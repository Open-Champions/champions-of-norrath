// Observed: MCA_Busy__Fv at 0x0025c700 is 72 bytes. When the word checkResults equals 1 it
// returns 1 without any call. Otherwise it calls sceMcSync(1, 0, 0). A zero result from that call
// returns 1. A nonzero result returns 1 when commandQueueEmpty__Fv returns zero, else 0.
// Inferred: nothing.
// Unknown: whether checkResults == 1 is a documented state or a private flag.

extern "C" int checkResults;
extern "C" int sceMcSync(int mode, int* command, int* result);
extern "C" int commandQueueEmpty__Fv();

extern "C" int MCA_Busy__Fv() {
    if (checkResults == 1) {
        return 1;
    }
    int busy = 1;
    if (sceMcSync(1, 0, 0) != 0) {
        busy = commandQueueEmpty__Fv() == 0;
    }
    return busy;
}
