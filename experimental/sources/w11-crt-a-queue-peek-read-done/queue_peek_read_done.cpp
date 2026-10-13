// Observed QueuePeekReadDone at 0x002d6298, 60 bytes, pinned SLUS_205.65 (a local symbol).
// Takes the queue header. Drops one byte from the front: the count (word 1) goes
// down by one and the read cursor (word 2) moves forward one byte; when it
// reaches queue + 16 + size (word 0) it wraps to queue + 16. Nothing checks for an
// empty queue, so a caller that has not seen a nonzero count underflows it.
// sceTtyRead calls it after copying the byte at the read cursor.
#include "../w9-startup-crt-f-tty-state/tty_state.hpp"

extern "C" void QueuePeekReadDone(TtyQueue* queue) {
    queue->count = queue->count - 1;
    unsigned char* const next = queue->readPtr + 1;
    queue->readPtr = next;
    unsigned char* const ringStart = reinterpret_cast<unsigned char*>(queue + 1);   // 16 bytes on the EE
    if (next == ringStart + queue->size) queue->readPtr = ringStart;
}
