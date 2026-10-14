/* Observed _raise_r at 0x00303a88, 216 bytes. Arguments: a reentrancy record and a
 * signal number. The record's first word is an error number and its word at offset
 * 468 points at a table of handler words, one per signal.
 *   - A signal number of 32 or more (unsigned compare, so negatives too) stores 22 in
 *     the error word and returns -1.
 *   - A null table pointer calls _init_signal_r(record); a nonzero result returns -1
 *     and the table pointer is read again afterwards.
 *   - The handler word for the signal is read. Zero tail-calls
 *     _kill_r(record, _getpid_r(record), signal) and returns its result. One returns 0.
 *     -1 stores 22 in the error word and returns 1. Anything else is a function:
 *     the table entry is set to zero, the function is called with the signal number,
 *     and 0 is returned.
 * raise (w9-startup-crt-e) calls it with _impure_ptr. The zero, one and -1 values are
 * the default, ignore and error markers of the usual signal interface.
 * Unknown: the rest of the record's layout. Compiled output is not claimed to match
 * the original.
 */
extern "C" {
struct Reent {
    int error_number;
    unsigned char unused_4[464];
    void (**signal_handlers)(int);
};
int _init_signal_r(Reent* reent);
int _getpid_r(Reent* reent);
int _kill_r(Reent* reent, int process, int signal_number);

int _raise_r(Reent* reent, int signal_number) {
    if (static_cast<unsigned>(signal_number) >= 32u) {
        reent->error_number = 22;
        return -1;
    }
    if (reent->signal_handlers == 0) {
        if (_init_signal_r(reent) != 0) return -1;
    }
    void (**const slot)(int) = reent->signal_handlers + signal_number;
    void (*const handler)(int) = *slot;
    if (handler == 0) return _kill_r(reent, _getpid_r(reent), signal_number);
    if (static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(handler)) == 1u) return 0;
    if (static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(handler)) == 0xffffffffu) {
        reent->error_number = 22;
        return 1;
    }
    *slot = 0;
    handler(signal_number);
    return 0;
}
}
