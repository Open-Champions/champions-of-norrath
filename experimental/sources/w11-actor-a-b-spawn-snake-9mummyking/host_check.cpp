// Host contract for bSpawnSnake__9MummyKing; not an EE input. Returns non-zero when wrong.
#include <cstring>
#include <initializer_list>
extern "C" int nmPlayers;
int nmPlayers;
#include "b_spawn_snake.cpp"
int main() {
    alignas(16) static unsigned char o[2048];
    int bad = 0;
    for (int count : {-1, 0, 1, 2, 4})
        for (int lim : {-5, 0, 3})
            for (int cur : {-9, 0, 2, 3, 4})
                for (unsigned mask = 0; mask < 32; ++mask) {
                    std::memset(o, 0, sizeof o);
                    nmPlayers = count;
                    std::memcpy(o + 808, &lim, 4); std::memcpy(o + 1132, &cur, 4);
                    for (int i = 0; i < 5; ++i) { unsigned v = (mask >> i & 1) ? 0x77u : 0u; std::memcpy(o + 1136 + 4 * i, &v, 4); }
                    int want = 0;
                    if (cur < lim) for (int i = 0; i < count; ++i) if (!(mask >> i & 1)) { want = 1; break; }
                    if (bSpawnSnake__9MummyKing(o) != want) bad = 1;
                }
    return bad;
}
