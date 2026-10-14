// Observed ustrlen__FPUs at 0x00191998, 52 bytes.
// It counts 16-bit units before the first zero unit and returns the count.
// The count is an int and the pointer is advanced two bytes at a time.
extern "C" int ustrlen__FPUs(unsigned short* s) {
    int n = 0;
    while (*s != 0) {
        s++;
        n++;
    }
    return n;
}
