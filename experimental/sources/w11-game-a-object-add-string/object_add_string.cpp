// Observed objectAddString__FPcP10GameObject at 0x00140410, 368 bytes. Registers an object under a
// name in the open-addressed tables stringHash and objectHash (512 words each). Arguments: the name
// (modified in place when it starts with a double quote) and the object (stored as given).
//   1. When the first character of the name is '"' (34): the text after it is copied to a local
//      512-byte buffer with strcpy, the buffer is cut at its first '"' (if any) with strchr, and the
//      result is copied back over the name with strcpy. So a quoted name loses both quotes in place.
//   2. objectStrings is incremented, then the name is hashed over strlen characters. Each character is
//      taken as a signed char; when bit 1 of _ctype_[1 + c] is set (an upper-case letter in the
//      newlib table) 32 is added. Hash h (signed 32-bit, starts at 0), per character: h += c;
//      h += h << 10; h ^= h >> 6 (arithmetic shift). Finally h += h << 3; h ^= h >> 11 (arithmetic).
//      The slot is h & 0x1ff.
//   3. Probing: an empty stringHash slot ends the search. A slot with a name and a non-null
//      objectHash entry is skipped. A slot with a name and a null object entry ends the search when
//      strcasecmp(slot name, name) is zero, and is skipped otherwise. The next slot is (slot + 1) &
//      0x1ff. There is no bound: a full table would loop forever.
//   4. stringHash[slot] = name and objectHash[slot] = object.
// Nothing is returned. Inferred: the tables map names to objects for name lookups, and a slot that has a
// name but no object is a name reserved earlier. Unknown: where the table sizes are enforced.
// Nothing here ran on the EE.
extern "C" {
extern char* stringHash[512];
extern void* objectHash[512];
extern int objectStrings;
extern unsigned char _ctype_[];
char* strcpy(char* destination, const char* source);
char* strchr(const char* text, int character);
unsigned strlen(const char* text);
int strcasecmp(const char* first, const char* second);
}

extern "C" void objectAddString__FPcP10GameObject(char* name, void* object) {
    if (name[0] == '"') {
        char text[512];
        strcpy(text, name + 1);
        if (strchr(text, '"') != nullptr) *strchr(text, '"') = 0;
        strcpy(name, text);
    }
    objectStrings = objectStrings + 1;
    int length = static_cast<int>(strlen(name));
    int hash = 0;
    for (int i = 0; i < length; ++i) {
        int c = name[i];
        if (_ctype_[1 + c] & 1) c += 32;
        hash += c;
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    int slot = hash & 0x1ff;
    for (;;) {
        char* existing = stringHash[slot];
        if (existing == nullptr) break;
        if (objectHash[slot] == nullptr && strcasecmp(existing, name) == 0) break;
        slot = (slot + 1) & 0x1ff;
    }
    stringHash[slot] = name;
    objectHash[slot] = object;
}
