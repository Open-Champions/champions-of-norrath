/* Observed sceTtyRead at 0x002d65e8, 208 bytes. Arguments: a destination
 * buffer and a maximum count. For each byte it waits until the count word of
 * the queue pointed to by tinfo+24 is nonzero, copies one byte from that
 * queue's read pointer (offset 8), and calls the local QueuePeekReadDone
 * (0x002d6298) on the queue. The loop ends after a line feed (10) or carriage
 * return (13) has been copied, in which case the count including that byte is
 * returned, or when the count is reached. A count of zero or less returns 0.
 * The waiting is a busy loop with no sleep. The queue is filled by the
 * original's deci2 handler, which is not part of this function.
 */
#include "../w9-startup-crt-f-tty-state/tty_state.hpp"

extern "C" {
void QueuePeekReadDone(TtyQueue* queue);

int sceTtyRead(char* buffer, int count) {
    int done = 0;
    while (done < count) {
        TtyQueue* const queue = tinfo.queue;
        while (queue->count == 0) {
        }
        char* slot = buffer + done;
        *slot = *tinfo.queue->readPtr;
        QueuePeekReadDone(tinfo.queue);
        ++done;
        if (*slot == '\n' || *slot == '\r') return done;
    }
    return done;
}
}
