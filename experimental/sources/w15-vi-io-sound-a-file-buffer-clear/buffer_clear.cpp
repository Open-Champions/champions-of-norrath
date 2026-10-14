// Observed: BufferClear__6VIFile at 0x010cb228, 32 bytes. With the file object as this, stores zero in the doublewords at +136,
// +176, +144, +152 and +160 and in the word at +168 (that order), and returns 0. Nothing is read.
// Inferred: these are the file's read/write buffer bookkeeping (BufferFlush, BufferedRead and BufferedWrite use the same area), and
// clearing them discards any buffered data without writing it.
// Unknown: the member names and which field is which; this function does not say.
extern "C" {
int BufferClear__6VIFile(unsigned char* file) {
    *reinterpret_cast<unsigned long long*>(file + 136) = 0;
    *reinterpret_cast<unsigned long long*>(file + 176) = 0;
    *reinterpret_cast<unsigned*>(file + 168) = 0;
    *reinterpret_cast<unsigned long long*>(file + 144) = 0;
    *reinterpret_cast<unsigned long long*>(file + 152) = 0;
    *reinterpret_cast<unsigned long long*>(file + 160) = 0;
    return 0;
}
}
