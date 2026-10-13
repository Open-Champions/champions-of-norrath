// Observed: End__Ct6VIPool1ZP8VISprite at 0x01224b30, 12 bytes. It ignores the pool and returns the
// constant 0x7fffffff, the value the Begin routine hands back when the pool has no
// first index. Inferred: it is the iteration end marker of VIPool<VISprite*>.
extern "C" {
int End__Ct6VIPool1ZP8VISprite(const void* pool) {
    (void)pool;
    return 0x7fffffff;
}
}
