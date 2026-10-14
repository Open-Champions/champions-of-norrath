// Observed: nmDeferedCalls is one 32-bit word at 0x00486b98 in zero-filled storage; nextNmDeferedCalls is
// the word immediately after it (0x00486b9c). scriptClear__Fv, scriptRun__Fv and scriptSuperClear__Fv
// store to it; scriptRun__Fv and scriptTask__Fv load it.
// Inferred: a count of deferred script calls, from the name and the integer loads and stores only.
// Unknown: the storage the count indexes and the meaning of the "nm" prefix.

extern "C" {
int nmDeferedCalls;
}
static_assert(sizeof(nmDeferedCalls) == 4, "nmDeferedCalls");
