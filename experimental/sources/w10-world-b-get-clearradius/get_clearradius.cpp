// Observed getClearradius__FPCc at 0x00269fc0, 1908 bytes in SLUS_205.65. Argument: an
// object or model name. Returns a small integer chosen by an ordered chain of name tests, the
// first match winning; no match returns 0. Every test is a case-insensitive whole-string
// comparison with strcasecmp, except: the name "darkelf" is searched for anywhere in the
// name with strstr (a hit gives 15), and one test, placed after "vaseweb" and before
// "gothpot1", accepts any name that begins with "skeleton" (compared case-insensitively over
// the literal's length through the character-type table _ctype_, indexed by the signed
// character) and gives 18. The last test is "giachest", which answers 35 on a match and 0 on
// a mismatch. The chain has 76 calls (75 strcasecmp and one strstr) plus the prefix test, in the order written below;
// the values were read from the image's flow by forcing each comparison to match in turn,
// and the order by following the compare calls. The names "fish1", "fish2" and "fish3" are
// tested and answer 0, the same as no match.
// Inferred: the value is the radius, in some game unit, of the free space an object of that
// kind needs ("clear radius", from the function name) and the names are actor/model names.
// Unknown: the unit and who uses the value.
extern "C" int strcasecmp(const char* left, const char* right);
extern "C" char* strstr(const char* haystack, const char* needle);
extern const unsigned char _ctype_[];

namespace {
// Case-insensitive prefix test as the image does it: for each character of the literal, both
// characters are lower-cased by adding 32 when the character-type table marks them as upper
// case (bit 0), the table being read at index (character + 1) with the signed character value.
bool starts_with_ignoring_case(const char* name, const char* literal) {
    for (; *literal != 0; ++literal, ++name) {
        const int a = *name;
        const int b = *literal;
        const int la = (_ctype_[1 + a] & 1) != 0 ? a + 32 : a;
        const int lb = (_ctype_[1 + b] & 1) != 0 ? b + 32 : b;
        if (la != lb) return false;
    }
    return true;
}
}

extern "C" int getClearradius__FPCc(const char* name) {
    if (strcasecmp(name, "barrel") == 0) return 16;
    if (strcasecmp(name, "goblinrecruit") == 0) return 18;
    if (strcasecmp(name, "goblinsoldier") == 0) return 18;
    if (strcasecmp(name, "goblinleader") == 0) return 18;
    if (strcasecmp(name, "goblinarcher") == 0) return 18;
    if (strcasecmp(name, "goblinshaman") == 0) return 18;
    if (strcasecmp(name, "goblin") == 0) return 18;
    if (strcasecmp(name, "orc") == 0) return 18;
    if (strcasecmp(name, "orcrecruit") == 0) return 18;
    if (strcasecmp(name, "orcsoldier") == 0) return 18;
    if (strcasecmp(name, "orcleader") == 0) return 18;
    if (strcasecmp(name, "orcarcher") == 0) return 18;
    if (strcasecmp(name, "orcshaman") == 0) return 18;
    if (strcasecmp(name, "powderkeg") == 0) return 11;
    if (strcasecmp(name, "woodelfarcher") == 0) return 20;
    if (strcasecmp(name, "woodelfspear") == 0) return 20;
    if (strcasecmp(name, "woodelfsoldier") == 0) return 20;
    if (strcasecmp(name, "firebeetle") == 0) return 14;
    if (strcasecmp(name, "npc") == 0) return 20;
    if (strcasecmp(name, "chest") == 0) return 22;
    if (strcasecmp(name, "chestcav") == 0) return 22;
    if (strcasecmp(name, "gold") == 0) return 24;
    if (strcasecmp(name, "blackwidow") == 0) return 15;
    if (strcasecmp(name, "chestfuneral") == 0) return 22;
    if (strcasecmp(name, "ghoul") == 0) return 18;
    if (strcasecmp(name, "badger") == 0) return 35;
    if (strcasecmp(name, "boxafay") == 0) return 24;
    if (strcasecmp(name, "boxbfay") == 0) return 22;
    if (strcasecmp(name, "vaseweb") == 0) return 12;
    if (starts_with_ignoring_case(name, "skeleton")) return 18;
    if (strcasecmp(name, "gothpot1") == 0) return 20;
    if (strcasecmp(name, "gothpot2") == 0) return 17;
    if (strcasecmp(name, "vampire") == 0) return 18;
    if (strstr(name, "darkelf") != 0) return 15;
    if (strcasecmp(name, "froglock") == 0) return 18;
    if (strcasecmp(name, "seamonster") == 0) return 35;
    if (strcasecmp(name, "dt_pota") == 0) return 28;
    if (strcasecmp(name, "dt_potb") == 0) return 15;
    if (strcasecmp(name, "dt_chest") == 0) return 12;
    if (strcasecmp(name, "mummydso") == 0) return 15;
    if (strcasecmp(name, "mummydsh") == 0) return 15;
    if (strcasecmp(name, "mummyeso") == 0) return 15;
    if (strcasecmp(name, "mummyesh") == 0) return 15;
    if (strcasecmp(name, "mummytso") == 0) return 15;
    if (strcasecmp(name, "mummytsh") == 0) return 15;
    if (strcasecmp(name, "mummyelitesoldier") == 0) return 15;
    if (strcasecmp(name, "scorpion") == 0) return 30;
    if (strcasecmp(name, "baskcold") == 0) return 15;
    if (strcasecmp(name, "lavamonster") == 0) return 30;
    if (strcasecmp(name, "lbeetle") == 0) return 14;
    if (strcasecmp(name, "ghulblue") == 0) return 18;
    if (strcasecmp(name, "lavakeg") == 0) return 18;
    if (strcasecmp(name, "cavchest") == 0) return 12;
    if (strcasecmp(name, "demon") == 0) return 15;
    if (strcasecmp(name, "watskel") == 0) return 18;
    if (strcasecmp(name, "schest") == 0) return 12;
    if (strcasecmp(name, "ulthork") == 0) return 35;
    if (strcasecmp(name, "coldbox") == 0) return 12;
    if (strcasecmp(name, "snobadge") == 0) return 25;
    if (strcasecmp(name, "ocechest") == 0) return 12;
    if (strcasecmp(name, "cyclops") == 0) return 35;
    if (strcasecmp(name, "cyclopsgrey") == 0) return 35;
    if (strcasecmp(name, "greenpot") == 0) return 15;
    if (strcasecmp(name, "bbeetle") == 0) return 14;
    if (strcasecmp(name, "ant") == 0) return 20;
    if (strcasecmp(name, "gobchest") == 0) return 12;
    if (strcasecmp(name, "faychest") == 0) return 12;
    if (strcasecmp(name, "weaprack") == 0) return 25;
    if (strcasecmp(name, "cauldron") == 0) return 12;
    if (strcasecmp(name, "lavakeg") == 0) return 18;
    if (strcasecmp(name, "fish1") == 0) return 0;
    if (strcasecmp(name, "fish2") == 0) return 0;
    if (strcasecmp(name, "fish3") == 0) return 0;
    if (strcasecmp(name, "deathweb") == 0) return 12;
    if (strcasecmp(name, "venchest") == 0) return 12;
    if (strcasecmp(name, "cthulu") == 0) return 20;
    return strcasecmp(name, "giachest") == 0 ? 35 : 0;
}
