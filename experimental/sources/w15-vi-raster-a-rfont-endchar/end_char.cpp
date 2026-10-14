// Observed: EndChar__7VIRFont at 0x011360f8 is 20 bytes in the pinned SLUS-20565 image. It adds one to the word at offset 36 (0x24)
// of the font object and returns 0.
// Inferred: the word is the current character index that BeginChar__7VIRFont uses to pick a 12-byte record; the zero is an int status.
// Unknown: the range of the index and whether it is reset between strings. Nothing here ran on the EE.
extern "C" {
int EndChar__7VIRFont(unsigned char* self) {
    *reinterpret_cast<int*>(self + 36) += 1;
    return 0;
}
}
