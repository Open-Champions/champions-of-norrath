// Observed strncpy at 0x00305118, 444 bytes. It copies at most count bytes
// from the source and stops after the source's zero; the rest of the count is
// filled with zero bytes. With no zero inside the count the result is not
// terminated. It returns the destination pointer. Aligned inputs copy 8- or
// 16-byte blocks without a zero first; the stored bytes match this byte loop.
extern "C" char* strncpy(char* destination, const char* source, unsigned count) {
    char* cursor = destination;
    while (count != 0u) {
        const char byte = *source++;
        *cursor++ = byte;
        --count;
        if (byte == '\0') break;
    }
    while (count != 0u) {
        *cursor++ = '\0';
        --count;
    }
    return destination;
}
