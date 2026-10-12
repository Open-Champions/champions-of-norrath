// Observed strcat at 0x00304240, 300 bytes. It finds the terminating zero of
// the destination (scanning aligned blocks first), calls strcpy on that
// position with the source, and returns the original destination pointer. The
// block scan only changes how many bytes are read at once, not the result.
extern "C" char* strcpy(char* destination, const char* source);

extern "C" char* strcat(char* destination, const char* source) {
    char* end = destination;
    while (*end != '\0') ++end;
    strcpy(end, source);
    return destination;
}
