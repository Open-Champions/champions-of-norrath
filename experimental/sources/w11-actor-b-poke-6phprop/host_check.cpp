// Host contract for msg_poke__6PhPropi; not an EE input. Includes the source.
#define W11_ACTOR_B_HOST 1
#include "msg_poke.cpp"
#include <cstring>
#include <initializer_list>
#include <string>
static unsigned char arena[0x8000];
unsigned char* w11_actor_b_host_base = arena;
int nmPlayers;
static std::string calls;
static unsigned mask;  // bit p set: player p takes the item
extern "C" int playerMaybeGetItem__FiPQ24item11description(int p, void* d) {
    calls += "get(" + std::to_string(p) + "," + std::to_string(static_cast<unsigned char*>(d) - arena) + ");";
    return (mask >> p) & 1 ? 1 : 0;
}
unsigned w11_actor_b_host_call(unsigned fn, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "call(" + std::to_string(fn) + "," + std::to_string(static_cast<unsigned char*>(adjusted) - arena) + "," + std::to_string(n) + "," +
             std::to_string(a1) + ");";
    return 0;
}
static void setw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
int main() {
    const unsigned self = 0x400, table = 0x2000;
    for (int bit : {0, 1})
    for (int players : {-1, 0, 1, 2, 4})
    for (unsigned m : {0u, 1u, 2u, 4u, 8u, 0xfu, 0x10u})
    for (int who : {0, 1, 3}) {
        std::memset(arena, 0x11, sizeof arena);
        setw(self + 72, table);
        short adj = -16; std::memcpy(arena + table + 8, &adj, 2);
        setw(table + 8 + 4, 0xabc0);
        unsigned long long flags = bit ? 0x8000000000ull : 0x7fffffffffull | 0xffffff0000000000ull;
        std::memcpy(arena + self + 336, &flags, 8);
        nmPlayers = players; mask = m; calls.clear();
        unsigned char before[sizeof arena]; std::memcpy(before, arena, sizeof arena);
        msg_poke__6PhPropi(arena + self, who);
        std::string want; bool taken = false;
        if (bit) {
            for (int p = 0; p < players; ++p) { want += "get(" + std::to_string(p) + "," + std::to_string(self + 328) + ");"; if ((m >> p) & 1) taken = true; }
        } else {
            want += "get(" + std::to_string(who) + "," + std::to_string(self + 328) + ");";
            taken = (m >> who) & 1;
        }
        if (taken) want += "call(43968," + std::to_string(self - 16) + ",1,3);";
        if (calls != want) return 1;
        if (std::memcmp(before, arena, sizeof arena)) return 2;
    }
    return 0;
}
