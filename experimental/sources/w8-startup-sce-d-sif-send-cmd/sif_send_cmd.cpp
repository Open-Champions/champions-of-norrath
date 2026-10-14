/* Observed sceSifSendCmd at 0x002d7690, 60 bytes. Takes six arguments and
 * forwards them to the local _sceSifSendCmd (0x002d7558) as seven: the first
 * argument, then a zero, then the remaining five in order. Returns that
 * result. The roles of the arguments are decided by the callee and are not
 * claimed here.
 */
extern "C" int _sceSifSendCmd(unsigned a, unsigned zero, void* b, int c, void* d, void* e, int f);

extern "C" int sceSifSendCmd(unsigned a, void* b, int c, void* d, void* e, int f) {
    return _sceSifSendCmd(a, 0, b, c, d, e, f);
}
