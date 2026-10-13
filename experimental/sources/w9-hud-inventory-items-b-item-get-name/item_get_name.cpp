// Observed getName__4itemPQ24item11descriptioni at 0x00101c30, 1396 bytes.
// Arguments: the item record (despite the mangled first type; see getWeight) and an
// integer `decorate`. Returns a pointer to a static buffer `name` (240 halfwords,
// file-local `name.1085` at 0x00486ea0) holding the item's display text as
// halfword characters. Item fields: +0 description pointer (the description's
// first word is a name key string), +8 64-bit flags, +32 byte `prefix`, +33 byte
// `suffix` (255 means none for both). `_4item$prefixTable` has 40-byte rows whose
// first word is a key string; `_4item$suffixTable` has 20-byte rows likewise.
// The buffer is cleared first. Then:
//  * If `decorate` is zero or flags bit 9 is set: the text of "<key>_name" (via
//    textGetString__FPc) is appended and findAndStripGender__4itemPUs runs on the
//    buffer. Done (no '+' trimming in this branch).
//  * Otherwise a key buffer is made from the description key with everything from the
//    first '+' dropped, then rewritten as "<key>_name" (sprintf). Its text is appended
//    to the buffer and findAndStripGender returns a gender code g (first call); the
//    buffer is cleared again. If the description or its key is null the (empty)
//    buffer is returned. Then, by the signed byte `language` (0x004844b4):
//      language 4: the key text is rebuilt from a second key buffer into the buffer and
//        g is recomputed by findAndStripGender. When prefix != 255, the attribute text
//        getAttributeName__4itemiPc(m, prefixRow.key) is run through usprintf (eight
//        zero floats) into the buffer, " " is appended, then the key text again, and
//        findAndStripGender runs once more; m is 0 for g 1, 1 for g 2, 3 for g 4, else 2.
//      language 3: the key text is rebuilt and g recomputed; when prefix != 255 " " and
//        getAttributeName(m, prefixRow.key) are appended, with m 0 for g 1, 1 for g 2,
//        2 for g 3, 3 for g 4, else -1.
//      other languages: when prefix != 255 getAttributeName(g, prefixRow.key) (g from the
//        first call) and " " are appended, then the key text is rebuilt and appended and
//        findAndStripGender runs.
//    If suffix != 255 the suffix text follows: language 3 and 4 append " " and
//    textGetString(suffixRow.key); other languages append " ", the text of the key "of"
//    (textGetString), " ", and getAttributeName(g, suffixRow.key).
// Names and meanings of the gender codes, the languages and flag bit 9 are not
// established; they are named after their uses only. The callees, tables and
// `language` are not defined here.
extern "C" {
extern unsigned char _4item$prefixTable[];
extern unsigned char _4item$suffixTable[];
extern signed char language;
int sprintf(char*, const char*, ...);
char* strcpy(char*, const char*);
char* strchr(const char*, int);
unsigned short* textGetString__FPc(char*);
void ustrcat__FPUsT0(unsigned short*, unsigned short*);
void ustrcat__FPUsPc(unsigned short*, const char*);
int findAndStripGender__4itemPUs(unsigned short*);
unsigned short* getAttributeName__4itemiPc(int, char*);
void usprintf__FPUsT0ffffffff(unsigned short*, unsigned short*, float, float, float, float, float, float, float, float);
}

struct NameDescription {
    char* key;
};

struct NameItem {
    const NameDescription* description;
    unsigned char gap[8 - sizeof(void*)];
    long long flags;
    unsigned char gap2[32 - 16];
    unsigned char prefix;
    unsigned char suffix;
};

static_assert(sizeof(void*) != 4 || (__builtin_offsetof(NameItem, flags) == 8 && __builtin_offsetof(NameItem, prefix) == 32), "item layout");

static unsigned short name[240];

static char* rowKey(const unsigned char* table, int stride, int index) {
    return *reinterpret_cast<char* const*>(table + index * stride);
}

// Copy `key` into `buffer`, drop everything from the first '+', and turn it into "<key>_name".
static void nameKey(char* buffer, const char* key) {
    strcpy(buffer, key);
    char* plus = strchr(buffer, '+');
    if (plus != nullptr) *plus = 0;
    sprintf(buffer, "%s_name", buffer);
}

extern "C" unsigned short* getName__4itemPQ24item11descriptioni(const NameItem* item, int decorate) {
    name[0] = 0;
    if (decorate == 0 || (item->flags & 0x200LL) != 0) {
        char key[208];
        sprintf(key, "%s_name", item->description->key);
        ustrcat__FPUsT0(name, textGetString__FPc(key));
        findAndStripGender__4itemPUs(name);
        return name;
    }
    char first[208];
    nameKey(first, item->description->key);
    ustrcat__FPUsT0(name, textGetString__FPc(first));
    int gender = findAndStripGender__4itemPUs(name);
    name[0] = 0;
    const NameDescription* description = item->description;
    if (description == nullptr || description->key == nullptr) return name;
    int lang = language;
    if (lang == 4) {
        char key[208];
        nameKey(key, description->key);
        ustrcat__FPUsT0(name, textGetString__FPc(key));
        int g = findAndStripGender__4itemPUs(name);
        if (item->prefix != 255) {
            int mode = g == 1 ? 0 : g == 2 ? 1 : g == 4 ? 3 : 2;
            unsigned short* attribute = getAttributeName__4itemiPc(mode, rowKey(_4item$prefixTable, 40, item->prefix));
            usprintf__FPUsT0ffffffff(name, attribute, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            ustrcat__FPUsPc(name, " ");
            ustrcat__FPUsT0(name, textGetString__FPc(key));
            findAndStripGender__4itemPUs(name);
        }
    } else if (lang == 3) {
        char key[208];
        nameKey(key, description->key);
        ustrcat__FPUsT0(name, textGetString__FPc(key));
        int g = findAndStripGender__4itemPUs(name);
        if (item->prefix != 255) {
            ustrcat__FPUsPc(name, " ");
            int mode = g == 4 ? 3 : g == 2 ? 1 : g == 3 ? 2 : g == 1 ? 0 : -1;
            ustrcat__FPUsT0(name, getAttributeName__4itemiPc(mode, rowKey(_4item$prefixTable, 40, item->prefix)));
        }
    } else {
        if (item->prefix != 255) {
            ustrcat__FPUsT0(name, getAttributeName__4itemiPc(gender, rowKey(_4item$prefixTable, 40, item->prefix)));
            ustrcat__FPUsPc(name, " ");
        }
        char key[208];
        nameKey(key, description->key);
        ustrcat__FPUsT0(name, textGetString__FPc(key));
        findAndStripGender__4itemPUs(name);
    }
    if (item->suffix == 255) return name;
    if (lang == 3 || lang == 4) {
        ustrcat__FPUsPc(name, " ");
        ustrcat__FPUsT0(name, textGetString__FPc(rowKey(_4item$suffixTable, 20, item->suffix)));
    } else {
        ustrcat__FPUsPc(name, " ");
        ustrcat__FPUsT0(name, textGetString__FPc(const_cast<char*>("of")));
        ustrcat__FPUsPc(name, " ");
        ustrcat__FPUsT0(name, getAttributeName__4itemiPc(gender, rowKey(_4item$suffixTable, 20, item->suffix)));
    }
    return name;
}
