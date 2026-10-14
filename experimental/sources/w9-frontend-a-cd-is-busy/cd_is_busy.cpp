// Observed: cdIsBusy__Fv at 0x00166d20 is 36 bytes. It reads two adjacent signed bytes, cdBusy
// (0x00483ffc) and streaming (0x00483ffd), and returns 1 when either is nonzero, else 0.
// Inferred: nothing.
// Unknown: nothing beyond what the two flags mean elsewhere.

extern "C" signed char cdBusy;
extern "C" signed char streaming;

extern "C" int cdIsBusy__Fv() {
    int busy = 0;
    if (cdBusy != 0 || streaming != 0) {
        busy = 1;
    }
    return busy;
}
