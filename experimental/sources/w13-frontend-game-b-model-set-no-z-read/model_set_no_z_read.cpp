// Observed modelSetNoZRead__Fv at 0x0016cc78, extent 16. Sets bit 5 (0x20) of the
// word modelOption (0x0048408c) with an OR and leaves every other bit unchanged.
// Returns nothing. modelSetNoZWrite__Fv sets bit 0 of the same word and
// modelResetOptionalParams__Fv clears it.
// Inferred: the name says bit 5 disables depth reads. Unknown: the meaning of
// the other bits. modelOption is defined by another package.
extern "C" {
extern int modelOption;
void modelSetNoZRead__Fv() {
    modelOption |= 0x20;
}
}
