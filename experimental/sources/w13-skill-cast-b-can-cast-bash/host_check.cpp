// Host contract for canCast__9SkillBashP8Creature; not an EE input.
#include <cstring>
#include <string>
static std::string calls;
static unsigned short text_marker[2];
extern "C" unsigned char player[4 * 23984];
unsigned char player[4 * 23984];
extern "C" unsigned short* textGetString__FPc(char* key) { calls += std::string("text(") + key + ");"; return text_marker; }
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int time, int player_index, char flag) {
    calls += "hud(" + std::to_string(text == text_marker) + "," + std::to_string(time) + "," + std::to_string(player_index) + "," + std::to_string(flag) + ");";
}
#include "can_cast.cpp"
int main() {
    unsigned char creature[1024];
    char skill;
    for (int index = 0; index < 3; ++index)
        for (int flag_set = 0; flag_set < 2; ++flag_set)
            for (int slot_set = 0; slot_set < 2; ++slot_set) {
                std::memset(player, 0, sizeof player);
                std::memset(creature, 0, sizeof creature);
                const unsigned flags = flag_set ? 0x41 : 0x01, idx = index, marker = 0x1234;
                std::memcpy(creature + 12, &flags, 4);
                std::memcpy(creature + 960, &idx, 4);
                if (slot_set) std::memcpy(player + index * 23984 + 21908, &marker, 4);
                calls.clear();
                const int r = can_cast_bash(&skill, creature);
                const bool ok = !flag_set || slot_set;
                const std::string expected = ok ? "" : "text(bash_warn);hud(1,180," + std::to_string(index) + ",0);";
                if (r != (ok ? 1 : 0) || calls != expected) return 1;
            }
    return 0;
}
