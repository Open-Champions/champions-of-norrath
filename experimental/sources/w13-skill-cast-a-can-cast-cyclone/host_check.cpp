// Host contract for canCast__12SkillCycloneP8Creature; not an EE input.
// The text lookup and the message call are stubs that log their calls; player is a small host array whose record
// size is the observed 23984 bytes.
#include <cstring>
#include <string>
static std::string calls;
static unsigned short fake_text[2];
extern "C" {
unsigned char player[23984 * 3];
unsigned short* textGetString__FPc(const char* key) { calls += std::string("text(") + key + ");"; return fake_text; }
void hudSetInfoMessage__FPUsiic(unsigned short* text, int a, int b, char c) {
    calls += "msg(" + std::to_string(text == fake_text) + "," + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(static_cast<int>(c)) + ");";
}
int can_cast_cyclone(void* skill, void* creature) asm("canCast__12SkillCycloneP8Creature");
}
int main() {
    alignas(8) unsigned char creature[1024];
    char skill;
    const int flags[] = {0, 0x40, 0x3f, -65, -1};
    const int bytes[] = {0, 3, 4, 5, -4, 127, -128};
    for (int fl : flags)
        for (int index = 0; index < 3; ++index)
            for (int b : bytes) {
                std::memset(creature, 0, sizeof creature);
                std::memset(player, 0, sizeof player);
                std::memcpy(creature + 12, &fl, 4);
                std::memcpy(creature + 960, &index, 4);
                player[index * 23984 + 23582] = static_cast<unsigned char>(b);
                calls.clear();
                const int r = can_cast_cyclone(&skill, creature);
                const bool blocked = (fl & 0x40) && b == 4;
                if (r != (blocked ? 0 : 1)) return 1;
                if (calls != (blocked ? "text(cyclone_warn);msg(1,180," + std::to_string(index) + ",0);" : "")) return 2;
            }
    return 0;
}
