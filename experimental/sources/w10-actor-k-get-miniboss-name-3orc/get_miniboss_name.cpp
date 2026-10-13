// Observed getMinibossName__3Orc at 0x0022b998, 60 bytes. Formats the signed byte at object+76
// into a 16-byte function-local buffer (observed at 0x007d7ae0 as zero-initialised storage) with
// the text "MBOrc%d" and returns the buffer address. Nothing here ran on the EE.
extern "C" {
int sprintf(char* out, const char* format, ...);

const char* getMinibossName__3Orc(void* self) {
    static char buffer[16];
    sprintf(buffer, "MBOrc%d", static_cast<int>(*reinterpret_cast<signed char*>(static_cast<unsigned char*>(self) + 76)));
    return buffer;
}
}
