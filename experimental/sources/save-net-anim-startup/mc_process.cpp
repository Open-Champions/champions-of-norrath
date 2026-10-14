// Observed: mcProcess calls mcHeartBeat, then returns 1 when the
// gameBailFromGameLoop word is nonzero and 0 when it is zero.
// Inferred: nothing about mcHeartBeat. The comparison is unsigned.
// Unknown: which store publishes the bail word.

extern "C" void mcHeartBeat__Fv();
extern "C" int gameBailFromGameLoop;

extern "C" int mcProcess__Fv() {
    mcHeartBeat__Fv();
    unsigned word = static_cast<unsigned>(gameBailFromGameLoop);
    return word != 0u ? 1 : 0;
}
