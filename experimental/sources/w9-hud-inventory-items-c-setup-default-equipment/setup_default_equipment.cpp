// Observed setupDefaultEquipment__Fi at 0x0019e780, 11948 bytes: fills a new
// player's inventory. owner is the player number; the player record is 23984
// bytes (see record.hpp). Names passed to item::create are short item
// identifiers. Observed, in order:
//  0. A 72-byte local item template is prepared for item::create: the four
//     halfwords at +36, +44, +52, +60 are -1, the bytes at +32 and +33 are 0xff
//     and the halfwords at +16..+22 and +24..+30 are 0. All other bytes stay as
//     they were on the stack. Calls below are give(name, a, b, announce):
//     create(name, template, a, b) then addItemToInventory(owner, template,
//     announce) (the third argument is named for how w8-hud-text-items-a uses
//     it), which returns the slot number used for equips. Equips are
//     equipItem(owner, slot address, 1) with the slot in list 0 (record+328) or
//     list 1 (record+4944), 72 bytes per slot. Some gives store a value in the
//     template byte at +32 between the two calls ("marked"); the byte then
//     stays for later creates.
//  1. skirmish = strstr(levelName + 81, "skirm") != 0.
//  2. flags = word at pad[88 * owner + 8]. When bit 2 is set: the word cheatMode
//     becomes 1; a feat table is chosen by the word at record+80: 0 Warrior, 1
//     Cleric, 2 ShadowKnight, 4 Wizard (any other value uses none). Entries are
//     56 bytes; the entry's first bytes are searched for "END" and the walk
//     stops at the first entry that has it; for each earlier entry the 64-bit
//     value at entry+40 goes to givePlayerFeat(feat, owner, -1) and
//     playerSetFeatRank(owner, feat, 1). The flags word is read again after a
//     table walk. The give-everything list below runs when bit 2 is set and
//     the byte MC_Debug is non-zero.
//  3. Give-everything list (names and counts are in the function bodies below).
//     Ten staves and bows; ten rounds each of gaspotion, firepotion and
//     explodingpotion (the create runs every round); ten rounds of twenty-two
//     crafting items; weapons, where one equip depends on the word at record+84
//     being not 5 and a later one on it being 5; rod variants; rings, earrings,
//     chokers and amulets in loops of 6, 8, 4 and 3 whose loop index is the first
//     create argument; then armor sets generated with sprintf over levels 1..4
//     and five pieces, or the shield list when armorDebug[3] is set. When none
//     of armorDebug[0..2] is set one of them is chosen with the LCG on eRandState
//     (state * 1664525 + 1013904223, bits 16.. modulo 3). Armor pieces are marked
//     with 0, 1, 2, 8 or 56 for piece 0, 1, 2, 3, 4; level 1 piece 0 is equipped
//     from list 1. Class 4 uses the three cloth sets (armorDebug[0..2]); the other
//     classes use leather, chain and plate.
//  4. Otherwise, unless skirmish: a class-dependent starting kit (leather armor
//     for every class but 4, which gets cloth; one or more weapons by class;
//     arrows for class 3; a recall potion and healing and rejuvenation potions).
//  5. Always: skills by class (0, 2 and 3 get skills 60 and 61; 1 gets 60 and 16;
//     4 gets 60 and 23; each with givePlayerSkill then rank 1), then
//     playerRecomputeWeight(owner).
// Unknown: what the feat table fields, the class numbers, the record words at
// +80/+84 and the template byte at +32 mean. The name symbols for the pad,
// skill-tree and debug storage come from the image's symbol list.
#include "../inventory-items-record/record.hpp"
extern "C" {
extern unsigned char levelName[];
extern unsigned char pad[];
extern int cheatMode;
extern signed char MC_Debug;
extern signed char armorDebug[4];
extern unsigned eRandState;
extern unsigned char WarriorSkillTree[];
extern unsigned char ClericSkillTree[];
extern unsigned char ShadowKnightSkillTree[];
extern unsigned char WizardSkillTree[];
char* strstr(const char* text, const char* pattern);
int sprintf(char* buffer, const char* format, ...);
void givePlayerFeat__Fxii(long long feat, int player, int value);
void playerSetFeatRank__Fixi(int player, long long feat, int rank);
void create__4itemPcPQ24item11descriptionii(char* name, void* description, int a, int b);
int addItemToInventory__FiPQ24item11descriptioni(int player, void* description, int announce);
void equipItem__FiPQ24item11descriptioni(int player, void* slot, int mode);
void givePlayerSkill__Fii(int skill, int player);
void playerSetSkillRank__Fiii(int player, int skill, int rank);
void playerRecomputeWeight__Fi(int player);
}

