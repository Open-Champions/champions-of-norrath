// Observed: Remove__7VILNode at 0x01072680 is 56 bytes. The node holds two pointers: the word at
// offset 0 (A) and the word at offset 4 (B). If either is null nothing is written. Otherwise
// B->[0] = A, then A->[4] = B (A and B are re-read from the node between the two stores), then
// both node words are cleared (offset 4 first, then offset 0).
// Inferred: a doubly linked unlink where offset 0 is the successor and offset 4 the predecessor;
// the stores fix that reading but nothing here names the fields.
// Unknown: the list head layout and whether callers expect a return value (none is set).
extern "C" {
void Remove__7VILNode(void* self) {
    unsigned* node = static_cast<unsigned*>(self);
    if (node[0] != 0 && node[1] != 0) {
        *reinterpret_cast<unsigned*>(static_cast<__UINTPTR_TYPE__>(node[1])) = node[0];
        *reinterpret_cast<unsigned*>(static_cast<__UINTPTR_TYPE__>(node[0]) + 4) = node[1];
        node[1] = 0;
        node[0] = 0;
    }
}
}
