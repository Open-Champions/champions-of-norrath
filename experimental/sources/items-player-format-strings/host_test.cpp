#include <cstdio>
#include <cstring>
extern "C" {
extern const char norrath_item_format_454428[];
extern const char norrath_item_format_454440[];
extern const char norrath_item_format_454458[];
extern const char norrath_item_format_454468[];
extern const char norrath_item_format_454478[];
extern const char norrath_item_format_454488[];
extern const char norrath_item_format_454498[];
extern const char norrath_item_format_4544a8[];
extern const char norrath_item_format_4544b8[];
extern const char norrath_item_format_4544c8[];
extern const char norrath_item_format_4544e0[];
extern const char norrath_item_format_4544f8[];
extern const char norrath_item_format_454508[];
extern const char norrath_item_format_454518[];
extern const char norrath_item_format_454530[];
extern const char norrath_item_format_454548[];
extern const char norrath_item_format_454560[];
extern const char norrath_item_format_454578[];
extern const char norrath_item_format_454588[];
extern const char norrath_item_format_454598[];
extern const char norrath_item_format_4545b0[];
extern const char norrath_item_format_4545c8[];
extern const char norrath_item_format_4545d8[];
extern const char norrath_item_format_4545e8[];
extern const char norrath_item_format_4545f8[];
extern const char norrath_item_format_454610[];
extern const char norrath_item_format_454628[];
extern const char norrath_item_format_454638[];
extern const char norrath_item_format_454648[];
extern const char norrath_item_format_454658[];
extern const char norrath_item_format_454668[];
extern const char norrath_item_format_454678[];
extern const char norrath_item_format_454688[];
extern const char norrath_item_format_4546a0[];
extern const char norrath_item_format_4546b8[];
extern const char norrath_item_format_4546d0[];
extern const char norrath_item_format_4546e8[];
extern const char norrath_item_format_454700[];
extern const char norrath_item_format_454710[];
extern const char norrath_item_format_454720[];
extern const char norrath_item_format_454738[];
extern const char norrath_item_format_483260[];
}
int main() {
    if (std::strcmp(norrath_item_format_454428, "damageincrease%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("damageincrease1", norrath_item_format_454428, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454440, "stunresistance%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("stunresistance1", norrath_item_format_454440, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454458, "critical%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("critical1", norrath_item_format_454458, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454468, "undeaddestroy%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("undeaddestroy1", norrath_item_format_454468, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454478, "undeadbonus%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("undeadbonus1", norrath_item_format_454478, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454488, "bugexploder%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("bugexploder1", norrath_item_format_454488, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454498, "bugzapper%d-%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("bugzapper1-2", norrath_item_format_454498, &value[0], &value[1]) != 2) return 2;
        if (value[0] != 1) return 3;
        if (value[1] != 2) return 3;
    }
    if (std::strcmp(norrath_item_format_4544a8, "firedamage%d-%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("firedamage1-2", norrath_item_format_4544a8, &value[0], &value[1]) != 2) return 2;
        if (value[0] != 1) return 3;
        if (value[1] != 2) return 3;
    }
    if (std::strcmp(norrath_item_format_4544b8, "colddamage%d-%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("colddamage1-2", norrath_item_format_4544b8, &value[0], &value[1]) != 2) return 2;
        if (value[0] != 1) return 3;
        if (value[1] != 2) return 3;
    }
    if (std::strcmp(norrath_item_format_4544c8, "shockdamage%d-%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("shockdamage1-2", norrath_item_format_4544c8, &value[0], &value[1]) != 2) return 2;
        if (value[0] != 1) return 3;
        if (value[1] != 2) return 3;
    }
    if (std::strcmp(norrath_item_format_4544e0, "poisondamage%d-%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("poisondamage1-2", norrath_item_format_4544e0, &value[0], &value[1]) != 2) return 2;
        if (value[0] != 1) return 3;
        if (value[1] != 2) return 3;
    }
    if (std::strcmp(norrath_item_format_4544f8, "attackspeed%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("attackspeed1", norrath_item_format_4544f8, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454508, "requiredlevel%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("requiredlevel1", norrath_item_format_454508, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454518, "requiredstrength%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("requiredstrength1", norrath_item_format_454518, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454530, "requiredintelligence%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("requiredintelligence1", norrath_item_format_454530, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454548, "requireddexterity%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("requireddexterity1", norrath_item_format_454548, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454560, "requiredstamina%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("requiredstamina1", norrath_item_format_454560, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454578, "requiredclass%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("requiredclass1", norrath_item_format_454578, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454588, "armorbonus%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("armorbonus1", norrath_item_format_454588, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454598, "healthregeneration%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("healthregeneration1", norrath_item_format_454598, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_4545b0, "manaregeneration%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("manaregeneration1", norrath_item_format_4545b0, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_4545c8, "healthbonus%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("healthbonus1", norrath_item_format_4545c8, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_4545d8, "manabonus%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("manabonus1", norrath_item_format_4545d8, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_4545e8, "strengthbonus%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("strengthbonus1", norrath_item_format_4545e8, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_4545f8, "intelligencebonus%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("intelligencebonus1", norrath_item_format_4545f8, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454610, "dexteritybonus%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("dexteritybonus1", norrath_item_format_454610, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454628, "staminabonus%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("staminabonus1", norrath_item_format_454628, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454638, "resistfire%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("resistfire1", norrath_item_format_454638, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454648, "resistcold%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("resistcold1", norrath_item_format_454648, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454658, "resistpoison%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("resistpoison1", norrath_item_format_454658, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454668, "resistdisease%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("resistdisease1", norrath_item_format_454668, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454678, "resistmagic%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("resistmagic1", norrath_item_format_454678, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454688, "poisonburst%d,%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("poisonburst1,2", norrath_item_format_454688, &value[0], &value[1]) != 2) return 2;
        if (value[0] != 1) return 3;
        if (value[1] != 2) return 3;
    }
    if (std::strcmp(norrath_item_format_4546a0, "fireburst%d,%d-%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("fireburst1,2-3", norrath_item_format_4546a0, &value[0], &value[1], &value[2]) != 3) return 2;
        if (value[0] != 1) return 3;
        if (value[1] != 2) return 3;
        if (value[2] != 3) return 3;
    }
    if (std::strcmp(norrath_item_format_4546b8, "frostburst%d,%d-%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("frostburst1,2-3", norrath_item_format_4546b8, &value[0], &value[1], &value[2]) != 3) return 2;
        if (value[0] != 1) return 3;
        if (value[1] != 2) return 3;
        if (value[2] != 3) return 3;
    }
    if (std::strcmp(norrath_item_format_4546d0, "lightningburst%d,%d-%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("lightningburst1,2-3", norrath_item_format_4546d0, &value[0], &value[1], &value[2]) != 3) return 2;
        if (value[0] != 1) return 3;
        if (value[1] != 2) return 3;
        if (value[2] != 3) return 3;
    }
    if (std::strcmp(norrath_item_format_4546e8, "weightmodifier%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("weightmodifier1", norrath_item_format_4546e8, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454700, "costmodifier%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("costmodifier1", norrath_item_format_454700, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454710, "requiredblunt%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("requiredblunt1", norrath_item_format_454710, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454720, "requiredslashing%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("requiredslashing1", norrath_item_format_454720, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454738, "diseasedamage%d-%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("diseasedamage1-2", norrath_item_format_454738, &value[0], &value[1]) != 2) return 2;
        if (value[0] != 1) return 3;
        if (value[1] != 2) return 3;
    }
    if (std::strcmp(norrath_item_format_483260, "mesh%d") != 0) return 1;
    {
        int value[4] = {};
        if (std::sscanf("mesh1", norrath_item_format_483260, &value[0]) != 1) return 2;
        if (value[0] != 1) return 3;
    }
    if (std::strcmp(norrath_item_format_454428, "damageincrease%d") != 0) return 4;
    if (std::strcmp(norrath_item_format_4546a0, "fireburst%d,%d-%d") != 0) return 5;
    if (std::strcmp(norrath_item_format_483260, "mesh%d") != 0) return 6;
    return 0;
}
