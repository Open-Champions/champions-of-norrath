// Observed: AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3 at 0x01027440, 272 bytes. With (this, a, b, c, d), where a and b are
// 12-byte vectors and c and d 16-byte colours passed by reference, it calls RemHead__7VILList(this+40). While that returns null it calls
// ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT(this, the word at this+36) and tries RemHead again. For the node N it gets, it calls
// AddHead__7VILListP7VILNode(this+28, N), then copies a to N+16, b to N+28, c to N+40 and d to N+56, stores zero in the word at N+72, and
// returns 0. The copies are made after AddHead.
// Inferred: this+40 is a free-node list and this+28 the live-segment list; a full trail reclaims the oldest segments through
// ClearSegmentsForward until a node is free, and a segment is two positions (a, b) with two colours (c, d) and a zero-initialised word. The
// word at this+36 is probably the oldest live segment, a guess from the argument it is passed.
// Unknown: the roles of a and b and c and d, the meaning of the word at N+72, and
// the layout of the nodes before +16.
extern "C" {
void* RemHead__7VILList(void* list);
void AddHead__7VILListP7VILNode(void* list, void* node);
void ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT(void* self, unsigned segment);

int AddSegment__9VITrailFxRC7VIVect3T1RC10VIColor32FT3(unsigned char* self, const unsigned* a, const unsigned* b, const unsigned* c,
                                                       const unsigned* d) {
    unsigned char* node;
    while ((node = static_cast<unsigned char*>(RemHead__7VILList(self + 40))) == nullptr)
        ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT(self, *reinterpret_cast<unsigned*>(self + 36));
    AddHead__7VILListP7VILNode(self + 28, node);
    unsigned* out = reinterpret_cast<unsigned*>(node + 16);
    out[0] = a[0]; out[1] = a[1]; out[2] = a[2];
    out = reinterpret_cast<unsigned*>(node + 28);
    out[0] = b[0]; out[1] = b[1]; out[2] = b[2];
    out = reinterpret_cast<unsigned*>(node + 40);
    out[0] = c[0]; out[1] = c[1]; out[2] = c[2]; out[3] = c[3];
    out = reinterpret_cast<unsigned*>(node + 56);
    out[0] = d[0]; out[1] = d[1]; out[2] = d[2]; out[3] = d[3];
    *reinterpret_cast<unsigned*>(node + 72) = 0;
    return 0;
}
}
