// Host contract for cast__12SkillBlessediP8CreatureG6Point3s; not an EE input.
#include <cstring>
#include <string>
struct Point3 { float x, y, z; };
static std::string calls;
static void* fake_block;
static void* running;
static int node;
extern "C" {
void* blockAlloc__Fi(int size) { calls += "alloc(" + std::to_string(size) + ");"; return fake_block; }
const void* spell_type_info() asm("__tf12SpellBlessed");
const void* spell_type_info() { calls += "tf;"; return &node; }
void* checkIfSpellIsRunning__8CreatureRC9type_info(void* creature, const void* typeinfo) {
    calls += "check(" + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," + (typeinfo == &node ? "node" : "?") + ");";
    return running;
}
void spell_ctor_blessed(void* self, int rank, void* creature, void* skill) asm("__12SpellBlessediP8CreatureP5Skill");
void spell_ctor_blessed(void* self, int rank, void* creature, void* skill) {
    calls += "ctor(" + std::to_string(self == fake_block) + "," + std::to_string(rank) + "," + std::to_string(reinterpret_cast<unsigned long>(creature)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(skill)) + ");";
}
void* cast_blessed(void* skill, int rank, void* creature, Point3 point) asm("cast__12SkillBlessediP8CreatureG6Point3s");
}
int main() {
    char block, skill;
    alignas(8) unsigned char creature[256];
    for (int rank : {-3, 0, 99}) {
        unsigned char before[sizeof creature];
        std::memset(creature, 0x5a, sizeof creature);
        std::memcpy(before, creature, sizeof creature);
        running = &block; fake_block = &block; calls.clear();
        if (cast_blessed(&skill, rank, creature, Point3{1, 2, 3}) != nullptr) return 1;
        if (calls != "tf;check(" + std::to_string(reinterpret_cast<unsigned long>(creature)) + ",node);" || std::memcmp(before, creature, sizeof creature) != 0) return 2;
        running = nullptr; calls.clear();
        void* r = cast_blessed(&skill, rank, creature, Point3{1, 2, 3});
        const short v75 = 75, v200 = 200, v15 = 15;
        const int at75[] = {140, 142, 148, 150}, at200[] = {144, 152}, at15[] = {146, 154};
        for (int o : at75) std::memcpy(before + o, &v75, 2);
        for (int o : at200) std::memcpy(before + o, &v200, 2);
        for (int o : at15) std::memcpy(before + o, &v15, 2);
        const std::string expected = "tf;check(" + std::to_string(reinterpret_cast<unsigned long>(creature)) + ",node);alloc(252);ctor(1," + std::to_string(rank) + "," +
                                     std::to_string(reinterpret_cast<unsigned long>(creature)) + "," + std::to_string(reinterpret_cast<unsigned long>(&skill)) + ");";
        if (r != &block || calls != expected || std::memcmp(before, creature, sizeof creature) != 0) return 3;
        fake_block = nullptr; calls.clear();
        r = cast_blessed(&skill, rank, creature, Point3{0, 0, 0});
        if (r != nullptr || calls.find("alloc(252);") == std::string::npos || calls.find("ctor") != std::string::npos) return 4;
    }
    return 0;
}
