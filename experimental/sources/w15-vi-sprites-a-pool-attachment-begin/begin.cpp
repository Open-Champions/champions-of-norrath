// Observed: Begin__Ct6VIPool1Z19VIHSpriteAttachment at 0x01203738, 44 bytes. It reads the word at pool+16; when that is not -1 it is the
// result, otherwise the result is what End__Ct6VIPool1Z19VIHSpriteAttachment returns for the same pool (a tail call).
// Inferred: pool+16 is the index of the first live entry, -1 meaning none.
extern "C" {
int End__Ct6VIPool1Z19VIHSpriteAttachment(const void* pool);

int Begin__Ct6VIPool1Z19VIHSpriteAttachment(const void* pool) {
    int first = *(static_cast<const int*>(pool) + 4);
    if (first != -1) return first;
    return End__Ct6VIPool1Z19VIHSpriteAttachment(pool);
}
}
