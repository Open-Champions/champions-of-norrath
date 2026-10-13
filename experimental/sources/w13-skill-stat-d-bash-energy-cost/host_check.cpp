#include <cstring>
#include <cstdint>
#include <initializer_list>
struct SkillBash;
struct VtPair { short adjustment; short pad; float (*function)(void*, int, int); };
extern "C" float get_energy_cost(SkillBash*, int) asm("getEnergyCost__9SkillBashi");
static void* seen_self; static int seen_rank, seen_c, calls;
static float fake(void* s, int r, int c) { seen_self = s; seen_rank = r; seen_c = c; ++calls; return 3.5f + r; }
struct Obj { char bytes[96]; const VtPair* vt; char more[32]; };
int main() {
    static VtPair table[18];
    for (int adj : {0, 8, -16}) {
        table[16].adjustment = static_cast<short>(adj); table[16].function = fake;
        table[15].function = nullptr; table[17].function = nullptr;
        Obj o; std::memset(&o, 0, sizeof o); o.vt = table; calls = 0;
        for (int rank : {-5, 0, 1, 7, 99}) {
            float r = get_energy_cost(reinterpret_cast<SkillBash*>(&o), rank);
            if (r != 3.5f + rank || seen_rank != rank || seen_c != 19) return 1;
            if (seen_self != reinterpret_cast<char*>(&o) + adj) return 2;
        }
        if (calls != 5) return 3;
    }
    return 0;
}
