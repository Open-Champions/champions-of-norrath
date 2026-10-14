// Observed: VIMemCardIsPrepared__Fv at 0x010d4040 is 8 bytes. It returns 1 and
// reads nothing.
// Inferred: nothing.
// Unknown: why the call exists, since it never reports an unprepared card.
extern "C" int VIMemCardIsPrepared__Fv() {
    return 1;
}
