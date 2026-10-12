// Observed lumpFindByType__FPvPci at 0x00166650, 208 bytes. The first argument
// addresses a lump directory: a 16-bit entry count at offset 0 and 12-byte
// entries from offset 4, each with a name pointer in its first word. The second
// argument is an extension string and the third is a number of matches to skip.
// Entries are visited in order. For each, strrchr finds the last '.' of the name;
// when there is one, strcmp compares the extension argument with the text after
// it. A match with the skip count at zero stops the scan, any other match
// decrements the count (so a negative count never stops on a match), and names
// without a '.' or with a different extension are passed over. The result is
// the address of the stopping entry, or null when every entry was visited.
// Inferred: the directory is the data of a loaded lump and the name pointers are
// valid strings, as the callers' use implies.
// Unknown: the other two words of an entry.
#ifdef NORRATH_W8_HOST
extern const char* norrath_w8_word_to_string(unsigned word);
#define NORRATH_W8_STRING(word) norrath_w8_word_to_string(word)
#else
#define NORRATH_W8_STRING(word) reinterpret_cast<const char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
char* strrchr(const char* text, int character);
int strcmp(const char* left, const char* right);

void* lumpFindByType__FPvPci(void* directory, char* extension, int skip) {
    unsigned* const entries = reinterpret_cast<unsigned*>(static_cast<unsigned char*>(directory) + 4);
    const int count = *static_cast<unsigned short*>(directory);
    int index = 0;
    const unsigned* cursor = entries;
    while (index < count) {
        const unsigned name_word = cursor[0];
        cursor += 3;
        const char* const dot = strrchr(NORRATH_W8_STRING(name_word), '.');
        if (dot != nullptr && strcmp(extension, dot + 1) == 0) {
            if (skip-- == 0) break;
        }
        ++index;
    }
    if (index == count) return nullptr;
    return entries + index * 3;
}
}
