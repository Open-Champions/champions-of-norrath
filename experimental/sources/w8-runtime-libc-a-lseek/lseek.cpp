// Observed lseek at 0x002d57d8, 8 bytes: it returns -1 and nothing else. The
// descriptor, 64-bit offset and origin are ignored and errno is not written.
// The result is the 32-bit load of -1 sign-extended to 64 bits, which is what
// norrath_factory_seek_reentrant compares against.
extern "C" long long lseek(int, long long, int) {
    return -1;
}
