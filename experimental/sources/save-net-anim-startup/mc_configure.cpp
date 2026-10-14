// Observed: MC_Configure calls stopCardQuery, then newMode with both
// arguments 0, and returns that second result.
// Inferred: nothing about either callee.
// Unknown: what the mode values mean.

extern "C" void stopCardQuery__Fv();
extern "C" int newMode__Fii(int mode, int argument);

extern "C" int MC_Configure__Fv() {
    stopCardQuery__Fv();
    return newMode__Fii(0, 0);
}
