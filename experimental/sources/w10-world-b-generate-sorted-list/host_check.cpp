// Host contract for generateSortedListAndInfoHeaders__Fv. Not an EE plan input.
// Build: c++ -std=c++17 host_check.cpp
// moreRecent is replaced by a table of ages (a record's byte 0 is its age; smaller is newer
// is not assumed - the mock answers "left newer than right" when left's age is larger).
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>
#include "generate_sorted_list.cpp"
extern "C" {
DirRecord MCA_DirContents[64]; DirRecord sortedDir[10]; SlotHeader slotInfoHeaders[10]; PlayerRace sortedPlayerRace[10];
int MCA_LastDirCount; int sortedDirCount;
}
static std::string events;
static int more_calls;
extern "C" int moreRecent__Fii(int l, int r) { ++more_calls; return MCA_DirContents[l].bytes[0] > MCA_DirContents[r].bytes[0]; }
extern "C" int MCA_Cmd__Fiiii(int c, const void* a, int s, int e) {
    events += "M" + std::to_string(c) + "," + std::to_string(s) + "," + std::to_string(e) + ",";
    if (c == 0) events += std::to_string(reinterpret_cast<const unsigned char*>(a) - sortedDir[0].bytes) + ";";
    else if (c == 3) events += std::to_string(static_cast<const SlotHeader*>(a) - slotInfoHeaders) + ";";
    else events += (a ? "x;" : "0;");
    return 0;
}
extern "C" void loadPlayerInfo__FiPiP12playerStruct(int c, int* out, void* p) {
    events += "L" + std::to_string(c) + "," + std::to_string(reinterpret_cast<PlayerRace*>(out) - sortedPlayerRace) + "," + (p ? "p" : "0") + ";";
}
int main() {
    std::srand(11);
    for (int round = 0; round != 400; ++round) {
        std::memset(MCA_DirContents, 0, sizeof MCA_DirContents);
        std::memset(sortedDir, 0xee, sizeof sortedDir);
        MCA_LastDirCount = std::rand() % 40;
        std::vector<int> has;
        for (int i = 0; i != MCA_LastDirCount; ++i) {
            MCA_DirContents[i].bytes[0] = std::rand() % 12;                 // age
            MCA_DirContents[i].bytes[1] = i;                                // identity
            const char* names[] = {"slot1", "xxslotyy", "slo", "SLOT", "", "my slot file"};
            const char* nm = names[std::rand() % 6];
            std::strcpy(reinterpret_cast<char*>(MCA_DirContents[i].bytes + 32), nm);
            // The text must be searched at +32 only: put "slot" in the first half of some records too.
            if (std::rand() % 3 == 0) std::strcpy(reinterpret_cast<char*>(MCA_DirContents[i].bytes + 16), "slot");
            if (std::strstr(nm, "slot")) has.push_back(i);
        }
        // Independent model: stable insertion, new before the first entry it is newer than.
        std::vector<int> model;
        for (int i : has) {
            size_t at = 0;
            while (at < model.size() && !(MCA_DirContents[i].bytes[0] > MCA_DirContents[model[at]].bytes[0])) ++at;
            model.insert(model.begin() + at, i);
        }
        if (model.size() > 10) model.resize(10);
        events.clear(); sortedDirCount = -5;
        generateSortedListAndInfoHeaders__Fv();
        if (sortedDirCount != int(model.size())) { std::printf("count %d vs %zu\n", sortedDirCount, model.size()); return 1; }
        std::string want;
        for (size_t k = 0; k != model.size(); ++k) {
            if (std::memcmp(&sortedDir[k], &MCA_DirContents[model[k]], 64) != 0) return 2;
            want += "M0,1,0," + std::to_string(k * 64 + 32) + ";M3,112,0," + std::to_string(k) + ";L2," + std::to_string(k) + ",0;M1,0,0,0;";
        }
        if (events != want) { std::printf("round %d events\n%s\n%s\n", round, events.c_str(), want.c_str()); return 3; }
        for (size_t k = model.size(); k != 10; ++k) if (sortedDir[k].bytes[0] != 0xee) return 4;   // beyond the count untouched
    }
    std::puts("generateSortedListAndInfoHeaders check passed");
    return 0;
}