namespace {
struct Kit {
    int owner;
    unsigned char* record;
    unsigned char description[72];

    // item::create into the template, then addItemToInventory; returns the slot number
    int give(const char* name, int a, int b, int announce = 1) {
        create__4itemPcPQ24item11descriptionii(const_cast<char*>(name), description, a, b);
        return addItemToInventory__FiPQ24item11descriptioni(owner, description, announce);
    }
    // the template byte at +32 is stored between the two calls
    int give_marked(const char* name, int a, int b, int mark) {
        create__4itemPcPQ24item11descriptionii(const_cast<char*>(name), description, a, b);
        description[32] = static_cast<unsigned char>(mark);
        return addItemToInventory__FiPQ24item11descriptioni(owner, description, 1);
    }
    // one create, then the same template added twice (the loop in the image runs its body twice)
    void give_twice(const char* name, int a, int b, int announce) {
        create__4itemPcPQ24item11descriptionii(const_cast<char*>(name), description, a, b);
        for (int round = 0; round < 2; ++round) addItemToInventory__FiPQ24item11descriptioni(owner, description, announce);
    }
    void equip(int list, int slot) {
        equipItem__FiPQ24item11descriptioni(owner, record + 328 + 4616 * list + 72 * slot, 1);
    }
};

// One armor piece of the generated sets: the name is format applied to level.
void armor_piece(Kit& kit, const char* format, int level, int variant, bool equip_it) {
    char name[104];
    sprintf(name, format, level);
    const int slot = kit.give_marked(name, -1, 0, variant);
    if (equip_it) kit.equip(1, slot);
}

void armor_set(Kit& kit, const char* const formats[5], int level, int piece, int variant) {
    const bool equip_it = level == 1 && piece == 0;
    for (int part = 0; part < 5; ++part) armor_piece(kit, formats[part], level, variant, equip_it);
}

const char* const leather_formats[5] = {"leatherarmor%d", "leatherleggings%d", "leatherhelmet%d", "leathergloves%d", "leatherboots%d"};
const char* const chain_formats[5] = {"chainmail%d", "chainleggings%d", "chainhelmet%d", "chaingloves%d", "chainboots%d"};
const char* const plate_formats[5] = {"platemail%d", "plateleggings%d", "platehelmet%d", "plategloves%d", "plateboots%d"};
const char* const cloth1_formats[5] = {"cloth1armor%d", "cloth1leggings%d", "cloth1helmet%d", "cloth1gloves%d", "cloth1boots%d"};
const char* const cloth2_formats[5] = {"cloth2armor%d", "cloth2leggings%d", "cloth2helmet%d", "cloth2gloves%d", "cloth2boots%d"};
const char* const cloth3_formats[5] = {"cloth3armor%d", "cloth3leggings%d", "cloth3helmet%d", "cloth3gloves%d", "cloth3boots%d"};

void give_everything(Kit& kit) {
    unsigned char* record = kit.record;
    kit.give("lightrod", 0, 0);
    kit.give("scepter", 0, 0);
    kit.give("quarterstaff", 0, 0);
    kit.give("battlestaff", 0, 0);
    kit.give("wardstaff", 0, 0);
    kit.give("longbow", 0, 0);
    kit.give("recurvebow", 0, 0);
    kit.give("darkwoodbow", 0, 0);
    kit.give("trueshoebow", 0, 0);
    kit.give("greatbow", 0, 0);
    for (int round = 0; round < 10; ++round) kit.give("gaspotion", -1, 0);
    for (int round = 0; round < 10; ++round) kit.give("firepotion", -1, 0);
    for (int round = 0; round < 10; ++round) kit.give("explodingpotion", -1, 0);
    for (int round = 0; round < 10; ++round) {
        kit.give("squashedfireeye", 0, 0);
        kit.give("piercedfireeye", 0, 0);
        kit.give("fireeye", 0, 0);
        kit.give("enflamedfireeye", 0, 0);
        kit.give("massivefireeye", 0, 0);
        kit.give("bonechip", 0, 0);
        kit.give("venomsac", 0, 0);
        kit.give("poisongland", 0, 0);
        kit.give("mummytongue", 0, 0);
        kit.give("stormshard", 0, 0);
        kit.give("soulgem", 0, 0);
        kit.give("vampirefang", 0, 0);
        kit.give("obscurebullrune", 0, 0);
        kit.give("starglyph", 0, 0);
        kit.give("ghoulflesh", 0, 0);
        kit.give("bloodstone", 0, 0);
        kit.give("regenstone", 0, 0);
        kit.give("meagermanastone", 0, 0);
        kit.give("crackedcarapace", 0, 0);
        kit.give("tinymistyglobe", 0, 0);
        kit.give("calmedmistyglobe", 0, 0);
        kit.give("whetstone", 0, 0);
    }
    kit.give("shortbow", 0, 0);
    kit.give("arrows", 0, 0);
    kit.give("arrows", 0, 0);
    kit.give("arrows", 0, 0);
    kit.give("dagger+requiredslashing10", 0, 0);
    kit.give("dagger+strengthbonus10+firedamage26-46", 1, 0);
    kit.give("dagger+colddamage30-50", 1, 0);
    kit.give("shortsword+shockdamage25-45", 0, 0);
    kit.give("shortsword+bugzapper40-60", 1, 0);
    kit.give("shortsword+bugexploder50", 2, 0);
    kit.give("shortsword+undeadbonus200", 0, 0);
    kit.give("shortsword+undeaddestroy75", 0, 0);
    kit.give("broadsword+stunresistance100", 0, 0);
    int slot = kit.give("broadsword+stunresistance50", 1, 0);
    if (*reinterpret_cast<int*>(record + 84) != 5) kit.equip(0, slot);
    kit.give("broadsword+rusty+fireburst50,20-40", -1, 0);
    kit.give("broadsword+castiron+frostburst25,40-80", 1, 0);
    kit.give("longsword+poisonburst100,0", 0, 0);
    kit.give("longsword+lightningburst25,25-50", 1, 0);
    kit.give("longsword+poisonburst100,1", 1, 0);
    kit.give("longsword+poisonburst100,2", 0, 0);
    kit.give_marked("spear", 0, 0, 3);
    kit.give("spear+poisonburst100,2", 0, 0);
    kit.give("bastardsword+poisonburst100,3", 0, 0);
    kit.give("bastardsword+damageincrease200", 1, 0);
    kit.give("bastardsword+healthregeneration100", 0, 0);
    kit.give("bastardsword+manaregeneration25", 1, 0);
    kit.give("greatsword+attackspeed-2", 0, 0);
    kit.give("greatsword", 0, 0);
    kit.give("flamberge+diseasedamage10-20", 1, 0);
    kit.give("cleaver", 0, 0);
    kit.give("handaxe", 0, 0);
    kit.give("handaxe", 0, 0);
    kit.give("handaxe", 0, 0);
    kit.give("battleaxe+resistmagic19+resistdisease11", 0, 0);
    kit.give("greataxe", 0, 0);
    kit.give("halberd", 0, 0);
    kit.give("spear", 0, 0);
    kit.give("club", 0, 0);
    slot = kit.give("mace", 0, 0);
    if (*reinterpret_cast<int*>(record + 84) == 5) kit.equip(0, slot);
    kit.give("pick", 0, 0);
    kit.give("staff", 0, 0);
    kit.give("staff", 1, 0);
    kit.give("greatstaff", 0, 0);
    kit.give("hammer", 0, 0);
    kit.give("morningstar", 0, 0);
    kit.give("dblmace", 0, 0);
    kit.give("warhammer", 0, 0);
    kit.give("rapier", 0, 0);
    kit.give("scimitar", 0, 0);
    kit.give("falchion", 0, 0);
    kit.give("claymore", 0, 0);
    kit.give("rod", 0, 0);
    kit.give("rod", 1, 0);
    kit.give("rod", 2, 0);
    for (int round = 0; round < 6; ++round) kit.give("ring+resistfire10", round, 0);
    for (int round = 0; round < 8; ++round) kit.give("earring+resistfire20", round, 0);
    for (int round = 0; round < 4; ++round) kit.give("choker+resistfire35", round, 0);
    for (int round = 0; round < 3; ++round) kit.give("amulet+resistfire60", round, 0);
    if (armorDebug[0] == 0 && armorDebug[1] == 0 && armorDebug[2] == 0) {
        eRandState = eRandState * 1664525u + 1013904223u;
        armorDebug[(eRandState >> 16) % 3u] = 1;
    }
    if (armorDebug[3] == 0) {
        for (int level = 1; level < 5; ++level) {
            for (int piece = 0; piece < 5; ++piece) {
                const int variant = piece == 4 ? 56 : (piece == 3 ? 8 : piece);
                if (*reinterpret_cast<int*>(record + 80) == 4) {
                    if (armorDebug[0] != 0) armor_set(kit, cloth1_formats, level, piece, variant);
                    if (armorDebug[1] != 0) armor_set(kit, cloth2_formats, level, piece, variant);
                    if (armorDebug[2] != 0) armor_set(kit, cloth3_formats, level, piece, variant);
                } else {
                    if (armorDebug[0] != 0) armor_set(kit, leather_formats, level, piece, variant);
                    if (armorDebug[1] != 0) armor_set(kit, chain_formats, level, piece, variant);
                    if (armorDebug[2] != 0) armor_set(kit, plate_formats, level, piece, variant);
                }
            }
        }
    }
    if (armorDebug[3] != 0) {
        kit.give("shield1", 0, 0);
        kit.give("shield2", 1, 0);
        kit.give("shield3", 2, 0);
        kit.give("donkeyshield1", 0, 0);
        kit.give("donkeyshield2", 1, 0);
        kit.give("donkeyshield3", 2, 0);
        kit.give("cshield1", 0, 0);
        kit.give("cshield2", 1, 0);
        kit.give("cshield3", 2, 0);
        kit.give("dshield1", 0, 0);
        kit.give("dshield2", 1, 0);
        kit.give("dshield3", 2, 0);
        kit.give("eshield1", 0, 0);
        kit.give("eshield2", 1, 0);
        kit.give("eshield3", 2, 0);
        kit.give("fshield1", 0, 0);
        kit.give("fshield2", 1, 0);
        kit.give("fshield3", 2, 0);
        kit.give("gshield1", 0, 0);
        kit.give("gshield2", 1, 0);
        kit.give("gshield3", 2, 0);
        kit.give("hshield1", 0, 0);
        kit.give("hshield2", 1, 0);
        kit.give("hshield3", 2, 0);
        kit.give("ishield1", 0, 0);
        kit.give("ishield2", 1, 0);
        kit.give("ishield3", 2, 0);
        kit.give("jshield1", 0, 0);
        kit.give("jshield2", 1, 0);
        kit.give("jshield3", 2, 0);
        kit.give("kshield1", 0, 0);
        kit.give("kshield2", 1, 0);
        kit.give("kshield3", 2, 0);
        kit.give("lshield1", 0, 0);
        kit.give("lshield2", 1, 0);
        kit.give("lshield3", 2, 0);
        kit.give("mshield1", 0, 0);
        kit.give("mshield2", 1, 0);
        kit.give("mshield3", 2, 0);
        kit.give("oshield1", 0, 0);
        kit.give("oshield2", 1, 0);
        kit.give("oshield3", 2, 0);
        kit.give("pshield1", 0, 0);
        kit.give("pshield2", 1, 0);
        kit.give("pshield3", 2, 0);
        kit.give("rshield1", 0, 0);
        kit.give("rshield2", 1, 0);
        kit.give("rshield3", 2, 0);
    }
}

void starting_kit(Kit& kit) {
    unsigned char* record = kit.record;
    const int kind = *reinterpret_cast<int*>(record + 80);
    const char* armor_names[4];
    int mark;
    if (kind == 4) {
        mark = 7;
        armor_names[0] = "cloth1armor1+armorbonus-25+requiredlevel1";
        armor_names[1] = "cloth1boots1+armorbonus-25+requiredlevel1";
        armor_names[2] = "cloth1leggings1+armorbonus-25+requiredlevel1";
        armor_names[3] = "cloth1gloves1+armorbonus-25+requiredlevel1";
    } else {
        mark = 2;
        armor_names[0] = "leatherarmor1+armorbonus-55+requiredlevel1";
        armor_names[1] = "leatherboots1+armorbonus-55+requiredlevel1";
        armor_names[2] = "leatherleggings1+armorbonus-55+requiredlevel1";
        armor_names[3] = "leathergloves1+armorbonus-55+requiredlevel1";
    }
    for (int part = 0; part < 4; ++part) kit.equip(1, kit.give_marked(armor_names[part], 0, 0, mark));
    // weapons by class; 'mark' below is the template byte at +32
    const int weapon_kind = *reinterpret_cast<int*>(record + 80);
    if (weapon_kind == 0) {
        kit.equip(0, kit.give_marked("axe+damageincrease-25+requiredlevel1", 0, 0, 7));
    } else if (weapon_kind == 3) {
        kit.equip(0, kit.give_marked("shortsword+damageincrease-45+requiredlevel1", 1, 0, 4));
        kit.equip(0, kit.give_marked("shortbow+damageincrease-10+requiredlevel1", 0, 0, 9));
        kit.give("arrows", 0, 0);
        kit.give("arrows", 0, 0);
    } else if (weapon_kind == 1) {
        kit.equip(0, kit.give_marked("mace+damageincrease-25+requiredlevel1", 0, 0, 7));
    } else if (weapon_kind == 2) {
        kit.equip(0, kit.give_marked("shortsword+damageincrease-35+requiredlevel1", 0, 0, 5));
    } else if (weapon_kind == 4) {
        kit.equip(0, kit.give_marked("quarterstaff+damageincrease-35+requiredlevel1", 0, 0, 5));
    }
    kit.give_twice("recallpotion", -1, 0, 0);
    kit.give_twice("leshealingpotion", -1, 0, 1);
    kit.give_twice("lesrejuvpotion", -1, 0, 1);
}

}

