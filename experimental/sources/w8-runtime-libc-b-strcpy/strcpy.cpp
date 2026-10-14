// Observed strcpy at 0x00304640, 276 bytes. It copies the source through its
// terminating zero and returns the destination pointer. Aligned inputs copy
// 8- or 16-byte blocks that contain no zero first; the stored bytes are the
// same as this byte loop. Overlapping buffers are not handled.
extern "C" char* strcpy(char* destination, const char* source) {
    char* cursor = destination;
    char byte;
    do {
        byte = *source++;
        *cursor++ = byte;
    } while (byte != '\0');
    return destination;
}
