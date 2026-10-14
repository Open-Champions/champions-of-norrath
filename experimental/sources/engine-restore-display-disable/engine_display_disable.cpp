// Observed engineDisplayDisable at 0x0048440c. Four-byte .sdata word.
// The file bytes are 03 00 00 00. _start's zero loop begins at 0x00486800
// and does not cover this word.
extern "C" {
int engineDisplayDisable = 3;
}
