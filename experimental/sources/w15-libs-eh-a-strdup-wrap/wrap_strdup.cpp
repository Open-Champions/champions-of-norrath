// Observed __wrap_strdup at 0x00144af0, 112 bytes (the --wrap partner of strdup).
// A zero enableMallocWrapper byte (0x00483b11) tail-calls strdup (0x00304758)
// with the same pointer. Any other byte calls strlen (0x00304cfc), then
// memAlloc__FiPc (0x00144368) with (length + 1, tag), then strcpy (0x00304640)
// from the source into that block, and returns the block pointer. The result of
// memAlloc is not tested for null before strcpy.
// The tag the original passes is the address 0x00483b10, eight file-backed zero
// bytes in .data just after the word `mark` (0x00483b08); read here as an empty
// string. A string literal of the same (empty) content stands in for it; the
// object's later run-time contents are unknown.
extern "C" {
extern unsigned char enableMallocWrapper;
char* strdup(const char* text);
unsigned strlen(const char* text);
char* strcpy(char* destination, const char* source);
void* memAlloc__FiPc(int bytes, char* tag);

char* __wrap_strdup(const char* text) {
    if (enableMallocWrapper == 0) return strdup(text);
    const int bytes = static_cast<int>(strlen(text)) + 1;
    char* block = static_cast<char*>(memAlloc__FiPc(bytes, const_cast<char*>("")));
    strcpy(block, text);
    return block;
}
}
