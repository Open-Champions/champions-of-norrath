// Observed modelSetNoZWrite__Fv at 0x0016cc68, extent 16. Sets bit 0 of the word
// modelOption (0x0048408c) with an OR and leaves every other bit unchanged.
// Returns nothing. modelResetOptionalParams__Fv clears the same word.
// Inferred: the name says bit 0 disables depth writes. Unknown: the meaning of
// the other bits. modelOption is defined by another package.
extern "C" {
extern int modelOption;
void modelSetNoZWrite__Fv() {
    modelOption |= 1;
}
}
