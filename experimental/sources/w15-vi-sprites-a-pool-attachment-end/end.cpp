// Observed: End__Ct6VIPool1Z19VIHSpriteAttachment at 0x012037a8 is 12 bytes: it returns the constant 0x7fffffff and reads nothing.
// Inferred: the iterator end marker, as for the other VIPool instantiations.
extern "C" {
int End__Ct6VIPool1Z19VIHSpriteAttachment(const void*) {
    return 0x7fffffff;
}
}
