// Host contract for msg_alert__8Skeleton...; not an EE input. iatan2 and the table entry are stubs.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
unsigned char* w10_actor_m_host_base;
static int ncalls, last_arg; static void* last_obj; static unsigned last_fn;
unsigned w10_actor_m_host_call(unsigned fn, void* o, int, unsigned long a1, unsigned long) { ++ncalls; last_fn = fn; last_obj = o; last_arg = static_cast<int>(a1); return 0; }
static int nat, ang; static float at_y, at_x;
extern "C" int iatan2__Fff(float a, float b) { ++nat; at_y = a; at_x = b; return ang; }
#include "msg_alert.cpp"
int main() {
    int bad = 0;
    w10_actor_m_host_base = nullptr;
    alignas(16) static unsigned char arena[0x2000];
    w10_actor_m_host_base = arena;
    const unsigned O = 0x100, T = 0x900, VT = 0x1000;
    for (int kind : {2, 1, 0, 3}) for (unsigned w1120 : {0u, 1u}) for (short st : {3, 5, 4, 0, 10}) for (int hasT : {1, 0})
    for (int h : {0, 500, -500, 32767, -32768}) for (int off : {-10000, -9999, -9998, 9998, 9999, 10000, 20000}) {
        std::memset(arena, 0, sizeof arena);
        std::memcpy(arena + O + 1120, &w1120, 4); std::memcpy(arena + O + 572, &st, 2);
        short hh = static_cast<short>(h); std::memcpy(arena + O + 56, &hh, 2);
        float ox = 5.0f, oy = 2.0f, tx = 9.0f, ty = 11.5f;
        std::memcpy(arena + O + 16, &ox, 4); std::memcpy(arena + O + 20, &oy, 4);
        std::memcpy(arena + T + 16, &tx, 4); std::memcpy(arena + T + 20, &ty, 4);
        unsigned vt = VT; std::memcpy(arena + O + 72, &vt, 4);
        short adj = 8; std::memcpy(arena + VT + 184, &adj, 2);
        unsigned fn = 0xfeed0023u; std::memcpy(arena + VT + 188, &fn, 4);
        ncalls = nat = 0; ang = h + off;
        msg_alert__8SkeletonifG6Point3P10GameObject(arena + O, kind, 1.0f, nullptr, hasT ? arena + T : nullptr);
        const unsigned delta = static_cast<unsigned>(ang) - static_cast<unsigned>(h) + 9999u;
        const bool reach = kind == 2 && w1120 != 0 && (st == 3 || st == 5) && hasT;
        if (nat != (reach ? 1 : 0)) bad = 1;
        if (reach && (at_y != ty - oy || at_x != tx - ox)) bad = 1;
        const bool fire = reach && delta < 19999u;
        if (ncalls != (fire ? 1 : 0)) bad = 1;
        if (fire && (last_fn != fn || last_obj != arena + O + 8 || last_arg != 15)) bad = 1;
    }
    return bad;
}
