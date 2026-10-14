// Observed: End__Ct6VIPool1Z13VIHSpritePlay at 0x01203838 is 12 bytes: it returns the constant 0x7fffffff and reads nothing.
// Inferred: this is the iterator end marker that Begin and Next return when no entry remains.
extern "C" {
int End__Ct6VIPool1Z13VIHSpritePlay(const void*) {
    return 0x7fffffff;
}
}
