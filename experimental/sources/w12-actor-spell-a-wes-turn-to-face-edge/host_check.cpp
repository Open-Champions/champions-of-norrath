// Host contract for TurnToFaceEdge__14WoodElfSoldier; not an EE input. Address-holding words are offsets from a host base.
// The ground search is scripted by call number; the stubs record every call.
#define W10_ACTOR_M_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include "turn.cpp"
unsigned char* w10_actor_m_host_base;
void* world;
alignas(16) static unsigned char arena[0x2000];
static std::string order;
static bool lows[4];
static int call_index;
static const unsigned TURN_FN = 0xaaaa0001u;
unsigned w10_actor_m_host_call(unsigned fn, void* a0, int n, unsigned long a1, unsigned long a2) {
    char b[160];
    const float* t = reinterpret_cast<const float*>(a2);
    std::snprintf(b, sizeof b, "turn(%d,%d,%d,%lu,%.5f,%.5f,%.5f);", fn == TURN_FN, static_cast<int>(static_cast<unsigned char*>(a0) - arena), n, a1, t[0], t[1], t[2]);
    order += b;
    return 0;
}
extern "C" float icos__Fi(int a) { char b[32]; std::snprintf(b, sizeof b, "c%d;", a); order += b; return 0.25f + static_cast<float>(a & 0xffff) / 70000.0f; }
extern "C" float isin__Fi(int a) { char b[32]; std::snprintf(b, sizeof b, "s%d;", a); order += b; return -0.5f + static_cast<float>(a & 0xffff) / 90000.0f; }
extern "C" float worldFindStandHeight__FP12_worldHeaderG6Point3f(void* w, Point3 p, float r) {
    char b[128];
    std::snprintf(b, sizeof b, "h(%d,%.5f,%.5f,%.5f,%.1f);", w == world, p.x, p.y, p.z, r);
    order += b;
    float z; std::memcpy(&z, arena + 0x100 + 24, 4);
    return lows[call_index++] ? z - 150.0f : z + 5.0f;
}
int main() {
    w10_actor_m_host_base = arena;
    world = arena + 0x1800;
    unsigned char* self = arena + 0x100;
    int bad = 0;
    for (int mask = 0; mask < 16; ++mask) {
        for (int i = 0; i < 4; ++i) lows[i] = (mask >> i) & 1;
        std::memset(arena, 0x5a, sizeof arena);
        const float pos[3] = {100.0f, -40.0f, 12.0f};
        std::memcpy(self + 16, pos, 12);
        const unsigned table = 0x1000;
        std::memcpy(self + 72, &table, 4);
        const short adjust = 16;
        std::memcpy(arena + table + 176, &adjust, 2);
        std::memcpy(arena + table + 180, &TURN_FN, 4);
        const float old[3] = {1.0f, 2.0f, 3.0f};
        std::memcpy(self + 272, old, 12);
        order.clear(); call_index = 0;
        TurnToFaceEdge__14WoodElfSoldier(self);
        // independent model
        const int angles[4] = {0, 16384, 32767, -16384};
        std::string want;
        float q[3] = {pos[0], pos[1], pos[2]};
        float result[3] = {1.0f, 2.0f, 3.0f};
        char b[160];
        for (int a = 0; a < 4; ++a) {
            const float cx = (0.25f + static_cast<float>(angles[a] & 0xffff) / 70000.0f) * 30.0f;
            const float sy = (-0.5f + static_cast<float>(angles[a] & 0xffff) / 90000.0f) * 30.0f;
            want += "c" + std::to_string(angles[a]) + ";s" + std::to_string(angles[a]) + ";";
            if (a == 1) { q[0] = pos[0] + cx; q[1] = pos[1] + sy; q[2] = pos[2]; } else { q[0] += cx; q[1] += sy; }
            std::snprintf(b, sizeof b, "h(1,%.5f,%.5f,%.5f,10.0);", q[0], q[1], q[2]);
            want += b;
            if ((mask >> a) & 1) { std::memcpy(result, q, 12); break; }
        }
        std::snprintf(b, sizeof b, "turn(1,%d,2,0,%.5f,%.5f,%.5f);", 0x100 + 16, result[0], result[1], result[2]);
        want += b;
        if (order != want) { std::fprintf(stderr, "mask %d\n%s\n%s\n", mask, order.c_str(), want.c_str()); bad = 1; }
        float got[3]; std::memcpy(got, self + 272, 12);
        if (std::memcmp(got, result, 12) != 0) { std::fprintf(stderr, "stored point wrong (mask %d)\n", mask); bad = 1; }
    }
    if (!bad) std::puts("turn to face edge ok");
    return bad;
}
