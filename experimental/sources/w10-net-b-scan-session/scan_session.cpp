// Observed uninitialised words, symbols without size, gap 4 to the next symbol:
//   scanMask    0x004869ac
//   sessionMark 0x00486c44
// Both are read as 32-bit ints by authored code. Unknown: meaning of either.
extern "C" {
int scanMask;
int sessionMark;
}
