// Observed: eqFind__FPci at 0x001951d0 is 388 bytes (a local symbol). With name and flag:
//  1. flag nonzero picks nmWGroups and wGroup, flag zero picks nmEqGroups and eqGroup; each
//     group table has 12-byte records (word 0 entries, word 4 group name, word 8 entry count);
//  2. name is copied into an 80-byte local buffer and the text from its last '.' on is cut off;
//  3. an empty buffer returns 0;
//  4. otherwise the trailing run of digits of the text is located by scanning back from the last
//     character; the scan never takes the first character of the text as a digit, and a text
//     without a final digit has an empty run. The run is converted with atoi and the text is
//     cut where the run began; the number is -1 when the run is empty;
//  5. each group in order whose name equals the cut text by strcmp has its entries tested in
//     order, entries being 8 bytes apart, and the first whose halfword at offset 0 equals the
//     number is returned (as a pointer to the entry). Otherwise 0 is returned.
// Pointers kept in memory are 32-bit words on the target.
// Inferred: names such as "armor3.tex" are split into a group name and an index, with the
// extension dropped, and the entries are file table entries of the equipment caches.
// Unknown: the layout of the entries beyond their first halfword (the cache code reads more
// than 8 bytes from the result), and the type of the group records.
#ifdef NORRATH_HOST_CONTRACT
extern unsigned char* norrath_host_base;
#define NORRATH_PTR(w) (norrath_host_base + (w))
#define NORRATH_WORD(p) static_cast<unsigned>(reinterpret_cast<unsigned char*>(p) - norrath_host_base)
#else
#define NORRATH_PTR(w) reinterpret_cast<unsigned char*>(w)
#define NORRATH_WORD(p) static_cast<unsigned>(reinterpret_cast<unsigned long>(p))
#endif

extern "C" {
extern int nmEqGroups;
extern int nmWGroups;
extern unsigned eqGroup;
extern unsigned wGroup;
char* strcpy(char* to, const char* from);
char* strrchr(const char* text, int c);
__SIZE_TYPE__ strlen(const char* text);
int strcmp(const char* left, const char* right);
int atoi(const char* text);
}

namespace {
struct Group {
    unsigned entries;
    unsigned name;
    int count;
};
}

extern "C" unsigned char* eqFind__FPci(char* name, int flag) {
    int count;
    unsigned groups;
    if (flag) {
        count = nmWGroups;
        groups = wGroup;
    } else {
        count = nmEqGroups;
        groups = eqGroup;
    }
    char text[80];
    strcpy(text, name);
    if (strrchr(text, '.')) *strrchr(text, '.') = 0;
    if (text[0] == 0) return 0;
    char* digits = text + strlen(text) - 1;
    if (text < digits && *digits >= '0' && *digits <= '9') {
        --digits;
        while (text < digits && *digits >= '0' && *digits <= '9') --digits;
    }
    ++digits;
    int number = -1;
    int parsed = atoi(digits);
    char first = *digits;
    *digits = 0;
    if (first) number = parsed;
    const Group* group = reinterpret_cast<const Group*>(NORRATH_PTR(groups));
    for (int i = 0; i < count; ++i, ++group) {
        if (strcmp(text, reinterpret_cast<const char*>(NORRATH_PTR(group->name))) != 0) continue;
        unsigned char* entry = NORRATH_PTR(group->entries);
        for (int k = 0; k < group->count; ++k, entry += 8) {
            if (*reinterpret_cast<short*>(entry) == number) return entry;
        }
    }
    return 0;
}
