// Observed strcasecmp at 0x00304168, 216 bytes. Each byte is classified
// through the table view that starts 127 bytes before _ctype_ (indexed by
// character + 128); bit 1 marks an upper-case letter, which is folded by
// adding 32. The loop reads bytes as signed, so a byte above 0x7f indexes the
// bytes before _ctype_. The final mismatch (or the terminating zero) is read
// again as unsigned, classified again, and the result is the difference of
// those two folded values. The same original is also authored as
// norrath_factory_strcasecmp.
extern "C" const unsigned char norrath_factory_ctype_minimum_view[];

namespace {
int signed_byte(unsigned char byte) { return byte < 128u ? int(byte) : int(byte) - 256; }
int fold_signed(int character) {
    return (norrath_factory_ctype_minimum_view[unsigned(character + 128)] & 1u)
        ? character + 32 : character;
}
int fold_unsigned(unsigned character) {
    return (norrath_factory_ctype_minimum_view[character + 128u] & 1u)
        ? int(character) + 32 : int(character);
}
}

extern "C" int strcasecmp(const char* left, const char* right) {
    const auto* a = reinterpret_cast<const unsigned char*>(left);
    const auto* b = reinterpret_cast<const unsigned char*>(right);
    int a_signed = signed_byte(*a);
    while (a_signed != 0) {
        if (fold_signed(a_signed) != fold_signed(signed_byte(*b))) break;
        ++a;
        ++b;
        a_signed = signed_byte(*a);
    }
    return fold_unsigned(*a) - fold_unsigned(*b);
}
