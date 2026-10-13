// Observed getMinibossName__6Wraith at 0x002052e8, 60 bytes.
// Formats the signed byte at object+76 with sprintf into a 16-byte static buffer (the image keeps
// it as the local .bss symbol buffer.1059 at 0x007d7160) using the 11-byte format at 0x0046f8c0,
// which is the text "MBWraith" followed by one %d. The buffer address is returned.
// Inferred: the byte at +76 is the miniboss variant number set by initMiniboss (1 to 20) and the
// text is a resource name. Unknown: where the name is looked up. Nothing here ran on the EE.
extern "C" int sprintf(char* destination, const char* format, ...);

extern "C" char* getMinibossName__6Wraith(void* self) {
    static char buffer[16];
    sprintf(buffer, "MBWraith%d", static_cast<int>(*(static_cast<signed char*>(self) + 76)));
    return buffer;
}
