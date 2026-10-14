// Host check for act1. Not an EE plan input. Compares record count and each record against a
// separately written comma list of "name:number" items.
#include "act1.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static const char* const expected =
    "kelethin:30000,faydark1:34000,faydark2:34000,faydark3:34000,goblin1a_0:12000,goblin1a_2:12000,goblin1a_4:12000,goblin1a_6:12000,goblin1_f0:12000,goblin1_f1:12000,goblin1_f2:12000,goblin2_0:11000,goblin2_1:11000,goblin2_2:11000,goblin2_3:11000,goblin2_4:12000,goblin2_6:12000,orccave1:34000,orccave2:34000,orccave3:34000,ruinint1:24000,ruinint2:24000";

int main() {
    if (sizeof(act1) / sizeof(act1[0]) != 22) return 1;
    std::string rebuilt;
    for (const LevelRecord& r : act1) {
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
