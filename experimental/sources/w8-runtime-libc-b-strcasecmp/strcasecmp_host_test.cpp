// Host check only; not an EE build input. Build the source with -Dstrcasecmp=w8_strcasecmp so it
// does not collide with the host C library, compile this file separately, and link both. The table view is rebuilt for the
// "C" locale: only A-Z carry bit 1.
#include <cstdio>
#include <cstring>
extern "C" unsigned char norrath_factory_ctype_minimum_view[384] = {
#define Z8 0,0,0,0,0,0,0,0,
#define Z16 Z8 Z8
#define Z64 Z16 Z16 Z16 Z16
    Z64 Z64                                   // indices 0..127: characters -128..-1
    Z64 Z64                                   // characters 0..127 start here, patched below
    Z64 Z64
};
extern "C" int w8_strcasecmp(const char*, const char*);
static int fold(unsigned c) { return (c >= 'A' && c <= 'Z') ? int(c) + 32 : int(c); }
static int reference(const unsigned char* a, const unsigned char* b) {
    for (;; ++a, ++b) {
        if (fold(*a) != fold(*b) || *a == 0) return fold(*a) - fold(*b);
    }
}
int main() {
    unsigned char* table = norrath_factory_ctype_minimum_view;
    for (int c = 'A'; c <= 'Z'; ++c) table[128 + c] = 1;
    const char* words[] = {"", "a", "A", "abc", "ABC", "AbC", "abd", "ab", "ABCD", "Orccave1", "orcCAVE1", "orccave2", "@", "`", "[", "{", "Z", "z", "zz"};
    unsigned checked = 0;
    for (const char* x : words) {
        for (const char* y : words) {
            const int want = reference((const unsigned char*)x, (const unsigned char*)y);
            if (w8_strcasecmp(x, y) != want) return 1;
            ++checked;
        }
    }
    // High bytes are not letters in the "C" table, whether read signed or unsigned.
    if (w8_strcasecmp("\xc1", "\xe1") != (0xc1 - 0xe1)) return 2;
    if (w8_strcasecmp("a\xff", "a\x01") != 0xfe) return 3;
    std::printf("strcasecmp ok (%u cases)\n", checked);
    return 0;
}
