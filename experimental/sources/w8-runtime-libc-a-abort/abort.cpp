// Observed abort at 0x002ff8c0, 24 bytes. It calls raise at 0x00303bf8 with
// signal number 6, then _exit at 0x00100248 with status 1 (a tail call). It
// does not test raise's result, so a handler that returns still ends in _exit.
// Unknown: the bodies of raise and _exit are outside this unit.
extern "C" {
int raise(int signal_number);
void _exit(int status);

void abort() {
    raise(6);
    _exit(1);
}
}
