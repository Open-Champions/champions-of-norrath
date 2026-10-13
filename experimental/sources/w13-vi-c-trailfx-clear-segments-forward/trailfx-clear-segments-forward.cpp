// Observed: ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT at 0x01027618, 100 bytes,
// returns nothing. With the trail effect as this and a segment node as the argument:
// the list at this + 40 is the destination. Starting at the argument, it repeats:
// read the next-link word of the ORIGINAL argument node, call Remove__7VILNode on the
// current node, call AddTail__7VILListP7VILNode(this + 40, current node), and make the
// word read at the start of the round the new current node. It stops after a round
// once the first word of the current node is zero.
// Observed oddity: the link is read from the original argument on every round, not
// from the current node. This is reproduced literally because the image does so.
// Inferred: the nodes form a doubly linked list in the layout used by VILNode (next at
// +0), with a tail marker whose next word is zero. Unknown: member names and whether
// the original source meant to read the link from the moving node.
extern "C" {
void Remove__7VILNode(void* node);
void AddTail__7VILListP7VILNode(void* list, void* node);

void ClearSegmentsForward__9VITrailFxPQ29VITrailFx7SEGMENT(void* object, void* segment) {
    void* list = static_cast<unsigned char*>(object) + 40;
    void* const* origin = static_cast<void* const*>(segment);
    void* current = segment;
    do {
        void* next = *origin;
        Remove__7VILNode(current);
        AddTail__7VILListP7VILNode(list, current);
        current = next;
    } while (*static_cast<void* const*>(current) != 0);
}
}
