// Observed netLocalPadState at 0x004f3630, a local symbol with no size; the
// next symbol (g_net_buf) is 0x50 bytes later. Uninitialised (zero). The
// authored netInit clears 32 bytes of it and netGameStep addresses it as
// 8 bytes per local player, so 32 bytes are used; the 80-byte gap is kept as the
// span. Unknown: whether bytes 32..79 belong to this object.
extern "C" unsigned char netLocalPadState[80];
unsigned char netLocalPadState[80];
