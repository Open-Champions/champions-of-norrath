// Observed getMinibossName__11MaleDarkElf at 0x00289870, 60 bytes.
// Formats the signed byte at object+76 with sprintf and the format text "MBDarkElf%d" (at 0x4776f8)
// into a function-local static character buffer (the image's buffer.1090, 0x8d62c0) and returns the
// buffer's address.
// Inferred: the byte is the miniboss variant number chosen by initMiniboss__11MaleDarkElf and the
// result names the miniboss; the buffer size is not read from the image's symbol table here.
// Nothing here ran on the EE.
extern "C" int sprintf(char* buffer, const char* format, ...);

extern "C" char* getMinibossName__11MaleDarkElf(void* self) {
    static char buffer[64];
    sprintf(buffer, "MBDarkElf%d", static_cast<int>(*reinterpret_cast<signed char*>(static_cast<unsigned char*>(self) + 76)));
    return buffer;
}
