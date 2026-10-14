// Observed _close_r at 0x003094d0. It clears the errno word, calls close,
// and copies a nonzero errno into the reentrancy object only when close
// returns -1.
extern "C" unsigned char norrath_player_syscall_errno_word[];
extern "C" int close(int descriptor);

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

extern "C" int norrath_factory_close_reentrant(unsigned char* reent, int descriptor) {
    write_word(norrath_player_syscall_errno_word, 0u);
    const int result = close(descriptor);
    if (result == -1) {
        const unsigned code = read_word(norrath_player_syscall_errno_word);
        if (code != 0u) write_word(reent, code);
    }
    return result;
}
