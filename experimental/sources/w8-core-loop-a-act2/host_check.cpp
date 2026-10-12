// Host check for act2. Not an EE plan input. Compares record count and each record against a
// separately written comma list of "name:number" items.
#include "act2.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static const char* const expected =
    "gnomehub:30000,anthill1:35000,anthill2:35000,lava1:35000,lava2:35000,battle1:35000,battle3:35000,gothic1:35000,gothic2:42000,nas:35000";

int main() {
    if (sizeof(act2) / sizeof(act2[0]) != 10) return 1;
    std::string rebuilt;
    for (const LevelRecord& r : act2) {
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
