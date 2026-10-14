// Observed strdup at 0x00304758, 32 bytes. Loads the word at _impure_ptr (0x00448480)
// and hands it, with the caller's string, to _strdup_r (0x00304778); the callee's
// result is the result.
extern "C" {
extern void* norrath_factory_impure_pointer_word;
char* _strdup_r(void* reent, const char* text);

char* strdup(const char* text) {
    return _strdup_r(norrath_factory_impure_pointer_word, text);
}
}
