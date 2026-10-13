// Host contract for addTurnAnimation__8AntQueen; not an EE input. Words that hold addresses are offsets from a
// host base. The expected values are computed here with the steps stated in add_turn_animation.cpp. The
// authored function was also compared with the original in a private R5900 run. Returns non-zero when wrong.
#define W11_ACTOR_A_HOST 1
unsigned char* w11_actor_a_host_base;
#include "add_turn_animation.cpp"
#include <cmath>
#include <cstring>
#include <initializer_list>
int main() {
    alignas(16) static unsigned char arena[0x4000];
    w11_actor_a_host_base = arena;
    int bad = 0;
    unsigned char* o = arena + 0x1000;
    for (short h : {(short)0, (short)256, (short)-256, (short)1000, (short)-1000, (short)512, (short)-512})
        for (float p : {0.0f, 0.5f, -0.75f})
            for (float q : {0.0f, 0.25f, -0.5f}) {
                std::memset(arena, 0, sizeof arena);
                unsigned rw = 0x200; std::memcpy(o + 36, &rw, 4);
                const unsigned ids[4] = {111, 222, 333, 444};
                std::memcpy(arena + 0x200 + 180, &ids[0], 4); std::memcpy(arena + 0x200 + 68, &ids[1], 4);
                std::memcpy(arena + 0x200 + 188, &ids[2], 4); std::memcpy(arena + 0x200 + 76, &ids[3], 4);
                std::memcpy(o + 284, &h, 2); std::memcpy(o + 980, &p, 4); std::memcpy(o + 984, &q, 4);
                o[292] = 5;
                const unsigned recs[6] = {111, 222, 333, 444, 555, 111};
                for (int i = 0; i < 6; ++i) { std::memcpy(o + 296 + 32 * i, &recs[i], 4); unsigned m = 0x12345678u; std::memcpy(o + 304 + 32 * i, &m, 4); }
                float t = (float)h * (1.0f / 512.0f);
                if (t > 1.0f) t = 1.0f;
                if (t < -1.0f) t = -1.0f;
                float d = p + (t - q) * 0.09f;
                float s = q + d;
                float w = s + s;
                addTurnAnimation__8AntQueen(o);
                float gp, gq; short out;
                std::memcpy(&gp, o + 980, 4); std::memcpy(&gq, o + 984, 4); std::memcpy(&out, o + 988, 2);
                if (std::abs(gq - s) > 1e-6f || std::abs(gp - (d - d * 0.96f)) > 1e-6f) bad = 1;
                if (out != (short)(int)(w * 2177.0f) && std::abs(out - w * 2177.0f) > 1.5f) bad = 1;
                for (int i = 0; i < 6; ++i) {
                    unsigned v; std::memcpy(&v, o + 304 + 32 * i, 4);
                    float f; std::memcpy(&f, &v, 4);
                    bool left = recs[i] == 111 || recs[i] == 222, right = recs[i] == 333 || recs[i] == 444;
                    if (i >= 5) { if (v != 0x12345678u) bad = 1; continue; }
                    if (w > 0) {
                        if (left && std::abs(f - w) > 1e-5f) bad = 1;
                        if (right && v != 0) bad = 1;
                    } else {
                        if (left && v != 0) bad = 1;
                        if (right && std::abs(f + w) > 1e-5f) bad = 1;
                    }
                    if (!left && !right && v != 0x12345678u) bad = 1;
                }
            }
    return bad;
}
