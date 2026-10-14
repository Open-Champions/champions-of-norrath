// Observed sceSifLoadModule at 0x002dd000, 32 bytes. It calls
// _sceSifLoadModule with the same path, argument length, and argument, a
// stack word for the result, and a fifth argument of 0. The stack word is
// not read after the call. The returned value is the callee's value.
extern "C" int _sceSifLoadModule(const char* path, int argument_length, const void* argument,
                                 int* result_word, int operation);

extern "C" int sceSifLoadModule(const char* path, int argument_length, const void* argument) {
    int result_word;
    return _sceSifLoadModule(path, argument_length, argument, &result_word, 0);
}
