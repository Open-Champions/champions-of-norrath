// Observed: Size__Ct6VIPool1Z13VIHSpritePlay at 0x01203730 is 8 bytes: it returns the word at pool+0 and does nothing else.
// Inferred: the first word is the live entry count (the sprite code compares this result with zero before processing its play list).
// Unknown: whether the count can differ from the number of chained entries.
extern "C" {
int Size__Ct6VIPool1Z13VIHSpritePlay(const void* pool) {
    return *static_cast<const int*>(pool);
}
}
