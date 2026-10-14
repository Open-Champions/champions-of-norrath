// Observed strupr at 0x003068c8, 72 bytes. Walks the string up to its zero byte.
// Each byte is sign-extended and looked up in the character-class table (window
// view, index byte + 128); when the class word has bit 1 set (lower case) the
// byte is replaced by itself minus 32. The argument pointer is returned.
extern "C" {
extern unsigned char norrath_factory_ctype_minimum_view[];

char* strupr(char* text) {
    char* cursor = text;
    while (*cursor != 0) {
        const int value = *cursor;
        if (norrath_factory_ctype_minimum_view[value + 128] & 2u) *cursor = static_cast<char>(value - 32);
        ++cursor;
    }
    return text;
}
}
