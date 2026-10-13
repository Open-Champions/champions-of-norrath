// Observed _iopnotify at 0x002e46a8, 36 bytes (a local routine). The reboot-notification handler
// that sceSifMInitRpc registers (command mask 0x80000014): when its first argument equals 1 it
// stores 0 in the word _sceSifMInitCheck (0x00447c48) and then tail-calls
// sceSifSetSreg(1, 0). Any other first argument does nothing.
// The second argument is not read. The original leaves its caller's value in the result register
// on the do-nothing path and returns whatever sceSifSetSreg returns on the other, so the result
// is not authored as meaningful. Unknown: what the argument 1 means (a reboot phase is assumed
// from the registration only), and the role of Sreg slot 1. Nothing was executed on the EE.
extern "C" {
extern int _sceSifMInitCheck;
int sceSifSetSreg(int index, int value);

void _iopnotify(int mode, void* argument) {
    (void)argument;
    if (mode == 1) {
        _sceSifMInitCheck = 0;
        sceSifSetSreg(1, 0);
    }
}
}
