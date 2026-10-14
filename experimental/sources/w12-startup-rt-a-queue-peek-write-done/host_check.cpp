// Host check for QueuePeekWriteDone. Not a build-plan input.
//   c++ -std=c++17 -DNORRATH_HOST_CONTRACT -o check host_check.cpp && ./check
// Pointer fields are compared as pointers, so the check also runs on a 64-bit host.
#include "queue_peek_write_done.cpp"
int main() {
    struct { TtyQueue header; unsigned char ring[8]; } q;
    unsigned char* ring = q.ring;
    q.header.size = 8;
    q.header.count = 3;
    q.header.readPtr = ring + 2;
    q.header.writePtr = ring + 5;
    QueuePeekWriteDone(&q.header);
    if (q.header.count != 4 || q.header.writePtr != ring + 6) return 1;
    QueuePeekWriteDone(&q.header);
    QueuePeekWriteDone(&q.header);          // reaches ring + 8: wraps
    if (q.header.count != 6 || q.header.writePtr != ring) return 2;
    if (q.header.size != 8 || q.header.readPtr != ring + 2) return 3;
    QueuePeekWriteDone(&q.header);          // no full-queue guard
    if (q.header.count != 7 || q.header.writePtr != ring + 1) return 4;
    q.header.writePtr = ring + 9;           // past the end is not wrapped (equality only)
    QueuePeekWriteDone(&q.header);
    if (q.header.writePtr != ring + 10) return 5;
    return 0;
}
