// Observed: jumpGateSave__FPc at 0x002847c0 is 56 bytes. It stores the signed
// byte jumpGateDestCount, sign extended to a word, at offset 0 of the buffer
// argument, copies 480 bytes from jumpGateEntries to offset 4 with memcpy, and
// returns 484.
// Inferred: the return value is the number of bytes written (4 + 480).
// Unknown: the layout of the 480 bytes of jumpGateEntries.

extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" signed char jumpGateDestCount;
extern "C" char jumpGateEntries[];

extern "C" int jumpGateSave__FPc(char* buffer) {
    *reinterpret_cast<int*>(buffer) = jumpGateDestCount;
    memcpy(buffer + 4, jumpGateEntries, 480);
    return 484;
}
