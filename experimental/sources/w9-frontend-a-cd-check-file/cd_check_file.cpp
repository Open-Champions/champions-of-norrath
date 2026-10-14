// Observed: cdCheckForFileInCurrentLayer__FPc at 0x00166720 is 188 bytes. It formats the name
// with the string at 0x00484000 ("%s;1") into an 80-byte-or-smaller stack buffer, then converts
// the buffer in place to upper case: a byte whose _ctype_ entry (indexed by the signed byte plus
// one) has bit 2 set loses 32. It then walks the table that currentLayerDir points to: the entry
// count is the word at offset 0x6000 of that table (reloaded every pass) and each entry is 32
// bytes of name. It returns 1 when an entry compares equal to the buffer with strcmp, 0 when
// the walk reaches the count.
// Inferred: nothing.
// Unknown: the stack buffer's true size, and what the bytes after the name in each 32-byte
// entry hold.

extern "C" {
extern unsigned char* currentLayerDir;
extern const unsigned char _ctype_[];
int sprintf(char* buffer, const char* format, ...);
int strcmp(const char* left, const char* right);
}

extern "C" int cdCheckForFileInCurrentLayer__FPc(char* name) {
    char buffer[80];
    sprintf(buffer, "%s;1", name);
    for (char* p = buffer; *p != 0; ++p) {
        const signed char c = *p;
        if ((_ctype_[c + 1] & 2) != 0) {
            *p = static_cast<char>(c - 32);
        }
    }
    int index = 0;
    for (;;) {
        const int count = *reinterpret_cast<int*>(currentLayerDir + 0x6000);
        if (!(index < count)) {
            break;
        }
        if (strcmp(buffer, reinterpret_cast<char*>(currentLayerDir) + index * 32) == 0) {
            break;
        }
        index += 1;
    }
    return index != *reinterpret_cast<int*>(currentLayerDir + 0x6000);
}
