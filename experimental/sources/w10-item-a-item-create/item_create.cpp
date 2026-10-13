// Observed create__4itemPcPQ24item11descriptionii at 0x00105f60, 5428 bytes
// (char* name, description* D, int mesh, int named). D is the 72-byte item
// description. Reads of 8-byte fields are the image's own `ld`/`sd`.
//   1. The name is copied into a local text B (strcpy). B is cut at the first '+';
//      the rest, if any, is a list of '+'-separated option tokens.
//   2. Table lookup: with `named` 0 the 64-byte entries of _4item$itemTable
//      (0x0032b320) are searched by strcmp(entry word +0, B); with `named` nonzero
//      the entries of _4item$namedItemTable (0x0031fbe0) are searched with the
//      original `name` (not B). The table ends with an entry whose first word is 0;
//      a name that is not found selects entry 0. Entry E: D word +0 = E, halfword
//      +4 = halfword E+12, 8 bytes at +8 = the 8 bytes at E+40.
//      Before that, D is overwritten with a 72-byte local image in which halfwords
//      +16..+22 and +24..+30 are 0, bytes +32 and +33 are 0xff and the id halfwords
//      of the four modifier records at +36, +44, +52, +60 are -1; every other byte
//      comes from the (uninitialized) local, so bytes +34..+35 and the padding of
//      the modifier records hold stack garbage.
//   3. For each option token, three int slots are zeroed and the tokens are tried
//      in this order with sscanf; the first format that converts at least one
//      value (a nonzero return, EOF counts) is used. A modifier record is
//      {halfword id, v1, v2, v3}; "add(id, v...)" takes the first of the four
//      records whose id is -1 or equals `id` (and does nothing when all four are
//      taken by other ids), sets the id and stores the given values (missing ones
//      0) into v1, v2, v3.
//        damageincrease%d 35; stunresistance%d 30; critical%d 29; undeaddestroy%d
//        28; undeadbonus%d 27; bugexploder%d 26; bugzapper%d-%d 25 (two values);
//        firedamage%d-%d 21; colddamage%d-%d 22; shockdamage%d-%d 24;
//        poisondamage%d-%d 23; attackspeed%d 20; requiredlevel%d 14;
//        requiredstrength%d 15; requiredintelligence%d 16; requireddexterity%d 17;
//        requiredstamina%d 18; requiredclass%d 19; armorbonus%d 13;
//        healthregeneration%d 11; manaregeneration%d 12; healthbonus%d 5;
//        manabonus%d 6; strengthbonus%d 7; intelligencebonus%d 8; dexteritybonus%d
//        9; staminabonus%d 10; resistfire%d 0; resistcold%d 1; resistpoison%d 2;
//        resistdisease%d 3; resistmagic%d 4; poisonburst%d,%d 33 (two values);
//        fireburst%d,%d-%d 31 (three values); frostburst%d,%d-%d 32;
//        lightningburst%d,%d-%d 34; weightmodifier%d 36; costmodifier%d 37;
//        requiredblunt%d 39; requiredslashing%d 40; then "mesh%d" replaces the
//        `mesh` argument (no record); then diseasedamage%d-%d 38 (two values).
//   4. A final 5-bit field of the 8 bytes at D+8 (bits 11..15) is set: when `mesh`
//      is -1 the value is chosen at random below n, where n is bits 11..15 of the 8
//      bytes at E+40 (n <= 0 gives 0): syncRandState = syncRandState * 0x19660d +
//      0x3c6ef35f and ((unsigned)state >> 16) % n. Otherwise it is `mesh`. The field
//      is stored as (D8 & 0xffffffffffff07ff) | (value << 11) (a 32-bit shift,
//      sign-extended).
// Unknown: what most of the ids mean, what the entry fields at E+12 and E+40 are
// (the 5-bit field looks like a count of model variants), and what `named` selects.
extern "C" {
extern unsigned char _4item$itemTable[];
extern unsigned char _4item$namedItemTable[];
extern int syncRandState;
char* strcpy(char*, const char*);
char* strchr(const char*, int);
int strcmp(const char*, const char*);
int sscanf(const char*, const char*, ...);
void* memset(void*, int, __SIZE_TYPE__);

#ifdef ITEM_CREATE_HOST_POINTER
// Host check only: maps a 32-bit table word to a host pointer.
extern "C++" char* ITEM_CREATE_HOST_POINTER(unsigned);
static inline char* text(unsigned w) { return ITEM_CREATE_HOST_POINTER(w); }
#else
static inline char* text(unsigned w) { return reinterpret_cast<char*>(static_cast<__UINTPTR_TYPE__>(w)); }
#endif

static inline short& half(unsigned char* base, int off) { return *reinterpret_cast<short*>(base + off); }

static void add(unsigned char* d, int id, int v1, int v2, int v3) {
    for (int i = 0; i < 4; i++) {
        unsigned char* record = d + 36 + 8 * i;
        if (half(record, 0) == -1 || half(record, 0) == id) {
            half(record, 0) = static_cast<short>(id);
            half(record, 2) = static_cast<short>(v1);
            half(record, 4) = static_cast<short>(v2);
            half(record, 6) = static_cast<short>(v3);
            return;
        }
    }
}

static bool one(unsigned char* d, char* token, const char* format, int id, int* v) {
    if (!sscanf(token, format, &v[0])) return false;
    add(d, id, v[0], 0, 0);
    return true;
}

static bool two(unsigned char* d, char* token, const char* format, int id, int* v) {
    if (!sscanf(token, format, &v[0], &v[1])) return false;
    add(d, id, v[0], v[1], 0);
    return true;
}

static bool three(unsigned char* d, char* token, const char* format, int id, int* v) {
    if (!sscanf(token, format, &v[0], &v[1], &v[2])) return false;
    add(d, id, v[0], v[1], v[2]);
    return true;
}

void create__4itemPcPQ24item11descriptionii(char* name, unsigned char* d, int mesh, int named) {
    char buffer[208];
    unsigned char local[72];
    int v[3];
    strcpy(buffer, name);
    for (int i = 0; i < 4; i++) half(local, 36 + 8 * i) = -1;
    local[32] = 0xff;
    local[33] = 0xff;
    for (int i = 0; i < 4; i++) {
        half(local, 16 + 2 * i) = 0;
        half(local, 24 + 2 * i) = 0;
    }
    __asm__ volatile("" : "+m"(local));  // the remaining bytes are copied as they are (stack garbage in the image)
    for (int i = 0; i < 72; i++) d[i] = local[i];
    char* rest = strchr(buffer, '+');
    if (rest) {
        *rest = 0;
        rest++;
    }
    unsigned char* table = named ? _4item$namedItemTable : _4item$itemTable;
    const char* key = named ? name : buffer;
    int index = 0;
    while (*reinterpret_cast<unsigned*>(table + index * 64) != 0 && strcmp(text(*reinterpret_cast<unsigned*>(table + index * 64)), key) != 0) index++;
    if (*reinterpret_cast<unsigned*>(table + index * 64) == 0) index = 0;
    unsigned char* entry = table + index * 64;
    *reinterpret_cast<unsigned*>(d) = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(entry));
    half(d, 4) = half(entry, 12);
    *reinterpret_cast<long long*>(d + 8) = *reinterpret_cast<long long*>(entry + 40);
    while (rest != nullptr) {
        char* token = rest;
        rest = strchr(token, '+');
        if (rest) {
            *rest = 0;
            rest++;
        }
        memset(v, 0, 12);
        if (one(d, token, "damageincrease%d", 35, v)) continue;
        if (one(d, token, "stunresistance%d", 30, v)) continue;
        if (one(d, token, "critical%d", 29, v)) continue;
        if (one(d, token, "undeaddestroy%d", 28, v)) continue;
        if (one(d, token, "undeadbonus%d", 27, v)) continue;
        if (one(d, token, "bugexploder%d", 26, v)) continue;
        if (two(d, token, "bugzapper%d-%d", 25, v)) continue;
        if (two(d, token, "firedamage%d-%d", 21, v)) continue;
        if (two(d, token, "colddamage%d-%d", 22, v)) continue;
        if (two(d, token, "shockdamage%d-%d", 24, v)) continue;
        if (two(d, token, "poisondamage%d-%d", 23, v)) continue;
        if (one(d, token, "attackspeed%d", 20, v)) continue;
        if (one(d, token, "requiredlevel%d", 14, v)) continue;
        if (one(d, token, "requiredstrength%d", 15, v)) continue;
        if (one(d, token, "requiredintelligence%d", 16, v)) continue;
        if (one(d, token, "requireddexterity%d", 17, v)) continue;
        if (one(d, token, "requiredstamina%d", 18, v)) continue;
        if (one(d, token, "requiredclass%d", 19, v)) continue;
        if (one(d, token, "armorbonus%d", 13, v)) continue;
        if (one(d, token, "healthregeneration%d", 11, v)) continue;
        if (one(d, token, "manaregeneration%d", 12, v)) continue;
        if (one(d, token, "healthbonus%d", 5, v)) continue;
        if (one(d, token, "manabonus%d", 6, v)) continue;
        if (one(d, token, "strengthbonus%d", 7, v)) continue;
        if (one(d, token, "intelligencebonus%d", 8, v)) continue;
        if (one(d, token, "dexteritybonus%d", 9, v)) continue;
        if (one(d, token, "staminabonus%d", 10, v)) continue;
        if (one(d, token, "resistfire%d", 0, v)) continue;
        if (one(d, token, "resistcold%d", 1, v)) continue;
        if (one(d, token, "resistpoison%d", 2, v)) continue;
        if (one(d, token, "resistdisease%d", 3, v)) continue;
        if (one(d, token, "resistmagic%d", 4, v)) continue;
        if (two(d, token, "poisonburst%d,%d", 33, v)) continue;
        if (three(d, token, "fireburst%d,%d-%d", 31, v)) continue;
        if (three(d, token, "frostburst%d,%d-%d", 32, v)) continue;
        if (three(d, token, "lightningburst%d,%d-%d", 34, v)) continue;
        if (one(d, token, "weightmodifier%d", 36, v)) continue;
        if (one(d, token, "costmodifier%d", 37, v)) continue;
        if (one(d, token, "requiredblunt%d", 39, v)) continue;
        if (one(d, token, "requiredslashing%d", 40, v)) continue;
        if (sscanf(token, "mesh%d", &v[0])) {
            mesh = v[0];
            continue;
        }
        two(d, token, "diseasedamage%d-%d", 38, v);
    }
    int value;
    if (mesh == -1) {
        long long n = (*reinterpret_cast<long long*>(entry + 40) >> 11) & 0x1f;
        value = static_cast<int>(n);
        if (n > 0) {
            syncRandState = syncRandState * 0x19660d + 0x3c6ef35f;
            value = static_cast<int>(static_cast<unsigned>(syncRandState) >> 16) % static_cast<int>(n);
        }
    } else {
        value = mesh;
    }
    long long field = static_cast<long long>(static_cast<int>(static_cast<unsigned>(value) << 11));
    *reinterpret_cast<long long*>(d + 8) = (*reinterpret_cast<long long*>(d + 8) & static_cast<long long>(0xffffffffffff07ffull)) | field;
}
}
