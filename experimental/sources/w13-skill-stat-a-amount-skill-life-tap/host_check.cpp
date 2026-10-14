// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror -DW13_HOST host_check.cpp get_amount.cpp
#include <climits>
#include <cstring>
struct SkillLifeTap;
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
extern "C" int w13_host_call(unsigned function, unsigned char* adjusted, int a, int b) {
    if (calls < 8) log_[calls] = {function, reinterpret_cast<long>(adjusted), a, b};
    ++calls;
    return (function == 115 ? static_cast<int>(static_cast<unsigned>(a) * 3u + static_cast<unsigned>(b)) : static_cast<int>(1000u + static_cast<unsigned>(a) * 7u + static_cast<unsigned>(b)));
}
extern "C" int get_amount(SkillLifeTap*, int) asm("getAmount__12SkillLifeTapi");
int main() {
    unsigned char* object = object_storage + 64;
    SkillLifeTap* self = reinterpret_cast<SkillLifeTap*>(object);
    const int ranks[] = {INT_MIN, -1000, -1, 0, 1, 2, 7, 1000, INT_MAX};
    entry(88, 3, 111);
    entry(120, 5, 115);
    entry(128, 17, 116);
    for (int rank : ranks) {
        calls = 0;
        const int tweaked = static_cast<int>(static_cast<unsigned>(rank) * 3u + 1u);
        const int expected = static_cast<int>(1000u + 7u + static_cast<unsigned>(tweaked));
        if (get_amount(self, rank) != expected) return 1;
        if (calls != 2) return 2;
        if (log_[0].function != 115 || log_[0].adjusted != base() + 5 || log_[0].a != rank || log_[0].b != 1) return 3;
        if (log_[1].function != 111 || log_[1].adjusted != base() + 3 || log_[1].a != 1 || log_[1].b != tweaked) return 4;
    }
    return 0;
}
