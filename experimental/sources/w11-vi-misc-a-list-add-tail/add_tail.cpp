// Observed: AddTail__7VILListP7VILNode at 0x01072810 is 36 bytes. A null node does
// nothing. Otherwise, with last = word 2 of the list: node word 0 = list + 4,
// node word 1 = last, word 0 of last = node, list word 2 = node. It returns nothing.
// Inferred: node word 0 is the next link and word 1 the previous link; list + 4 is
// the end sentinel that Init__7VILList establishes.
extern "C" {
void AddTail__7VILListP7VILNode(void* list, void* node) {
    if (node == nullptr) return;
    unsigned* header = static_cast<unsigned*>(list);
    unsigned* item = static_cast<unsigned*>(node);
    unsigned* last = reinterpret_cast<unsigned*>(header[2]);
    item[0] = reinterpret_cast<unsigned>(header + 1);
    item[1] = reinterpret_cast<unsigned>(last);
    last[0] = reinterpret_cast<unsigned>(item);
    header[2] = reinterpret_cast<unsigned>(item);
}
}
