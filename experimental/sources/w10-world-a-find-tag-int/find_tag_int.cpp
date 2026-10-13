// Observed GameObject::findTagInt (findTagInt__10GameObjectPci) at 0x00140f60, 316 bytes in
// SLUS_205.65. Arguments: the object, a writable text key, an occurrence index.
// The object holds, at +44, a pointer to a null-terminated array of C strings (tag lines).
//  1. A null +44 returns 0 without touching the key.
//  2. The key is lower-cased in place through the _ctype_ table (class bit 0x01, "upper",
//     adds 32), so the caller's buffer is modified.
//  3. Each entry is compared with strncmp over strlen(key) bytes; the entry matches when the
//     compare is zero and the next character of the entry is '='. Non-matches, including
//     a prefix match not followed by '=', are skipped. The +44 pointer is read again on
//     every step.
//  4. The occurrence index counts matches: the index is decremented at each match, and the
//     match at which the result equals -1 (so index 0 picks the first match, 1 the second)
//     returns atoi of the text after the '='. A negative index therefore never matches.
//  5. Falling off the end of the array returns 0.
// Inferred: the array is the object's property tag list ("name=value"). Unknown: the field name
// of +44 and the owner of the list.
extern "C" {
extern const unsigned char _ctype_[];
unsigned strlen(const char*);
int strncmp(const char*, const char*, unsigned);
int atoi(const char*);

int findTagInt__10GameObjectPci(void* self, char* key, int occurrence) {
    char** list = *reinterpret_cast<char***>(static_cast<char*>(self) + 44);
    if (list == 0) return 0;
    if (*key != 0) {
        char* p = key;
        do {
            signed char c = *p;
            *p = (_ctype_[1 + c] & 1) ? c + 32 : c;
            p++;
        } while (*p != 0);
    }
    unsigned length = strlen(key);
    int i = 0;
    for (char** entries = *reinterpret_cast<char***>(static_cast<char*>(self) + 44); entries[i] != 0;
         entries = *reinterpret_cast<char***>(static_cast<char*>(self) + 44)) {
        char* text = entries[i];
        if (strncmp(text, key, length) == 0 && text[length] == '=') {
            if (--occurrence == -1) return atoi(text + length + 1);
        }
        i++;
    }
    return 0;
}
}
