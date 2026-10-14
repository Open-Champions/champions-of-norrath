// Observed _strdup_r at 0x00304778, 108 bytes. Takes the string length with strlen
// (0x00304cfc), asks _malloc_r (0x00301840) for length + 1 bytes under the caller's
// reentrancy pointer, and when the block is not null copies length + 1 bytes into it
// with memcpy (0x00301ffc). Returns the block, which is null when the allocation
// failed. The result of memcpy is not used.
extern "C" {
unsigned strlen(const char* text);
void* _malloc_r(void* reent, unsigned bytes);
void* memcpy(void* destination, const void* source, unsigned count);

char* _strdup_r(void* reent, const char* text) {
    const unsigned bytes = strlen(text) + 1;
    char* block = static_cast<char*>(_malloc_r(reent, bytes));
    if (block != nullptr) memcpy(block, text, bytes);
    return block;
}
}
