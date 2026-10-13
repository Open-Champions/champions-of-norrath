// Observed loadInProgress at 0x484110 in .sdata, size 1, initial value 0.
// The next symbol, huffcachepages, begins at 0x484114. Readers in the authored
// sources treat it as a signed byte state: 0 idle, then small positive values the
// texture loader counts down (see the texture load and screaming routines).
// Inferred: a byte. Unknown: the full set of states and who else writes it.
extern "C" {
signed char loadInProgress = 0;
}
