// Observed amxNewWorldParameter at 0x0013c3f8, 124 bytes. It walks the
// pointer list at currentRunningObject+44. The first entry whose
// first six characters are "level=" is kicked with zero and passed,
// without that prefix, to doNewWorld. Other entries are skipped.
// An exhausted list returns zero without a kick.
extern "C" unsigned char* currentRunningObject;
extern "C" int strncmp(const char* left, const char* right, unsigned count);
extern "C" void gameNetKick__Fi(int code);
extern "C" void doNewWorld__FPc(char* name);

static void* load_ptr(const unsigned char* bytes) {
    void* value;
    __builtin_memcpy(&value, bytes, sizeof value);
    return value;
}

extern "C" int amxNewWorldParameter__FP5__amxPi(void*, int*) {
    char** list = static_cast<char**>(load_ptr(currentRunningObject + 44));
    char* text = *list;
    while (text != nullptr) {
        if (strncmp(text, "level=", 6) == 0) {
            gameNetKick__Fi(0);
            doNewWorld__FPc(text + 6);
            return 0;
        }
        list += 1;
        text = *list;
    }
    return 0;
}
