// Host check for act5. Not an EE plan input. Compares record count and each record against a
// separately written comma list of "name:number" items.
#include "act5.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static const char* const expected =
    "airhub:35000,hate1:35000,hate3:35000,hate4:35000,gameComplete:35000";

int main() {
    if (sizeof(act5) / sizeof(act5[0]) != 5) return 1;
    std::string rebuilt;
    for (const LevelRecord& r : act5) {
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
