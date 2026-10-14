// Observed frombcd at 0x002d1360, 32 bytes, a local routine of the system-configuration (Scf) library.
// It takes the low byte of its argument register (the routine masks the register itself; the parameter
// is declared wider than a byte so the compiler keeps that mask),
// treats it as two packed decimal digits and returns the binary value:
// the byte minus six times its high nibble, truncated to a byte. Bytes whose nibbles are not decimal digits
// are not rejected; they go through the same arithmetic.
// Callers: convertfrombcd, once per clock-record byte.
extern "C" {
unsigned char frombcd(unsigned value) {
    unsigned v = value & 0xff;
    return static_cast<unsigned char>(v - (v >> 4) * 6);
}
}
