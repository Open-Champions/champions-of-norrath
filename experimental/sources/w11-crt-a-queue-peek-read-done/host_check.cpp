// Host contract for QueuePeekReadDone. Not an EE plan input.
#include "queue_peek_read_done.cpp"
int main() {
    struct { TtyQueue header; unsigned char ring[8]; } q;
    unsigned char* ring = q.ring;
    q.header.size = 8;
    q.header.count = 3;
    q.header.readPtr = ring + 5;
    q.header.writePtr = ring + 8;
    QueuePeekReadDone(&q.header);
    if (q.header.count != 2 || q.header.readPtr != ring + 6) return 1;
    QueuePeekReadDone(&q.header);
    QueuePeekReadDone(&q.header);          // reaches ring + 8: wraps
    if (q.header.count != 0 || q.header.readPtr != ring) return 2;
    if (q.header.size != 8 || q.header.writePtr != ring + 8) return 3;
    QueuePeekReadDone(&q.header);          // below zero: no guard
    if (q.header.count != -1 || q.header.readPtr != ring + 1) return 4;
    q.header.readPtr = ring + 9;           // past the end is not wrapped (equality only)
    QueuePeekReadDone(&q.header);
    if (q.header.readPtr != ring + 10) return 5;
    return 0;
}
