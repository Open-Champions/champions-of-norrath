// Observed: End__Ct6VIPool1ZP8VIRefMap at 0x01224ce0, 12 bytes. It ignores the pool and returns the
// constant 0x7fffffff, the value the Begin routine hands back when the pool has no
// first index. Inferred: it is the iteration end marker of VIPool<VIRefMap*>.
extern "C" {
int End__Ct6VIPool1ZP8VIRefMap(const void* pool) {
    (void)pool;
    return 0x7fffffff;
}
}
