#include "lump_find.hpp"
namespace {
using Word = unsigned;
Word address(const unsigned char* p) {
    return static_cast<Word>(reinterpret_cast<__UINTPTR_TYPE__>(p));
}
unsigned char* bytes(Word a) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a));
}
Word read_half(Word a) { return Word(bytes(a)[0]) | (Word(bytes(a)[1]) << 8); }
Word read_word(Word a) {
    return Word(bytes(a)[0]) | (Word(bytes(a)[1]) << 8) |
           (Word(bytes(a)[2]) << 16) | (Word(bytes(a)[3]) << 24);
}
int signed_word(Word a) {
    return a < 0x80000000u ? static_cast<int>(a) : -1 - static_cast<int>(~a);
}
int midpoint(int lower, int upper) {
    const Word sum = Word(lower) + Word(upper);
    return signed_word((sum >> 1) | (sum & 0x80000000u));
}
}
extern "C" unsigned char* norrath_lump_find(unsigned char* lump, char* key) {
    const Word base = address(lump) + 4u;
    const int initial_maximum = static_cast<int>(read_half(address(lump))) - 1;
    int lower = 0, upper = initial_maximum;
    for (;;) {
        const int middle = midpoint(lower, upper);
        int relation;
        if (middle < 0) {
            relation = 1;
        } else if (initial_maximum < middle) {
            relation = -1;
        } else {
            const Word record = base + 12u * Word(middle);
            const Word name = read_word(record);
            relation = norrath_string_compare(key, reinterpret_cast<char*>(bytes(name)));
        }
        if (relation == 0) return bytes(base + 12u * Word(middle));
        if (!(lower < upper)) return nullptr;
        if (relation <= 0) upper = signed_word(Word(middle) - 1u);
        else lower = signed_word(Word(middle) + 1u);
    }
}
