// Observed playerAllocateArmorCache__Fii at 0x001955e8, 948 bytes, 304-byte frame. Arguments: a
// player index p and a race (the class index of the player record, below 10). The armor cache
// norrath_player_armor_cache_view_7cb770 has ten rows (cache kinds 0 to 9) of 88-byte elements per
// player: element = cache + 352 * row + 88 * p. Element words used: +0 the allocated block, +4 its
// size, the signed byte at +84 selects the group table, bytes +85 and +86 are set here.
//  1. ten sizes start at zero (memset). Every row of _4item$itemTable (64-byte rows ending at a null name
//     word) is looked at: the text pointer at +48 must be nonzero and equipmentSlot2Cache[signed
//     byte at +28] must not be -1; call that value row r.
//  2. A name is made: for r in 6 to 9 the text is copied as is; otherwise, when race is 6 or 7 only
//     texts containing "cloth" are kept, for other races only texts without it, and the name is
//     sprintf("%s_%s", racePrefix[race], text). Trailing decimal digits are then cut off.
//  3. The group table is wGroup/nmWGroups when the byte at element +84 (element of row r) is
//     nonzero, else eqGroup/nmEqGroups; the first 12-byte group whose name word (+4) is
//     strcmp-equal to the name is used. A group has an array pointer (+0) and a count (+8); with a
//     positive count the array is walked two counts at a time in 16-byte steps and sizes[r] becomes
//     the larger of itself and ((halfword +2) + (halfword +10)) << 11 (rounded up to 2048).
//  4. sizes[8] becomes the larger of sizes[8] and sizes[7].
//  5. For each row 0 to 9: the block memAlloc__FiPc(sizes[row], armorName[row]) is stored at +0, the
//     size at +4, the byte race + 1 at +85, and the byte at +86 is 1 when nmPlayers is at least 2
//     or coreForceSmallCharacters is nonzero, else 0.
// Inferred: this sizes and allocates the model caches of a player's armor pieces from the largest
// group of parts the player's items could use. Unknown: the group records' other words and the
// meaning of the bytes at +84, +85 and +86.
extern "C" {
extern unsigned char norrath_player_armor_cache_view_7cb770[];
extern unsigned char _4item$itemTable[];
extern int norrath_equipment_slot_to_cache[] __asm__("equipmentSlot2Cache.1137");
extern const char* norrath_armor_name[] __asm__("armorName.1138");
extern char* racePrefix[];
extern int nmWGroups;
extern int nmEqGroups;
extern unsigned wGroup;
extern unsigned eqGroup;
extern int nmPlayers;
extern signed char coreForceSmallCharacters;
void* memAlloc__FiPc(int size, char* label);
int sprintf(char* out, const char* format, ...);
char* strcpy(char* to, const char* from);
void* memset(void* to, int value, __SIZE_TYPE__ size);
char* strstr(const char* haystack, const char* needle);
int strcmp(const char* left, const char* right);
__SIZE_TYPE__ strlen(const char* text);
}

#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_base;
#endif

namespace {
// Pointer words of the image are 32-bit; a host check maps them to offsets in one arena.
inline unsigned char* at(unsigned word) {
#ifdef NORRATH_HOST_ARENA
    return norrath_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
inline unsigned wordOf(const void* pointer) {
#ifdef NORRATH_HOST_ARENA
    return static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - norrath_host_base);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
#endif
}
inline unsigned& word(unsigned char* base, int offset) { return *reinterpret_cast<unsigned*>(base + offset); }
inline int& integer(unsigned char* base, int offset) { return *reinterpret_cast<int*>(base + offset); }
inline short halfword(const unsigned char* base, int offset) { return *reinterpret_cast<const short*>(base + offset); }
}

extern "C" void playerAllocateArmorCache__Fii(int player, int race) {
    int sizes[10];
    memset(sizes, 0, sizeof sizes);
    char name[160];
    const bool clothRace = static_cast<unsigned>(race - 6) < 2u;
    for (unsigned char* row = _4item$itemTable; word(row, 0) != 0; row += 64) {
        const char* text = reinterpret_cast<const char*>(at(word(row, 48)));
        if (word(row, 48) == 0) continue;
        const int cacheRow = norrath_equipment_slot_to_cache[static_cast<signed char>(row[28])];
        if (cacheRow == -1) continue;
        if (static_cast<unsigned>(cacheRow - 6) < 4u) {
            strcpy(name, text);
        } else {
            if (clothRace) {
                if (strstr(text, "cloth") == nullptr) continue;
            } else {
                if (strstr(text, "cloth") != nullptr) continue;
            }
            sprintf(name, "%s_%s", racePrefix[race], text);
        }
        for (;;) {
            if (static_cast<signed char>(name[strlen(name) - 1]) < 48) break;
            if (!(static_cast<signed char>(name[strlen(name) - 1]) < 58)) break;
            name[strlen(name) - 1] = 0;
        }
        unsigned char* element = norrath_player_armor_cache_view_7cb770 + 88 * player + 352 * cacheRow;
        unsigned char* group = nullptr;
        if (static_cast<signed char>(element[84]) != 0) {
            for (int i = 0; i < nmWGroups; ++i) {
                unsigned char* candidate = at(wGroup) + 12 * i;
                if (strcmp(name, reinterpret_cast<const char*>(at(word(candidate, 4)))) == 0) {
                    group = candidate;
                    break;
                }
            }
        } else {
            for (int i = 0; i < nmEqGroups; ++i) {
                unsigned char* candidate = at(eqGroup) + 12 * i;
                if (strcmp(name, reinterpret_cast<const char*>(at(word(candidate, 4)))) == 0) {
                    group = candidate;
                    break;
                }
            }
        }
        if (group == nullptr) continue;
        if (integer(group, 8) <= 0) continue;
        const unsigned char* part = at(word(group, 0));
        for (int count = 0; count < integer(group, 8); count += 2, part += 16) {
            const int wanted = (((halfword(part, 2) + halfword(part, 10)) << 11) + 2047) & -2048;
            const int current = sizes[cacheRow];
            sizes[cacheRow] = current < wanted ? wanted : current;
        }
    }
    sizes[8] = sizes[8] < sizes[7] ? sizes[7] : sizes[8];
    for (int row = 0; row < 10; ++row) {
        unsigned char* element = norrath_player_armor_cache_view_7cb770 + 88 * player + 352 * row;
        word(element, 0) = wordOf(memAlloc__FiPc(sizes[row], const_cast<char*>(norrath_armor_name[row])));
        integer(element, 4) = sizes[row];
        element[85] = static_cast<unsigned char>(race + 1);
        element[86] = (nmPlayers >= 2 || coreForceSmallCharacters != 0) ? 1 : 0;
    }
}
