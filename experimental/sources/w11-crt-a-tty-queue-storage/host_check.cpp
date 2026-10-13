// Host contract for the q.12 storage layout. Not an EE plan input.
// The EE has 32-bit pointers; the exact 16/0x110 sizes are only asserted when the host does too.
#include <cstddef>
#include "tty_queue_storage.cpp"
int main() {
    if (offsetof(TtyQueueObject, ring) != sizeof(TtyQueue)) return 1;
    if (sizeof(TtyQueueObject) != sizeof(TtyQueue) + 256) return 2;
    if (sizeof(void*) == 4 && (sizeof(TtyQueue) != 16 || sizeof(TtyQueueObject) != 0x110)) return 3;
    unsigned char* raw = reinterpret_cast<unsigned char*>(&norrath_tty_queue);
    for (unsigned i = 0; i < sizeof(TtyQueueObject); ++i) if (raw[i] != 0) return 4;
    return 0;
}
