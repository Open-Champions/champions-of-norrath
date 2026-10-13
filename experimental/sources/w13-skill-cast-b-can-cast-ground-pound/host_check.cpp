// Host contract for canCast__16SkillGroundPoundP8Creature; not an EE input.
// 32-bit words are offsets from a host base; the text and message calls log.
#define W13_SKILL_CAST_B_HOST 1
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_b_host_base;
int w13_skill_cast_b_host_call(unsigned, void*, int, int) { return 0; }
static std::string calls;
static unsigned short text_marker[2];
extern "C" unsigned char player[4 * 23984];
unsigned char player[4 * 23984];
extern "C" unsigned short* textGetString__FPc(char* key) { calls += std::string("text(") + key + ");"; return text_marker; }
extern "C" void hudSetInfoMessage__FPUsiic(unsigned short* text, int time, int player_index, char flag) {
    calls += "hud(" + std::to_string(text == text_marker) + "," + std::to_string(time) + "," + std::to_string(player_index) + "," + std::to_string(flag) + ");";
}
#include "can_cast.cpp"
static unsigned char arena[0x400] __attribute__((aligned(16)));
static void put32(unsigned char* where, unsigned value) { std::memcpy(where, &value, 4); }
int main() {
    w13_skill_cast_b_host_base = arena;
    unsigned char creature[1024];
    char skill;
    const unsigned item_word = 0x100, description_word = 0x200;
    for (int index = 0; index < 3; ++index) {
        for (int flag_set = 0; flag_set < 2; ++flag_set)
            for (int state = 0; state < 4; ++state)
                for (int slot = -1; slot < 3; ++slot)
                    for (int item_kind = 0; item_kind < 4; ++item_kind) {
                        std::memset(player, 0, sizeof player);
                        std::memset(creature, 0, sizeof creature);
                        std::memset(arena, 0, sizeof arena);
                        put32(creature + 12, flag_set ? 0x41 : 0x01);
                        put32(creature + 960, index);
                        unsigned char* record = player + index * 23984;
                        record[23582] = state;
                        record[23572] = static_cast<unsigned char>(slot);
                        // item_kind: 0 empty slot, 1 description word 1, 2 description word 0, 3 description word 2
                        if (item_kind) {
                            put32(record + 21960 + 4 * slot, item_word);
                            put32(arena + item_word, description_word);
                            put32(arena + description_word + 32, item_kind == 1 ? 1 : item_kind == 2 ? 0 : 2);
                        }
                        calls.clear();
                        const int r = can_cast_ground_pound(&skill, creature);
                        const bool ok = !flag_set || (state == 2 && (item_kind == 0 || item_kind == 1));
                        const std::string expected = ok ? "" : "text(groundpound_warn);hud(1,180," + std::to_string(index) + ",0);";
                        if (r != (ok ? 1 : 0) || calls != expected) return 1;
                    }
    }
    return 0;
}
