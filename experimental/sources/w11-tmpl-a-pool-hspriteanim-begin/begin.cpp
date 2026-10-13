// Observed: Begin__Ct6VIPool1ZP13VIHSpriteAnim at 0x01224e00, 44 bytes. It reads the word at
// pool+16; when that word is not -1 it is the result, otherwise the result is
// whatever the matching End routine returns for the same pool (tail call).
// Inferred: pool+16 is the index of the first live entry, -1 meaning none.
extern "C" {
int End__Ct6VIPool1ZP13VIHSpriteAnim(const void* pool);

int Begin__Ct6VIPool1ZP13VIHSpriteAnim(const void* pool) {
    int first = *reinterpret_cast<const int*>(static_cast<const char*>(pool) + 16);
    if (first != -1) return first;
    return End__Ct6VIPool1ZP13VIHSpriteAnim(pool);
}
}
