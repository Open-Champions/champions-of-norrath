// Observed: __7VILNode at 0x01072670, 12 bytes. With one argument (the node) it stores zero at +4 and then zero at +0 and returns
// without touching anything else.
// Inferred: a doubly linked list node whose first two words are links (next at +0, previous at +4 as AddTail uses them);
// the class name comes only from the mangled symbol.
// Unknown: the node's total size and any members beyond +8.
extern "C" {
void __7VILNode(void* self) {
    unsigned* words = static_cast<unsigned*>(self);
    words[1] = 0;
    words[0] = 0;
}
}
