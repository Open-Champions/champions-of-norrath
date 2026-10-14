// Observed: Log2__13VIEESurfCachei at 0x01133c68 is 44 bytes. With (cache, n) it returns the
// number of times n can be shifted right by one (arithmetic) before it is no longer positive:
// 0 for n <= 0, otherwise the bit length of n (1 -> 1, 2 -> 2, 4 -> 3). The cache is unused.
// Inferred: a ceiling-free integer log used to turn a power-of-two size into an exponent;
// for an exact power of two 2^k it returns k + 1, so callers presumably subtract one.
// Unknown: which callers rely on that offset.
extern "C" {
int Log2__13VIEESurfCachei(const void* cache, int n) {
    (void)cache;
    int bits = 0;
    while (n > 0) {
        n >>= 1;
        ++bits;
    }
    return bits;
}
}
