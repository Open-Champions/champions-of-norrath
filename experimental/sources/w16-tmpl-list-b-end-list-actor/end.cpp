// Observed: End__Ct6VIList1Z7VIActor at 0x01225d08, 12 bytes. Returns 0x7fffffff; the argument is not read.
// Inferred: 0x7fffffff is the end marker that the iterators of this container family compare their cursor
// against, as in the sibling End instances. The template argument comes only from the mangled name.
// Unknown: nothing beyond the constant.
extern "C" {
int End__Ct6VIList1Z7VIActor(const void* container) {
    (void)container;
    return 0x7fffffff;
}
}
