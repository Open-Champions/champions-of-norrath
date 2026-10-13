// Observed QueueInit at 0x002d6230, 36 bytes, pinned SLUS_205.65 (a local symbol).
// Takes the ring size. Stores it in word 0 of the single local queue q.12
// (0x008de500), clears the count in word 1, points both the read cursor (word 2)
// and the write cursor (word 3) at the first byte behind the 16-byte header, and
// returns the address of the queue. No allocation and no size check; the ring
// bytes are not touched. sceTtyInit stores the result in tinfo.queue.
// Unknown: whether the original ever reuses the queue for another size.
#include "../w11-crt-a-tty-queue-storage/tty_queue_storage.hpp"

extern "C" TtyQueue* QueueInit(int size) {
    TtyQueue* const queue = &norrath_tty_queue.header;
    queue->size = size;
    queue->count = 0;
    queue->readPtr = norrath_tty_queue.ring;
    queue->writePtr = norrath_tty_queue.ring;
    return queue;
}
