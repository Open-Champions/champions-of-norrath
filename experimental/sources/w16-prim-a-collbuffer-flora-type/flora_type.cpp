// Observed: FloraType__12VICollBufferSc at 0x0108bc40 is 12 bytes. It stores the low byte of its
// argument at offset 81 of the object and returns 0.
// Inferred: the byte is a per-buffer flora (vegetation) category; the signed-char argument type
// comes from the mangled name. Unknown: the set of values and who reads the byte.
extern "C" {
int FloraType__12VICollBufferSc(unsigned char* self, signed char type) {
    self[81] = static_cast<unsigned char>(type);
    return 0;
}
}
