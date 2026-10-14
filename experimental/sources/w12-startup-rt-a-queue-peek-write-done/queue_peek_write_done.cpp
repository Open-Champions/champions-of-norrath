// Observed QueuePeekWriteDone at 0x002d6258, 60 bytes, pinned SLUS_205.65 (a local symbol).
// Takes the queue header, the counterpart of QueuePeekReadDone. Adds one to the count (word 1) and
// one to the write cursor (word 3); when the cursor reaches queue + 16 + size (word 0) it wraps to
// queue + 16. Nothing checks for a full queue. Returns nothing.
// Inferred: sceTtyWrite-side callers call it after storing a byte at the write cursor.
#include "../w9-startup-crt-f-tty-state/tty_state.hpp"

extern "C" void QueuePeekWriteDone(TtyQueue* queue) {
    queue->count = queue->count + 1;
    unsigned char* const next = queue->writePtr + 1;
    queue->writePtr = next;
    unsigned char* const ringStart = reinterpret_cast<unsigned char*>(queue + 1);   // 16 bytes on the EE
    if (next == ringStart + queue->size) queue->writePtr = ringStart;
}
