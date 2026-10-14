// Host contract for QueueInit. Not an EE plan input.
#include "queue_init.cpp"
#include "../w11-crt-a-tty-queue-storage/tty_queue_storage.cpp"
#include <cstring>
int main() {
    unsigned char* raw = reinterpret_cast<unsigned char*>(&norrath_tty_queue);
    std::memset(raw, 0xa5, sizeof(norrath_tty_queue));
    TtyQueue* q = QueueInit(256);
    if (q != &norrath_tty_queue.header) return 1;
    if (q->size != 256 || q->count != 0) return 2;
    if (q->readPtr != raw + sizeof(TtyQueue) || q->writePtr != raw + sizeof(TtyQueue)) return 3;
    for (unsigned i = sizeof(TtyQueue); i < sizeof(norrath_tty_queue); ++i) if (raw[i] != 0xa5) return 4;
    q = QueueInit(-7);
    if (q->size != -7 || q->readPtr != raw + sizeof(TtyQueue)) return 5;
    return 0;
}
