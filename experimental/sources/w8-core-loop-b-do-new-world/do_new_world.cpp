// Observed doNewWorld__FPc at 0x0013a478, 256 bytes.
// Copies the name into a stack buffer and into levelName (0x0034b660). When currentRunningObject is nonzero, the
// word at +44 of that object points at a null-terminated array of C string pointers; each is compared with the
// first 12 bytes of "orientation=", and for every match the remaining text is converted with atoi and its low
// byte stored at levelName+80 (the last match wins, and the array walk continues to the end).
// When no entry matched, the stack buffer gets " - " appended if it is not empty, then "NO ORIENTATION", and the
// byte at levelName+80 is set to -1. The stack buffer is not read afterwards; it is kept because the original
// calls are present. Nothing is returned.
// Inferred: levelName+80 holds an orientation value, from the string it is parsed from.
// Unknown: the stack buffer's purpose, its exact size (at most 112 bytes fit in the frame), and what the object
// at currentRunningObject is.
extern "C" char* strcpy(char* to, const char* from);
extern "C" unsigned strlen(const char* text);
extern "C" char* strcat(char* to, const char* from);
extern "C" int strncmp(const char* a, const char* b, unsigned count);
extern "C" int atoi(const char* text);
extern "C" char levelName[];
extern "C" unsigned char* currentRunningObject;

extern "C" void doNewWorld__FPc(char* name) {
    char text[112];
    bool found = false;
    strcpy(text, name);
    strcpy(levelName, name);
    if (currentRunningObject) {
        char** arguments = *(char***)(currentRunningObject + 44);
        for (char* argument = *arguments; argument; argument = *++arguments) {
            if (strncmp(argument, "orientation=", 12) == 0) {
                found = true;
                levelName[80] = (char)atoi(argument + 12);
            }
        }
    }
    if (!found) {
        if (strlen(text)) strcat(text, " - ");
        strcat(text, "NO ORIENTATION");
        levelName[80] = -1;
    }
}
