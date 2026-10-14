// Observed: End__Ct5VISet1ZP5VIWnd at 0x0120ac50 is 8 bytes. It ignores its argument and returns 0.
// Inferred: the iterator of this window set is an element index and 0 is the past-the-end value;
// the callers compare iterators against this result.
// Unknown: the meaning of the template parameters beyond the mangled name.
extern "C" {
int End__Ct5VISet1ZP5VIWnd(const void* set) {
    (void)set;
    return 0;
}
}
