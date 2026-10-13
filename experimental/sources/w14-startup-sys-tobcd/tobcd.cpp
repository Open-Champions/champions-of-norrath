// Observed tobcd at 0x002d1330, 44 bytes, a local routine of the system-configuration (Scf) library.
// It takes the low byte of its argument register (the routine masks the register itself; the parameter
// is declared wider than a byte so the compiler keeps that mask), divides it by ten with an unsigned register divide and returns
// the byte plus six times the quotient, truncated to a byte: a binary value below 100 becomes packed
// decimal. The original keeps the divisor in a register and keeps the divide-by-zero check of the
// compiler; the divisor is the constant ten so the check can never fire.
// Values of 100 and more are not rejected; the result wraps at a byte.
// Callers: converttobcd, once per clock-record byte.
extern "C" {
unsigned char tobcd(unsigned value) {
    unsigned v = value & 0xff;
    return static_cast<unsigned char>(v + (v / 10) * 6);
}
}
