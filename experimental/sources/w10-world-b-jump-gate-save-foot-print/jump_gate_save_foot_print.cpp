// Observed jumpGateSaveFootPrint__Fv at 0x002847a0, 28 bytes in SLUS_205.65, with a 4112-byte
// frame. It calls jumpGateSave__FPc with the address of a 4096-byte local buffer and returns
// whatever that call returned (the image leaves the call's result register untouched).
// Inferred: the return is an int, the byte count jumpGateSave reports (484 in the image);
// the buffer is larger than that count, and its contents are discarded here.
// Unknown: why the buffer is 4096 bytes, and whether the caller uses the result.
extern "C" int jumpGateSave__FPc(char* buffer);

extern "C" int jumpGateSaveFootPrint__Fv() {
    char buffer[4096];
    return jumpGateSave__FPc(buffer);
}
