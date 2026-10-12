// Observed getLongLevelName__FPc at 0x0025bf40, 168 bytes. The name is copied
// into a 128-byte stack buffer with strcpy. While that copy is at least two
// characters long, textGetString__FPc (0x001671f8) is called on it and the
// result kept. If the 16-bit unit at byte offset 4 of that result is 0x3a
// (':'), the last character of the copy is cut off and the loop repeats;
// otherwise the loop stops. After the loop a nonzero kept result is returned,
// and a zero one (no pass ran) returns textGetString__FPc of the original name.
// A null result from textGetString__FPc is not supported: the image would read address 4 and continue,
// here the unit-2 read assumes a valid entry. Unknown: the buffer is never bounds checked here, as in the image; the
// meaning of a ':' at unit 2 of a returned string is not established.
extern "C" char* strcpy(char*, const char*);
extern "C" __SIZE_TYPE__ strlen(const char*);
extern "C" unsigned short* textGetString__FPc(char*);

extern "C" unsigned short* getLongLevelName__FPc(char* name) {
    char buffer[128];
    unsigned short* found = 0;
    strcpy(buffer, name);
    while (strlen(buffer) >= 2) {
        found = textGetString__FPc(buffer);
        if (found[2] != 0x3a) break;
        buffer[strlen(buffer) - 1] = 0;
    }
    if (found != 0) return found;
    return textGetString__FPc(name);
}
