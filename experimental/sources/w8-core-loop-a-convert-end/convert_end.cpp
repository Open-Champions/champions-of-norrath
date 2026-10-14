// Observed convertEnd__Fi at 0x0024f978, 40 bytes, leaf. It returns its argument with the four
// bytes in reverse order (a 32-bit byte swap). Callers in the sound code feed it words read from
// sound headers.
// Inferred: the name suggests an endianness conversion; no other use was checked.
// Unknown: whether the original source used shifts or a table; only the result is reproduced.
extern "C" int convertEnd__Fi(int value) {
    const unsigned v = static_cast<unsigned>(value);
    return static_cast<int>((v << 24) | ((v & 0xff00u) << 8) | ((v >> 8) & 0xff00u) | (v >> 24));
}
