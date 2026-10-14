// Observed: Begin__Ct6VIPool1Z19VIRasterMaterialLRU at 0x01211da0, 44 bytes (a weak const member). With the pool as this it reads the
// signed word at this+16. If that word is -1 the result is End__Ct6VIPool1Z19VIRasterMaterialLRU(this) (a tail call), otherwise the
// result is the word itself.
// Inferred: the word is the index of the first live entry, with -1 meaning that the pool is empty; the same shape as the other VIPool
// Begin routines. Unknown: the member name and how the word is maintained.
extern "C" {
int End__Ct6VIPool1Z19VIRasterMaterialLRU(const void* pool);

int Begin__Ct6VIPool1Z19VIRasterMaterialLRU(const void* pool) {
    int first = static_cast<const int*>(pool)[4];
    if (first == -1) return End__Ct6VIPool1Z19VIRasterMaterialLRU(pool);
    return first;
}
}
