// Observed: hudNetWaitMsgReady__Fv at 0x001c7f90 is 120 bytes with a 288-byte
// frame. It scans player indices from 0 below nmPlayers (signed compare) and
// stops at the first index for which netPlayerIsLocal__Fi returns nonzero; if
// none does, the index ends at nmPlayers. It then builds a two-byte message
// (byte 0 = 1, byte 1 = the low byte of the index) in a local buffer and calls
// netSendMessage__FiPUci(netWaitMsgHandler, buffer, 2). Nothing is returned.
// Inferred: nothing.
// Unknown: the buffer size (the frame leaves 256 bytes), what the message tells
// the receiving peer, and the return type of netSendMessage__FiPUci, which the
// caller ignores.

extern "C" int nmPlayers;
extern "C" int netWaitMsgHandler;
extern "C" int netPlayerIsLocal__Fi(int player);
extern "C" int netSendMessage__FiPUci(int handler, unsigned char* data, int size);

extern "C" void hudNetWaitMsgReady__Fv() {
    unsigned char message[256];
    int i = 0;
    while (i < nmPlayers) {
        if (netPlayerIsLocal__Fi(i) != 0) break;
        ++i;
    }
    message[0] = 1;
    message[1] = static_cast<unsigned char>(i);
    netSendMessage__FiPUci(netWaitMsgHandler, message, 2);
}
