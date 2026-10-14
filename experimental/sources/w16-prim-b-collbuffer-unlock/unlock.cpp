// Observed: Unlock__12VICollBuffer at 0x0108ba80 is 28 bytes. It reads the word at offset 12; if
// that word is nonzero it stores zero there and returns 0, otherwise it returns -1 and leaves the
// object unchanged.
// Inferred: offset 12 is the same "locked" flag that Lock__12VICollBuffer sets (it stores 1 when
// the word is zero). Unknown: whether any other field is touched elsewhere on unlock.
extern "C" {
int Unlock__12VICollBuffer(unsigned char* self) {
    int* flag = reinterpret_cast<int*>(self + 12);
    if (*flag == 0)
        return -1;
    *flag = 0;
    return 0;
}
}
