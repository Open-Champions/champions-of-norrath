// Observed: EndPrimGroup__12VICollBuffer at 0x0108bb20 is 20 bytes. It increments the word at
// offset 44 of the object by one and returns 0.
// Inferred: offset 44 counts completed primitive groups; BeginPrimGroup indexes a 32-byte-record
// array with the same word. Unknown: any upper bound (none is checked here).
extern "C" {
int EndPrimGroup__12VICollBuffer(unsigned char* self) {
    *reinterpret_cast<int*>(self + 44) += 1;
    return 0;
}
}
