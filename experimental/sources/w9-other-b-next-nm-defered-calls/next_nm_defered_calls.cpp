// Observed: nextNmDeferedCalls is one 32-bit word at 0x00486b9c in zero-filled storage, directly after
// nmDeferedCalls. scriptClear__Fv, scriptRun__Fv, scriptSuperClear__Fv and scriptAddDeferedCall__FPc
// touch it: the first three store to it, and scriptAddDeferedCall__FPc, scriptRun__Fv and
// scriptTask__Fv load it.
// Inferred: a count for the following list of deferred calls, from the name and integer loads and
// stores only.
// Unknown: how it relates to nmDeferedCalls beyond the shared writers.

extern "C" {
int nextNmDeferedCalls;
}
static_assert(sizeof(nextNmDeferedCalls) == 4, "nextNmDeferedCalls");
