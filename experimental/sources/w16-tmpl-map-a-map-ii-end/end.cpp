// Observed: End__Ct5VIMap2ZiZi at 0x01203428 is 8 bytes. It ignores its argument and returns 0.
// Inferred: iterators of this map are element indices and 0 is the past-the-end value, as for
// the window set's End. Unknown: meaning of the template parameters beyond the mangled name.
extern "C" {
int End__Ct5VIMap2ZiZi(const void* map) {
    (void)map;
    return 0;
}
}
