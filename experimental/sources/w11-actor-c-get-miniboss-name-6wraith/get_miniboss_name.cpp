// Observed getMinibossName__6Wraith at 0x002052e8, 60 bytes. Formats the signed byte at self+76 with the
// string "MBWraith%d" into a function-local static 16-byte buffer (symbol buffer.1059 in the image, at
// 0x007d7160) with sprintf and returns that buffer. The format string is read from the image
// (0x0046f8c0). Inferred: the byte is a variant number and the result is a name for the miniboss
// resource set; not established here. Nothing here ran on the EE.
extern "C" int sprintf(char* destination, const char* format, ...);

extern "C" const char* getMinibossName__6Wraith(const void* self) {
    static char buffer[16];
    const signed char variant = *(static_cast<const signed char*>(self) + 76);
    sprintf(buffer, "MBWraith%d", static_cast<int>(variant));
    return buffer;
}
