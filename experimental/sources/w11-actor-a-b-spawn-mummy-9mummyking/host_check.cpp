// Host contract for bSpawnMummy__9MummyKing; not an EE input. Returns non-zero when wrong.
#include <cstring>
#include <initializer_list>
extern "C" int nmPlayers;
int nmPlayers;
#include "b_spawn_mummy.cpp"
int main() {
    alignas(16) static unsigned char o[2048];
    int bad = 0;
    for (int count : {-1, 0, 1, 2, 4})
        for (unsigned tail : {0u, 5u})
            for (unsigned mask = 0; mask < 16; ++mask) {
                std::memset(o, 0, sizeof o);
                nmPlayers = count;
                for (int i = 0; i < 4; ++i) { unsigned v = (mask >> i & 1) ? 0x77u : 0u; std::memcpy(o + 1108 + 4 * i, &v, 4); }
                std::memcpy(o + 1124, &tail, 4);
                int want = 0;
                for (int i = 0; i < count; ++i) if (!(mask >> i & 1)) { want = 1; break; }
                if (!want) want = tail == 0;
                if (bSpawnMummy__9MummyKing(o) != want) bad = 1;
            }
    return bad;
}
