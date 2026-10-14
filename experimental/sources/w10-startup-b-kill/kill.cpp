/* Observed kill at 0x002d58b8, 40 bytes. Arguments: a process identifier and a signal
 * number. When the identifier is 1 (the only one getpid reports) it calls Exit with the
 * signal number as the argument; Exit is not expected to return. In every path that
 * returns, the result is 0. Any other identifier does nothing and returns 0.
 * _kill_r calls it. Unknown: the meaning of the Exit argument (it is the signal number,
 * not an exit status chosen here). Compiled output is not claimed to match the original.
 */
extern "C" {
void Exit(int status);

int kill(int process, int signal_number) {
    if (process == 1) Exit(signal_number);
    return 0;
}
}
