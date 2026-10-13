// Observed: End__Ct10VIMultiMap2ZfZ14VIEmitterEntry at 0x011f5058, 8 bytes (a weak
// template instantiation). It ignores its argument and returns zero.
// Inferred: zero is the past-the-end cursor of the multimap. Unknown: whether
// the template names it as a constant.
extern "C" {
int End__Ct10VIMultiMap2ZfZ14VIEmitterEntry(const void* map) {
    return 0;
}
}
