/* Observed sceTtyWrite at 0x002d6470, 372 bytes. Arguments: a source buffer
 * and a byte count. A nonzero busy word (tinfo+12) returns -1 at once.
 * Otherwise it calls DIntr, sets busy, points the send cursor (tinfo+16) at
 * wbuf OR-ed with 0x20000000 and copies source bytes to offset 12 of that
 * area. A line feed is expanded to carriage return plus line feed. Copying
 * stops when the count runs out or 256 bytes have been stored. If the 256th
 * byte is the carriage return of an expanded line feed, that line feed and its
 * source byte are not copied or counted. The number of source bytes copied is
 * the result. The packet length (stored in tinfo+4 and in the first halfword
 * of the area) is the stored byte count plus 12. sceDeci2ReqSend(tinfo's
 * socket, the signed byte at offset 7 of the area) is then called. A negative
 * result clears busy, calls EIntr if DIntr returned nonzero and returns -1.
 * Otherwise it polls sceDeci2Poll(socket) until busy is cleared (by the
 * original's handler), calls EIntr if DIntr returned nonzero and returns the
 * copied count.
 */
#include "../w9-startup-crt-f-tty-state/tty_state.hpp"

extern "C" {
int DIntr();
int EIntr();
int sceDeci2ReqSend(int socket, int arg);
int sceDeci2Poll(int socket);

int sceTtyWrite(const char* source, int count) {
    int copied = 0;
    int stored = 0;
    if (tinfo.busy != 0) return -1;
    const int interrupts = DIntr();
    tinfo.busy = 1;
    TtyPacketHeader* const header = reinterpret_cast<TtyPacketHeader*>(ttyUncached(wbuf));
    tinfo.sendCursor = reinterpret_cast<unsigned char*>(header);
    char* destination = reinterpret_cast<char*>(header) + 12;
    while (--count != -1) {
        if (*source == '\n') {
            *destination++ = '\r';
            ++stored;
            if (stored >= 256) break;
        }
        *destination++ = *source++;
        ++stored;
        ++copied;
        if (stored >= 256) break;
    }
    tinfo.length = stored + 12;
    header->length = static_cast<unsigned short>(tinfo.length);
    if (sceDeci2ReqSend(tinfo.socket, static_cast<signed char>(header->byte7)) < 0) {
        tinfo.busy = 0;
        if (interrupts != 0) EIntr();
        return -1;
    }
    while (tinfo.busy != 0) sceDeci2Poll(tinfo.socket);
    if (interrupts != 0) EIntr();
    return copied;
}
}