extern "C" void setupDefaultEquipment__Fi(int owner) {
    using namespace inventory_items;
    Kit kit;
    kit.owner = owner;
    kit.record = player_record(owner);
    unsigned char* record = kit.record;
    unsigned char* description = kit.description;
    for (int pair = 3; pair >= 0; --pair) *reinterpret_cast<short*>(description + 36 + 8 * pair) = -1;
    description[33] = 0xff;
    description[32] = 0xff;
    for (int offset = 16; offset < 24; offset += 2) {
        *reinterpret_cast<short*>(description + offset) = 0;
        *reinterpret_cast<short*>(description + 8 + offset) = 0;
    }
    const bool skirmish = strstr(reinterpret_cast<const char*>(levelName + 81), "skirm") != 0;
    unsigned char* pad_record = pad + 88 * owner;
    unsigned flags = *reinterpret_cast<unsigned*>(pad_record + 8);
    if (flags & 2u) {
        cheatMode = 1;
        const unsigned char* tree = 0;
        const int kind = *reinterpret_cast<int*>(record + 80);
        if (kind == 1) tree = ClericSkillTree;
        else if (kind == 0) tree = WarriorSkillTree;
        else if (kind == 2) tree = ShadowKnightSkillTree;
        else if (kind == 4) tree = WizardSkillTree;
        if (tree != 0) {
            int index = 0;
            for (;;) {
                const unsigned char* entry = tree + 56 * index;
                ++index;
                if (strstr(reinterpret_cast<const char*>(entry), "END") != 0) break;
                const long long feat = *reinterpret_cast<const long long*>(entry + 40);
                givePlayerFeat__Fxii(feat, owner, -1);
                playerSetFeatRank__Fixi(owner, feat, 1);
            }
            flags = *reinterpret_cast<unsigned*>(pad_record + 8);
        }
    }
    const bool everything = (flags & 2u) != 0 && MC_Debug != 0;
    if (everything) {
        give_everything(kit);
    } else if (!skirmish) {
        starting_kit(kit);
    }
    const int kind = *reinterpret_cast<int*>(record + 80);
    if (kind == 0 || kind == 3 || kind == 2) {
        givePlayerSkill__Fii(60, owner);
        playerSetSkillRank__Fiii(owner, 60, 1);
        givePlayerSkill__Fii(61, owner);
        playerSetSkillRank__Fiii(owner, 61, 1);
    } else if (kind == 1) {
        givePlayerSkill__Fii(60, owner);
        playerSetSkillRank__Fiii(owner, 60, 1);
        givePlayerSkill__Fii(16, owner);
        playerSetSkillRank__Fiii(owner, 16, 1);
    } else if (kind == 4) {
        givePlayerSkill__Fii(60, owner);
        playerSetSkillRank__Fiii(owner, 60, 1);
        givePlayerSkill__Fii(23, owner);
        playerSetSkillRank__Fiii(owner, 23, 1);
    }
    playerRecomputeWeight__Fi(owner);
}
