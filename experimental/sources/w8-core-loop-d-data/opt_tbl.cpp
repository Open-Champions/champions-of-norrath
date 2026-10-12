// Observed optTbl at 0x007d0ca0, a local symbol; the next symbol is 40 bytes later. Every file byte is zero, so the definition is
// zero-initialized storage. Ten words is the whole gap, not a recorded count. Unknown: how many entries are used.
extern "C" {
unsigned optTbl[10] = {};
}
