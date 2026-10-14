// Host check for act3. Not an EE plan input. Compares record count and each record against a
// separately written comma list of "name:number" items.
#include "act3.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static const char* const expected =
    "beachhub:25000,island2:25000,kerriel:25000,islcaves:25000,islcaves_rondo:25000,islcaves_kerriel:25000,islcaves_decave1:25000,islcaves_decave2:25000,oceanc1:25000,oceanc2:25000,ocefloor:25000,des1:40000,des2:40000,grass:25000,desert:25000,bronzeht:25000,frozesea:20000,cloudt:20000";

int main() {
    if (sizeof(act3) / sizeof(act3[0]) != 18) return 1;
    std::string rebuilt;
    for (const LevelRecord& r : act3) {
        if (!r.name || !r.name[0]) return 2;
        if (!rebuilt.empty()) rebuilt += ",";
        rebuilt += r.name;
        rebuilt += ":" + std::to_string(r.number);
    }
    if (rebuilt != expected) {
        std::printf("%s\n", rebuilt.c_str());
        return 3;
    }
    return 0;
}
