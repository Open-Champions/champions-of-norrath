// Observed: End__Ct6VIPool1ZP13VIHSpriteAnim at 0x01224e30, 12 bytes. It ignores the pool and returns the
// constant 0x7fffffff, the value the Begin routine hands back when the pool has no
// first index. Inferred: it is the iteration end marker of VIPool<VIHSpriteAnim*>.
extern "C" {
int End__Ct6VIPool1ZP13VIHSpriteAnim(const void* pool) {
    (void)pool;
    return 0x7fffffff;
}
}
