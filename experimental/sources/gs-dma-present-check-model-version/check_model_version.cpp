/* Observed checkModelVersion at 0x002d18f0, 160 bytes. It opens
 * "rom0:ROMVER" with mode 1. A negative descriptor returns -1. Otherwise it
 * reads one byte at a time into a 256-byte buffer until a zero byte or 256
 * nonzero bytes, closes the descriptor, and calls atoi on the address
 * count-9 bytes into that buffer. The result is 1 when that value is greater
 * than 0x01315670, and 0 otherwise. Fresh compilation is not an original-byte
 * match. What the ROM text contains is unknown.
 */
extern "C" int sceOpen(const char* path, int mode);
extern "C" int sceRead(int descriptor, void* buffer, int count);
extern "C" int sceClose(int descriptor);
extern "C" int atoi(const char* text);

extern "C" int checkModelVersion() {
    const int descriptor = sceOpen("rom0:ROMVER", 1);
    if (descriptor < 0) return -1;
    unsigned char buffer[256];
    int count = 0;
    unsigned char* cursor = buffer;
    while (count < 256) {
        sceRead(descriptor, cursor, 1);
        const unsigned char byte = *cursor;
        cursor += 1;
        if (byte == 0) break;
        count += 1;
    }
    sceClose(descriptor);
    const int parsed = atoi(reinterpret_cast<char*>(buffer) + (count - 9));
    return parsed > 0x01315670 ? 1 : 0;
}
