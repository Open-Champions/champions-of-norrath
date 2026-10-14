// Observed isceSifSendCmd at 0x002d76d0, 60 bytes. It forwards to _sceSifSendCmd
// (0x002d7558) with its six arguments moved one place up and the constant 1
// inserted as the second argument: (a, b, c, d, e, f) becomes
// (a, 1, b, c, d, e, f). The callee's result is returned unchanged.
// Unknown: the meaning of the six arguments and of the constant, and the body
// of _sceSifSendCmd.
extern "C" {
int _sceSifSendCmd(int a, int mode, int b, int c, int d, int e, int f);

int isceSifSendCmd(int a, int b, int c, int d, int e, int f) {
    return _sceSifSendCmd(a, 1, b, c, d, e, f);
}
}
