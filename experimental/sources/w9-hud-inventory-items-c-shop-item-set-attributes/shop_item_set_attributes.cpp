// Observed shopItemSetAttributes__FiiPcPQ24item11description at 0x001b9fd8,
// 7020 bytes. Arguments: a player number, a list number, a text of the form
// "name+attribute+attribute..." and the item record being built (the 72-byte
// slot shape; its halfword at +34 is the item's slot number in that player's
// list). Returns 1 when every attribute was handled (also when the text has no
// '+'), 0 after showing a refusal message.
// Item records hold four attribute entries of 8 bytes starting at +36: a signed
// halfword id (-1 when unused) and three halfword values at +2, +4, +6.
// Steps, in order:
//  1. The text is copied to a 208-byte local buffer and cut at its first '+';
//     with no '+' the result is 1 at once. The part before the first '+' is not used.
//  2. refuse_dense = (strstr(text, "firedamage") or strstr(text, "colddamage") is
//     found) and the player's own slot (record+328+4616*list+72*slot) already has
//     three or more entries with an id other than -1.
//  3. For each following segment (cut at the next '+'): the three-int scratch is
//     cleared with memset(.., 0, 12); the number of used entries in the player's slot
//     is counted; when it is 4 or more, or refuse_dense holds, the refusal
//     "CantCombineTooMany" is shown and the result is 0. Otherwise the segment is
//     matched against 41 sscanf formats in the identify order below (a result
//     other than 0, including EOF, counts as a match) to get the attribute id
//     (-1 when none matches). The player's slot is searched for an entry with that
//     id; when one exists and its first value is non-zero, "CantCombineAttribute"
//     is shown and the result is 0. Otherwise the segment is matched again, in the
//     apply order (it differs only in having diseasedamage last), and for the first
//     matching format the item record's first entry whose id is -1 or already equal
//     to the attribute id gets that id and the numbers read (one, two or three, the
//     rest stored as 0); with all four entries taken by other ids nothing is stored.
//  4. A refusal plays game_butn_cantdo.anm from the fx lump when the player is local,
//     computes side = 0 when the word at inventory+56 equals the player, else 1 when
//     the word at inventory+172 does, else 0, gets the strings for
//     "CantCombineTitle" and the message key with textGetString and calls
//     multiHudAddHelpMessage(player, title, message, side).
// Unknown: the meaning of the attribute ids beyond the formats' names, and the
// 208-byte buffer limit.
#include "../inventory-items-record/record.hpp"
extern "C" {
extern const unsigned char norrath_inventory_ability_sound_lump_view[];
extern unsigned char inventory[];
char* strcpy(char* to, const char* from);
char* strstr(const char* text, const char* pattern);
char* strchr(const char* text, int c);
int sscanf(const char* text, const char* format, ...);
void* memset(void* target, int value, unsigned size);
int netPlayerIsLocal__Fi(int player);
char* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume);
unsigned short* textGetString__FPc(char* key);
void multiHudAddHelpMessage__FiPUsT1i(int player, unsigned short* title, unsigned short* message, int side);
}

