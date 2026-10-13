// Observed lockAlpha at 0x00486d08, a four-byte .bss word (the next symbol, nameGridPos, is at
// +4). The authored select-screen setup clears it. Type taken as int from that use only.
// Unknown: its meaning.
extern "C" {
int lockAlpha;
}
