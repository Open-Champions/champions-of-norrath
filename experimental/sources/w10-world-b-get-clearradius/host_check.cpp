// Host contract for getClearradius__FPCc. Not an EE plan input. Build: c++ -std=c++17 host_check.cpp
// strcasecmp and strstr are small independent stand-ins. _ctype_ is a 257-byte table (below) that marks
// ASCII capitals with bit 0 at index (character + 1) like the real one; characters with the
// high bit set are not exercised here (the paired run against the image covers them).
#include <cstdio>
#include <cstring>
#include <string>
extern "C" int strcasecmp(const char* a, const char* b) {
    for (;; ++a, ++b) {
        int x = static_cast<unsigned char>(*a), y = static_cast<unsigned char>(*b);
        if (x >= 'A' && x <= 'Z') x += 32;
        if (y >= 'A' && y <= 'Z') y += 32;
        if (x != y) return x < y ? -1 : 1;
        if (x == 0) return 0;
    }
}
extern "C" char* strstr(const char* h, const char* n) {
    const size_t len = std::strlen(n);
    for (; *h; ++h) if (std::strncmp(h, n, len) == 0) return const_cast<char*>(h);
    return len == 0 ? const_cast<char*>(h) : nullptr;
}
#include "get_clearradius.cpp"
extern const unsigned char _ctype_[257] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
struct Row { const char* name; int value; bool contains; };
static const Row rows[] = {
    {"barrel", 16, false},
    {"goblinrecruit", 18, false},
    {"goblinsoldier", 18, false},
    {"goblinleader", 18, false},
    {"goblinarcher", 18, false},
    {"goblinshaman", 18, false},
    {"goblin", 18, false},
    {"orc", 18, false},
    {"orcrecruit", 18, false},
    {"orcsoldier", 18, false},
    {"orcleader", 18, false},
    {"orcarcher", 18, false},
    {"orcshaman", 18, false},
    {"powderkeg", 11, false},
    {"woodelfarcher", 20, false},
    {"woodelfspear", 20, false},
    {"woodelfsoldier", 20, false},
    {"firebeetle", 14, false},
    {"npc", 20, false},
    {"chest", 22, false},
    {"chestcav", 22, false},
    {"gold", 24, false},
    {"blackwidow", 15, false},
    {"chestfuneral", 22, false},
    {"ghoul", 18, false},
    {"badger", 35, false},
    {"boxafay", 24, false},
    {"boxbfay", 22, false},
    {"vaseweb", 12, false},
    {"gothpot1", 20, false},
    {"gothpot2", 17, false},
    {"vampire", 18, false},
    {"darkelf", 15, true},
    {"froglock", 18, false},
    {"seamonster", 35, false},
    {"dt_pota", 28, false},
    {"dt_potb", 15, false},
    {"dt_chest", 12, false},
    {"mummydso", 15, false},
    {"mummydsh", 15, false},
    {"mummyeso", 15, false},
    {"mummyesh", 15, false},
    {"mummytso", 15, false},
    {"mummytsh", 15, false},
    {"mummyelitesoldier", 15, false},
    {"scorpion", 30, false},
    {"baskcold", 15, false},
    {"lavamonster", 30, false},
    {"lbeetle", 14, false},
    {"ghulblue", 18, false},
    {"lavakeg", 18, false},
    {"cavchest", 12, false},
    {"demon", 15, false},
    {"watskel", 18, false},
    {"schest", 12, false},
    {"ulthork", 35, false},
    {"coldbox", 12, false},
    {"snobadge", 25, false},
    {"ocechest", 12, false},
    {"cyclops", 35, false},
    {"cyclopsgrey", 35, false},
    {"greenpot", 15, false},
    {"bbeetle", 14, false},
    {"ant", 20, false},
    {"gobchest", 12, false},
    {"faychest", 12, false},
    {"weaprack", 25, false},
    {"cauldron", 12, false},
    {"lavakeg", 18, false},
    {"fish1", 0, false},
    {"fish2", 0, false},
    {"fish3", 0, false},
    {"deathweb", 12, false},
    {"venchest", 12, false},
    {"cthulu", 20, false},
    {"giachest", 35, false}
};
static std::string flip(std::string s) {
    for (auto& c : s) c = (c >= 'a' && c <= 'z') ? char(c - 32) : (c >= 'A' && c <= 'Z') ? char(c + 32) : c;
    return s;
}
// Independent model of the chain: rows in order, a case-sensitive text search for the one
// "contains" row, and the case-insensitive prefix test after the row named "vaseweb".
static int model(const std::string& name) {
    const int n = sizeof rows / sizeof rows[0];
    for (int i = 0; i != n; ++i) {
        bool hit = rows[i].contains ? name.find(rows[i].name) != std::string::npos
                                    : strcasecmp(name.c_str(), rows[i].name) == 0;
        if (hit) return rows[i].value;
        if (std::string(rows[i].name) == "vaseweb" && name.size() >= 8 && strcasecmp(name.substr(0, 8).c_str(), "skeleton") == 0) return 18;
    }
    return 0;
}
int main() {
    const int n = sizeof rows / sizeof rows[0];
    if (n != 76) return 1;
    for (int i = 0; i != n; ++i) {
        const std::string name = rows[i].name;
        const std::string variants[] = {name, flip(name), name + "x", name.substr(0, name.size() - 1), "x" + name, name + " " + name};
        for (const std::string& v : variants)
            if (getClearradius__FPCc(v.c_str()) != model(v)) { std::printf("row %d '%s'\n", i, v.c_str()); return 2; }
        // The exact names answer their own value unless an earlier row also matches them.
        if (!rows[i].contains && getClearradius__FPCc(name.c_str()) != model(name)) return 3;
    }
    // The text search for "darkelf" works anywhere in the name and is case sensitive; the prefix test
    // only at the start and is not.
    if (getClearradius__FPCc("a_darkelf_b") != 15 || getClearradius__FPCc("DARKELF") != 0) return 5;
    if (getClearradius__FPCc("skeleton") != 18 || getClearradius__FPCc("SkeletonKing") != 18 || getClearradius__FPCc("skeleto") != 0) return 6;
    if (getClearradius__FPCc("the skeleton") != 0 || getClearradius__FPCc("") != 0) return 7;
    // Spot values from the table: first row, a middle row and the last row's match/mismatch.
    if (getClearradius__FPCc("barrel") != 16 || getClearradius__FPCc("goblin") != 18 || getClearradius__FPCc("giachest") != 35) return 8;
    if (getClearradius__FPCc("fish2") != 0 || getClearradius__FPCc("giachest ") != 0) return 9;
    std::puts("getClearradius check passed");
    return 0;
}
