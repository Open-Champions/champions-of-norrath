// Host contract for cast__17SkillConvertEnemyiP8CreatureG6Point3s; not an EE input.
#include <string>
static std::string calls;
static void* fake_block;
struct ListHead { void* first; unsigned unknown[3]; };
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
void spell_ctor_convert_enemy(void* self, int rank, void* creature, void* skill) asm("__17SpellConvertEnemyiP8CreatureP5Skill");
void spell_ctor_convert_enemy(void* self, int rank, void* creature, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(skill)) + ");";
}
void listDestroy__FP8ListHead(ListHead* list) { calls += std::string("destroy(") + (list->first == nullptr ? "empty" : "nonempty") + ");"; }
struct Point3 { float x, y, z; };
void* cast_convert_enemy(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__17SkillConvertEnemyiP8CreatureG6Point3s");
}
int main() {
    char block, skill, creature;
    const int ranks[] = {-3, 0, 1, 99};
    for (int rank : ranks) {
        fake_block = &block; calls.clear();
        void* r = cast_convert_enemy(&skill, rank, &creature, Point3{1.0f, 2.0f, 3.0f}, 4);
        const std::string expected = "alloc(120);ctor(1," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(&creature)) + "," +
                                     std::to_string(reinterpret_cast<unsigned long>(&skill)) + ");destroy(empty);";
        if (r != &block || calls != expected) return 1;
        fake_block = nullptr; calls.clear();
        r = cast_convert_enemy(&skill, rank, &creature, Point3{0, 0, 0}, -4);
        if (r != nullptr || calls != "alloc(120);destroy(empty);") return 2;
    }
    return 0;
}
