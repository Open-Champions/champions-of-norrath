// Observed: Lock__12VICollBuffer at 0x0108ba60 is 32 bytes. It reads the word at offset 12; if
// that word is zero it stores 1 there and returns 0, otherwise it leaves the object unchanged
// and returns -1.
// Inferred: offset 12 is a "locked" flag guarding the buffer. Unknown: who clears it.
extern "C" {
int Lock__12VICollBuffer(unsigned char* self) {
    int* flag = reinterpret_cast<int*>(self + 12);
    if (*flag != 0)
        return -1;
    *flag = 1;
    return 0;
}
}
