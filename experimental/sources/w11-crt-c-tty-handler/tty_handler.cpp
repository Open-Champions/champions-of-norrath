/* Observed sceTtyHandler at 0x002d62d8, 404 bytes (a file-local routine; the
 * symbol table name is used). sceTtyInit passes it to sceDeci2Open as the
 * receive handler together with the address of tinfo. Arguments: an event
 * number, a count and the tinfo pointer. Fields below use the layout of
 * tty_state.hpp. Events 1 and 2 are the same case, 3 and 4 have their own,
 * anything else does nothing.
 *   1, 2 with a nonzero count: if received + count (unsigned) exceeds 320 it
 *        prints "TTY: packet size larger than expect\n" (and carries on).
 *        Then it calls sceDeci2ExRecv(socket, receiveBuffer + received,
 *        count & 0xffff); a negative result prints "TTY: receive error"; the
 *        result is added to received, whatever its sign.
 *   1, 2 with a zero count: the receive buffer holds a packet whose 16-bit
 *        length is at offset 0. Bytes 12 up to that length are pushed one at
 *        a time: each is stored through the queue's write pointer and
 *        QueuePeekWriteDone(queue) is called. The queue and receive buffer
 *        pointers and the length are read again on every pass. Then received
 *        is cleared.
 *   3:   sceDeci2ExSend(socket, sendCursor, length & 0xffff). A negative
 *        result prints "TTY: send err %d\n" with it and clears busy; otherwise
 *        the send cursor advances and length shrinks by the result.
 *   4:   a nonzero length prints "TTY: err ti->wlen=%08x\n" with it; busy is
 *        cleared either way.
 * Inferred: events 3 and 4 are send-progress and send-complete, and 1 and 2
 * receive notifications; the Deci2 event numbering is not established here.
 */
#include "../w9-startup-crt-f-tty-state/tty_state.hpp"

extern "C" {
void kprintf(const char* format, ...);
int sceDeci2ExRecv(int socket, void* buffer, unsigned length);
int sceDeci2ExSend(int socket, void* buffer, unsigned length);
void QueuePeekWriteDone(TtyQueue* queue);

void sceTtyHandler(int event, int count, TtyInfo* info) {
    switch (event) {
    case 1:
    case 2:
        if (count == 0) {
            unsigned char* const packet = info->receiveBuffer;
            unsigned index = 12;
            if (index < *reinterpret_cast<unsigned short*>(packet)) {
                const unsigned char* source = packet;
                do {
                    const unsigned char byte = source[index];
                    ++index;
                    *info->queue->writePtr = byte;
                    QueuePeekWriteDone(info->queue);
                    source = info->receiveBuffer;
                } while (index < *reinterpret_cast<unsigned short*>(packet));
            }
            info->received = 0;
        } else {
            if (static_cast<unsigned>(info->received + count) > 320u) kprintf("TTY: packet size larger than expect\n");
            int received = sceDeci2ExRecv(info->socket, info->receiveBuffer + info->received, static_cast<unsigned short>(count));
            if (received < 0) kprintf("TTY: receive error");
            info->received = info->received + received;
        }
        break;
    case 3: {
        int sent = sceDeci2ExSend(info->socket, info->sendCursor, static_cast<unsigned short>(info->length));
        if (sent >= 0) {
            info->sendCursor = info->sendCursor + sent;
            info->length = info->length - sent;
        } else {
            kprintf("TTY: send err %d\n", sent);
            info->busy = 0;
        }
        break;
    }
    case 4:
        if (info->length != 0) kprintf("TTY: err ti->wlen=%08x\n", info->length);
        info->busy = 0;
        break;
    default:
        break;
    }
}
}
