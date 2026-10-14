// Observed: Begin__Ct6VIPool1Z17VIHSpritePlayNode at 0x01203a78 is 24 bytes up to the next symbol (the symbol table says 44) and calls nothing.
// It reads the word at pool+16 and returns it, except that when the word is -1 it returns
// 0x7fffffff instead.
// Inferred: pool+16 is the index of the first used element and -1 means none; the past-the-end
// value of this pool is the constant 0x7fffffff, inlined here rather than via an End call
// (the other pool Begin routines call End instead).
// Unknown: the template parameters beyond the mangled name.
extern "C" {
int Begin__Ct6VIPool1Z17VIHSpritePlayNode(const void* pool) {
    const int first = *reinterpret_cast<const int*>(static_cast<const unsigned char*>(pool) + 16);
    return first == -1 ? 0x7fffffff : first;
}
}
