// Host check for act4. Not an EE plan input. Compares record count and each record against a
// separately written comma list of "name:number" items.
#include "act4.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static const char* const expected =
    "elfhub:45000,elfhubn:45000,khathuun:45000,khathuun_katone:45000,khathuun_kattwo:45000,khathuun_katnn:45000,khathuun_katne:45000,arena1:45000,garden:45000";

int main() {
    if (sizeof(act4) / sizeof(act4[0]) != 9) return 1;
    std::string rebuilt;
    for (const LevelRecord& r : act4) {
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
