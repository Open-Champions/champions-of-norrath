// Observed getMinibossName__10SeaMonster at 0x001f8bc8, 60 bytes. Formats the signed byte at
// object+76 into a 16-byte function-local buffer (observed at 0x007d6dd0) with the text
// "MBSeaMonster%d" and returns the buffer address. Nothing here ran on the EE.
extern "C" {
int sprintf(char* out, const char* format, ...);

const char* getMinibossName__10SeaMonster(void* self) {
    static char buffer[16];
    sprintf(buffer, "MBSeaMonster%d", static_cast<int>(*reinterpret_cast<signed char*>(static_cast<unsigned char*>(self) + 76)));
    return buffer;
}
}
