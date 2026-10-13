// Host check for givePlayerSkill__Fii. Not an EE plan input.
#include <cstring>
#include <string>
#define memset host_memset
#include "give_player_skill.cpp"
#undef memset
unsigned char player[2 * 23984];
unsigned char* skills[16];
const unsigned char norrath_player_skill_placeholder_view[9] = {'n', 'e', 'w', 's', 't', 'y', 'l', 'e', 0};
static std::string ev;
extern "C" {
void* host_memset(void* d, int v, unsigned n) { ev += "M" + std::to_string(n) + ";"; return std::memset(d, v, n); }
int playerGetSkillRank__Fii(int p, int s) { return p == 0 && s == 4 ? 0 : 7; }
void playerSetSkillRank__Fiii(int p, int s, int r) { ev += "S" + std::to_string(p) + "," + std::to_string(s) + "," + std::to_string(r) + ";"; }
}
static short& id(int p, int e) { return *reinterpret_cast<short*>(player + p * 23984 + 22008 + 104 * e); }
int main() {
    static unsigned char kind[16][16];
    for (int k = 0; k < 16; ++k) { skills[k] = kind[k]; kind[k][8] = static_cast<unsigned char>(0x40 + k); }
    kind[9][8] = 3;
    for (int p = 0; p < 2; ++p) for (int e = 0; e < 15; ++e) id(p, e) = -1;
    id(0, 0) = 2; id(0, 1) = 6;
    std::memset(player + 21976 + 104 * 2, 0xab, 104);
    id(0, 2) = -1;                                                             // dirty but free entry 2
    givePlayerSkill__Fii(4, 0);
    unsigned char* entry = player + 21976 + 104 * 2;
    if (ev != "M104;S0,4,1;") return 1;                                        // rank 0 is raised to 1
    if (std::memcmp(entry, "newstyle", 9) != 0 || entry[9] != 0 || id(0, 2) != 4 || entry[36] != 1 || entry[37] != 0x44) return 2;
    ev.clear();
    givePlayerSkill__Fii(4, 0);
    if (!ev.empty()) return 3;                                                 // already owned
    givePlayerSkill__Fii(9, 0);
    if (!ev.empty()) return 4;                                                 // kind 3 is ignored
    givePlayerSkill__Fii(5, 1);
    if (ev != "M104;S1,5,7;" || id(1, 0) != 5) return 5;                       // rank 7 stays
    ev.clear();
    for (int e = 0; e < 15; ++e) id(1, e) = static_cast<short>(20 + e);
    givePlayerSkill__Fii(6, 1);
    if (!ev.empty()) return 6;                                                 // table full
    id(1, 14) = -1;
    givePlayerSkill__Fii(6, 1);
    if (id(1, 14) != 6) return 7;                                              // last entry is usable
    return 0;
}
