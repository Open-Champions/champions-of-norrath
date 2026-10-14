// Observed cutMsgHandlerFunction__FPUci at 0x0018e470, 16 bytes. It ignores both
// arguments and adds one to the word netNmReady (0x004843a4); cutStartScene clears
// that word. cutInitNet registers this function as a network message handler.
// Inferred: each arriving cutscene message counts one partner as ready.
// Unknown: the message layout; the buffer and length are never read.
extern "C" int netNmReady;
extern "C" void cutMsgHandlerFunction__FPUci(unsigned char*, int) {
    netNmReady = netNmReady + 1;
}
