// Observed _lseek_r at 0x0030af30. The offset travels in one 64-bit register.
// Errno is cleared before lseek. A nonzero errno is copied into the
// reentrancy object only when the full 64-bit result is -1.
extern "C" unsigned char norrath_player_syscall_errno_word[];
extern "C" long long lseek(int descriptor, long long offset, int whence);

namespace {
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
}

extern "C" long long norrath_factory_seek_reentrant(unsigned char* reent, int descriptor, long long offset, int whence) {
    write_word(norrath_player_syscall_errno_word, 0u);
    const long long result = lseek(descriptor, offset, whence);
    if (result == -1) {
        const unsigned code = read_word(norrath_player_syscall_errno_word);
        if (code != 0u) write_word(reent, code);
    }
    return result;
}
