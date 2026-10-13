/* Observed sceTtyInit at 0x002d66b8, 184 bytes. Takes no argument (the
 * original zeroes $a0 for its first call; sceTtyInit's caller write passes
 * one that is ignored). Calls FlushCache(0), then
 * tinfo.socket = sceDeci2Open(528, &tinfo, handler) where handler is the local
 * routine at 0x002d62d8 (named sceTtyHandler in the symbol table). A negative
 * socket returns 0 with nothing else done. Otherwise it clears the busy,
 * length and received words of tinfo, sets the send cursor and receive buffer
 * pointers to wbuf and rbuf OR-ed with 0x20000000, stores 528, 'E' (69) and
 * 'H' (72) at offsets 4, 6 and 7 of wbuf, and zero at offsets 2 and 8,
 * stores QueueInit(256) in the queue pointer and returns 1. Only $a0 of the
 * QueueInit call is meaningful; the original has other registers live there.
 */
#include "../w9-startup-crt-f-tty-state/tty_state.hpp"

extern "C" {
void FlushCache(int mode);
int sceDeci2Open(int protocol, void* option, void (*handler)());
TtyQueue* QueueInit(int size);
void sceTtyHandler();

int sceTtyInit() {
    FlushCache(0);
    tinfo.socket = sceDeci2Open(528, &tinfo, sceTtyHandler);
    if (tinfo.socket < 0) return 0;
    tinfo.busy = 0;
    tinfo.length = 0;
    tinfo.received = 0;
    TtyPacketHeader* const header = reinterpret_cast<TtyPacketHeader*>(ttyUncached(wbuf));
    tinfo.sendCursor = reinterpret_cast<unsigned char*>(header);
    tinfo.receiveBuffer = ttyUncached(rbuf);
    header->protocol = 528;
    header->byte6 = 'E';
    header->byte7 = 'H';
    header->word8 = 0;
    header->zero = 0;
    tinfo.queue = QueueInit(256);
    return 1;
}
}
