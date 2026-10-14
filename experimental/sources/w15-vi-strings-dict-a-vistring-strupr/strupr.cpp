// Observed: strupr__8VIStringPc at 0x0101a820 is 28 bytes. Takes a string pointer (a0), calls the
// C-library-style strupr(a0) and returns that call's result unchanged. It does no other work.
// Inferred: a static VIString wrapper around the library upper-casing routine.
// Unknown: the library routine's exact behaviour (not part of this unit) and the return type; a
// char pointer is assumed because the callee's result is passed straight back.
extern "C" {
char* strupr(char* text);

char* strupr__8VIStringPc(char* text) {
    return strupr(text);
}
}
