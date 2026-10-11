// Observed errno word at 0x00486df0. It is separate from the impure
// reentrancy object. The reentrant I/O wrappers clear it before the
// underlying call and copy it only after a -1 result.
extern "C" {
unsigned char norrath_player_syscall_errno_word[4];
}
