// Observed: Next__Ct6VIList1ZP9VIGUIPagei at 0x01209610 is 20 bytes. It ignores its first argument
// and, given a node address in the second, loads the word at node+4 and returns it, except that a
// zero word is returned as 0x7fffffff.
// Inferred: node+4 is the link to the following node, zero ends the list and 0x7fffffff is the
// iterator's end marker. The mangled name types the argument as int; the code dereferences it, so it
// is declared here as the node pointer it holds (the same register contents on this target).
struct Node {
    int word0;
    int next;
};

extern "C" {
int Next__Ct6VIList1ZP9VIGUIPagei(const void* self, const Node* node) {
    int next = node->next;
    return next != 0 ? next : 0x7fffffff;
}
}
