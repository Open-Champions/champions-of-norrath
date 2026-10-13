// Host contract for canCast__18SkillConvertUndeadP8Creature; not an EE input.
// The object-table call, the box query and the message calls log.
#define W13_SKILL_CAST_B_HOST 1
#include <cstring>
#include <string>
unsigned char* w13_skill_cast_b_host_base;
#include "can_cast.cpp"
static std::string calls;
static unsigned short text_marker[2];
static void* result_list[8];
static int table_results[8];
int w13_skill_cast_b_host_call(unsigned, void*, int, int) { return 0; }
int w13_skill_cast_b_host_call_object(unsigned function_word, void* adjusted, void* argument) {
    int which = 0;
    for (int i = 0; i < 8; ++i) if (argument == result_list[i]) which = i;
    calls += "entry(" + std::to_string(function_word) + "," + std::to_string(static_cast<unsigned char*>(adjusted) - w13_skill_cast_b_host_base) + "," + std::to_string(which) + ");";
    return table_results[which];
}
extern "C" {
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** list, int max) {
    calls += "box(" + std::to_string(low.x) + "," + std::to_string(low.y) + "," + std::to_string(low.z) + "," + std::to_string(high.x) + "," + std::to_string(high.y) + "," +
             std::to_string(high.z) + "," + std::to_string(max) + ");";
    for (int i = 0; i < 8; ++i) list[i] = result_list[i];
}
unsigned short* textGetString__FPc(char* key) { calls += std::string("text(") + key + ");"; return text_marker; }
void hudSetInfoMessage__FPUsiic(unsigned short* text, int time, int player_index, char flag) {
    calls += "hud(" + std::to_string(text == text_marker) + "," + std::to_string(time) + "," + std::to_string(player_index) + "," + std::to_string(flag) + ");";
}
}
static unsigned char arena[0x800] __attribute__((aligned(16)));
static unsigned char objects[8][256];
int main() {
    w13_skill_cast_b_host_base = arena;
    unsigned char* const creature = arena + 0x40;
    char skill;
    const unsigned table = 0x600;
    const short adjust = -16;
    const unsigned function = 0x5555;
    const float pos[3] = {10.5f, -20.0f, 1000.0f};
    // list shapes: how many entries precede the null, which entries pass the table check, which have type byte 16
    for (int count = 0; count <= 4; ++count)
        for (int pass_mask = 0; pass_mask < 16; ++pass_mask)
            for (int type_mask = 0; type_mask < 16; ++type_mask) {
                std::memset(arena, 0, sizeof arena);
                std::memset(objects, 0, sizeof objects);
                std::memcpy(creature + 16, pos, 12);
                const unsigned flags = 0x40, tbl = table, player_index = 2;
                std::memcpy(creature + 12, &flags, 4);
                std::memcpy(creature + 72, &tbl, 4);
                std::memcpy(creature + 960, &player_index, 4);
                std::memcpy(arena + table + 160, &adjust, 2);
                std::memcpy(arena + table + 164, &function, 4);
                for (int i = 0; i < 8; ++i) { result_list[i] = i < count ? static_cast<void*>(objects[i]) : nullptr; table_results[i] = (pass_mask >> i) & 1 ? 7 : 0; objects[i][172] = (type_mask >> i) & 1 ? 16 : 3; }
                calls.clear();
                const int r = can_cast_convert_undead(&skill, creature);
                std::string expected = "box(" + std::to_string(10.5f - 329.0f) + "," + std::to_string(-20.0f - 329.0f) + "," + std::to_string(1000.0f - 329.0f) + "," +
                                       std::to_string(10.5f + 329.0f) + "," + std::to_string(-20.0f + 329.0f) + "," + std::to_string(1000.0f + 329.0f) + ",512);";
                int want = 0;
                for (int i = 0; i < count; ++i) {
                    expected += "entry(21845," + std::to_string(creature - 16 - arena) + "," + std::to_string(i) + ");";
                    if (((pass_mask >> i) & 1) && ((type_mask >> i) & 1)) { want = 1; break; }
                }
                if (!want) expected += "text(convertundead_warn);hud(1,180,2,0);";
                if (r != want || calls != expected) return 1;
            }
    return 0;
}
