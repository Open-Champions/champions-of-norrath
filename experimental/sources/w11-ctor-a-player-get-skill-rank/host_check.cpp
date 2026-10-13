// Host contract for getSkillRank__6Playeri; not an EE input. The callee stub records its
// arguments and returns a value derived from them, so a swapped, dropped or wrongly offset
// argument changes the result.
#include "get_skill_rank.cpp"
#include <cstring>
#include <initializer_list>
static int seen_owner, seen_skill, calls;
extern "C" int playerGetSkillRank__Fii(int owner, int skill) {
    seen_owner = owner; seen_skill = skill; ++calls;
    return owner * 1000 + skill;
}
int main() {
    alignas(16) unsigned char player[1024];
    std::memset(player, 0x11, sizeof player);
    int bad = 0;
    for (int owner : {0, 1, 3, -2}) {
        for (int skill : {0, 7, 61}) {
            std::memcpy(player + 960, &owner, 4);
            calls = 0;
            const int r = getSkillRank__6Playeri(player, skill);
            if (r != owner * 1000 + skill || seen_owner != owner || seen_skill != skill || calls != 1) bad = 1;
        }
    }
    return bad;
}
