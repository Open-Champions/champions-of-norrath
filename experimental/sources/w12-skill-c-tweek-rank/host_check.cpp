#include <climits>
struct Skill;
extern "C" int tweek_rank(const Skill*, int, int) asm("tweekRank__5Skillii");
int main() {
    const Skill* p = reinterpret_cast<const Skill*>(static_cast<unsigned long>(0x1234));
    const int values[] = {INT_MIN, -1, 0, 1, 13, INT_MAX};
    for (int first : values) for (int second : values) if (tweek_rank(p, first, second) != second) return 1;
    return 0;
}