namespace {
struct Rule {
    const char* format;
    int count;  // how many ints the format reads
    int id;
};

const Rule identify_rules[41] = {
    {"damageincrease%d", 1, 35}, {"stunresistance%d", 1, 30}, {"critical%d", 1, 29}, {"undeaddestroy%d", 1, 28},
    {"undeadbonus%d", 1, 27}, {"bugexploder%d", 1, 26}, {"bugzapper%d-%d", 2, 25}, {"firedamage%d-%d", 2, 21},
    {"colddamage%d-%d", 2, 22}, {"shockdamage%d-%d", 2, 24}, {"poisondamage%d-%d", 2, 23}, {"diseasedamage%d-%d", 2, 38},
    {"attackspeed%d", 1, 20}, {"requiredlevel%d", 1, 14}, {"requiredstrength%d", 1, 15}, {"requiredintelligence%d", 1, 16},
    {"requireddexterity%d", 1, 17}, {"requiredstamina%d", 1, 18}, {"requiredclass%d", 1, 19}, {"armorbonus%d", 1, 13},
    {"healthregeneration%d", 1, 11}, {"manaregeneration%d", 1, 12}, {"healthbonus%d", 1, 5}, {"manabonus%d", 1, 6},
    {"strengthbonus%d", 1, 7}, {"intelligencebonus%d", 1, 8}, {"dexteritybonus%d", 1, 9}, {"staminabonus%d", 1, 10},
    {"resistfire%d", 1, 0}, {"resistcold%d", 1, 1}, {"resistpoison%d", 1, 2}, {"resistdisease%d", 1, 3},
    {"resistmagic%d", 1, 4}, {"poisonburst%d,%d", 2, 33}, {"fireburst%d,%d-%d", 3, 31}, {"frostburst%d,%d-%d", 3, 32},
    {"lightningburst%d,%d-%d", 3, 34}, {"weightmodifier%d", 1, 36}, {"costmodifier%d", 1, 37}, {"requiredblunt%d", 1, 39},
    {"requiredslashing%d", 1, 40}};

// The apply order: the identify order with diseasedamage moved to the end.
const Rule apply_rules[41] = {
    {"damageincrease%d", 1, 35}, {"stunresistance%d", 1, 30}, {"critical%d", 1, 29}, {"undeaddestroy%d", 1, 28},
    {"undeadbonus%d", 1, 27}, {"bugexploder%d", 1, 26}, {"bugzapper%d-%d", 2, 25}, {"firedamage%d-%d", 2, 21},
    {"colddamage%d-%d", 2, 22}, {"shockdamage%d-%d", 2, 24}, {"poisondamage%d-%d", 2, 23},
    {"attackspeed%d", 1, 20}, {"requiredlevel%d", 1, 14}, {"requiredstrength%d", 1, 15}, {"requiredintelligence%d", 1, 16},
    {"requireddexterity%d", 1, 17}, {"requiredstamina%d", 1, 18}, {"requiredclass%d", 1, 19}, {"armorbonus%d", 1, 13},
    {"healthregeneration%d", 1, 11}, {"manaregeneration%d", 1, 12}, {"healthbonus%d", 1, 5}, {"manabonus%d", 1, 6},
    {"strengthbonus%d", 1, 7}, {"intelligencebonus%d", 1, 8}, {"dexteritybonus%d", 1, 9}, {"staminabonus%d", 1, 10},
    {"resistfire%d", 1, 0}, {"resistcold%d", 1, 1}, {"resistpoison%d", 1, 2}, {"resistdisease%d", 1, 3},
    {"resistmagic%d", 1, 4}, {"poisonburst%d,%d", 2, 33}, {"fireburst%d,%d-%d", 3, 31}, {"frostburst%d,%d-%d", 3, 32},
    {"lightningburst%d,%d-%d", 3, 34}, {"weightmodifier%d", 1, 36}, {"costmodifier%d", 1, 37}, {"requiredblunt%d", 1, 39},
    {"requiredslashing%d", 1, 40}, {"diseasedamage%d-%d", 2, 38}};

int scan(const char* segment, const Rule& rule, int* numbers) {
    if (rule.count == 1) return sscanf(segment, rule.format, numbers);
    if (rule.count == 2) return sscanf(segment, rule.format, numbers, numbers + 1);
    return sscanf(segment, rule.format, numbers, numbers + 1, numbers + 2);
}

short& entry_id(unsigned char* slot, int entry) { return *reinterpret_cast<short*>(slot + 36 + 8 * entry); }
short& entry_value(unsigned char* slot, int entry, int which) { return *reinterpret_cast<short*>(slot + 38 + 8 * entry + 2 * which); }

int used_entries(unsigned char* slot) {
    int used = 0;
    for (int entry = 0; entry < 4; ++entry) {
        if (entry_id(slot, entry) != -1) ++used;
    }
    return used;
}

int refuse(int owner, const char* message_key) {
    if (netPlayerIsLocal__Fi(owner) != 0) {
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0(const_cast<char*>(reinterpret_cast<const char*>(norrath_inventory_ability_sound_lump_view)),
                                    const_cast<char*>("game_butn_cantdo.anm")),
            0, 1.0f);
    }
    int side;
    if (*reinterpret_cast<int*>(inventory + 56) == owner) side = 0;
    else side = *reinterpret_cast<int*>(inventory + 172) == owner ? 1 : 0;
    unsigned short* title = textGetString__FPc(const_cast<char*>("CantCombineTitle"));
    unsigned short* message = textGetString__FPc(const_cast<char*>(message_key));
    multiHudAddHelpMessage__FiPUsT1i(owner, title, message, side);
    return 0;
}
}

extern "C" int shopItemSetAttributes__FiiPcPQ24item11description(int owner, int list, char* text, unsigned char* item) {
    using namespace inventory_items;
    char buffer[208];
    int numbers[3];
    strcpy(buffer, text);
    bool dense = false;
    if (strstr(text, "firedamage") != 0 || strstr(text, "colddamage") != 0) {
        unsigned char* own = reinterpret_cast<unsigned char*>(player_lists(owner)) + 4616 * list + 72 * *reinterpret_cast<short*>(item + 34);
        dense = used_entries(own) >= 3;
    }
    char* next = strchr(buffer, '+');
    if (next == 0) return 1;
    *next = 0;
    ++next;
    while (next != 0) {
        char* segment = next;
        next = strchr(segment, '+');
        if (next != 0) {
            *next = 0;
            ++next;
        }
        memset(numbers, 0, 12);
        unsigned char* own = reinterpret_cast<unsigned char*>(player_lists(owner)) + 4616 * list + 72 * *reinterpret_cast<short*>(item + 34);
        if (used_entries(own) >= 4 || dense) return refuse(owner, "CantCombineTooMany");
        int id = -1;
        for (int rule = 0; rule < 41; ++rule) {
            if (scan(segment, identify_rules[rule], numbers) != 0) {
                id = identify_rules[rule].id;
                break;
            }
        }
        own = reinterpret_cast<unsigned char*>(player_lists(owner)) + 4616 * list + 72 * *reinterpret_cast<short*>(item + 34);
        int existing = 0;
        for (int entry = 0; entry < 4; ++entry) {
            if (entry_id(own, entry) == id) {
                existing = entry_value(own, entry, 0);
                break;
            }
        }
        if (existing != 0) return refuse(owner, "CantCombineAttribute");
        for (int rule = 0; rule < 41; ++rule) {
            const Rule& candidate = apply_rules[rule];
            if (scan(segment, candidate, numbers) == 0) continue;
            for (int entry = 0; entry < 4; ++entry) {
                if (entry_id(item, entry) == -1 || entry_id(item, entry) == candidate.id) {
                    entry_id(item, entry) = static_cast<short>(candidate.id);
                    entry_value(item, entry, 0) = static_cast<short>(numbers[0]);
                    entry_value(item, entry, 1) = static_cast<short>(candidate.count >= 2 ? numbers[1] : 0);
                    entry_value(item, entry, 2) = static_cast<short>(candidate.count >= 3 ? numbers[2] : 0);
                    break;
                }
            }
            break;
        }
    }
    return 1;
}
