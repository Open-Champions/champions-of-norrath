/* Observed _kill_r at 0x00303c80, 92 bytes. Arguments: a reentrancy record, a process
 * identifier and a signal number. The global error word (0x00486df0, written elsewhere
 * under norrath_player_syscall_errno_word) is cleared, then kill(process, signal) is
 * called. When the result is -1 and the error word is nonzero, the first word of the
 * record takes the error word. The kill result is returned in every case.
 * _raise_r tail-calls it with the process identifier from _getpid_r.
 * Unknown: what kill does on the target. Compiled output is not claimed to match the
 * original.
 */
extern "C" {
extern unsigned char norrath_player_syscall_errno_word[];
int kill(int process, int signal_number);

int _kill_r(unsigned* reent, int process, int signal_number) {
    volatile unsigned* const error = reinterpret_cast<volatile unsigned*>(norrath_player_syscall_errno_word);
    *error = 0;
    const int result = kill(process, signal_number);
    if (result == -1) {
        const unsigned code = *error;
        if (code != 0) *reent = code;
    }
    return result;
}
}
