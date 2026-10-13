// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror -DW13_HOST host_check.cpp get_energy_cost.cpp
#include <climits>
#include <cstring>
struct SkillShock;
static unsigned char object_storage[256];
static unsigned char table[160];
static int calls;
struct Call { unsigned function; long adjusted; int a, b; };
static Call log_[8];
static long base() { return reinterpret_cast<long>(object_storage + 64); }
extern "C" const unsigned char* w13_host_table(const unsigned char* self) {
    return self == object_storage + 64 ? table : nullptr;
}
static void entry(int offset, short adjust, unsigned function) {
    std::memcpy(table + offset, &adjust, 2);
    std::memcpy(table + offset + 4, &function, 4);
}
extern "C" float w13_host_call(unsigned function, unsigned char* adjusted, int a, int b) {
    if (calls < 8) log_[calls] = {function, reinterpret_cast<long>(adjusted), a, b};
    ++calls;
    return 2.5f;
}
extern "C" float get_energy_cost(SkillShock*, int) asm("getEnergyCost__10SkillShocki");
int main() {
    unsigned char* object = object_storage + 64;
    SkillShock* self = reinterpret_cast<SkillShock*>(object);
    const int ranks[] = {INT_MIN, -1000, -1, 0, 1, 2, 7, 1000, INT_MAX};
    entry(88, 3, 111);
    entry(120, 5, 115);
    entry(128, 17, 116);
    for (int rank : ranks) {
        calls = 0;
        if (get_energy_cost(self, rank) != 2.5f) return 1;
        if (calls != 1) return 2;
        if (log_[0].function != 116 || log_[0].adjusted != base() + 17 || log_[0].a != rank || log_[0].b != 10) return 3;
    }
    return 0;
}
