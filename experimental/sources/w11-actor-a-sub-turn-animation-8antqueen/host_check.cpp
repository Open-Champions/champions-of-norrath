// Host contract for subTurnAnimation__8AntQueen; not an EE input. Words that hold addresses are offsets from a
// host base. Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
unsigned char* w11_actor_a_host_base;
#include "sub_turn_animation.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    alignas(16) static unsigned char arena[0x4000];
    w11_actor_a_host_base = arena;
    int bad = 0;
    unsigned char* o = arena + 0x1000;
    for (int n : {-3, 0, 1, 3, 6, 127, 0xe0})
        for (unsigned pick = 0; pick < 6; ++pick) {
            std::memset(arena, 0, sizeof arena);
            unsigned rw = 0x200; std::memcpy(o + 36, &rw, 4);
            const unsigned ids[4] = {111, 222, 333, 444};
            std::memcpy(arena + 0x200 + 180, &ids[0], 4); std::memcpy(arena + 0x200 + 68, &ids[1], 4);
            std::memcpy(arena + 0x200 + 188, &ids[2], 4); std::memcpy(arena + 0x200 + 76, &ids[3], 4);
            o[292] = static_cast<unsigned char>(n);
            float mark = 5.0f;
            const unsigned recs[8] = {111, 222, 333, 444, 555, 111, 0, 444};
            for (int i = 0; i < 8; ++i) {
                unsigned v = recs[(i + pick) % 8];
                std::memcpy(o + 296 + 32 * i, &v, 4);
                std::memcpy(o + 308 + 32 * i, &mark, 4);
            }
            subTurnAnimation__8AntQueen(o);
            for (int i = 0; i < 8; ++i) {
                float f; std::memcpy(&f, o + 308 + 32 * i, 4);
                unsigned v = recs[(i + pick) % 8];
                bool hit = i < static_cast<signed char>(n) && (v == 111 || v == 222 || v == 333 || v == 444);
                float tgt = hit ? __builtin_bit_cast(float, 0xbd088888u) : 5.0f;
                if (std::memcmp(&f, &tgt, 4) != 0) bad = 1;
            }
        }
    return bad;
}
