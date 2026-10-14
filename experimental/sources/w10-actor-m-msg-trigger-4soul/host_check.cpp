// Host contract for msg_trigger__4Souli; not an EE input. The table entry at byte 184 logs the
// adjusted object, its argument and the value of the byte at +570 at call time.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
unsigned char* w10_actor_m_host_base;
static int ncalls, seen_byte, seen_arg; static void* seen_obj; static unsigned seen_fn;
unsigned w10_actor_m_host_call(unsigned fn, void* adjusted, int n, unsigned long a1, unsigned long) {
    ++ncalls; seen_fn = fn; seen_obj = adjusted; seen_arg = static_cast<int>(a1);
    seen_byte = w10_actor_m_host_base[0x100 + 570]; (void)n; return 0;
}
#include "msg_trigger.cpp"
alignas(16) static unsigned char arena[0x2000];
int main() {
    w10_actor_m_host_base = arena;
    int bad = 0;
    for (int trig : {0, 1, -1, 10, 100}) {
        for (unsigned char b : {0x00, 0x7f, 0x80, 0xff}) {
            std::memset(arena, 0x55, sizeof arena);
            const unsigned obj = 0x100, table = 0x800;
            std::memcpy(arena + obj + 72, &table, 4);
            short adj = 8; std::memcpy(arena + table + 184, &adj, 2);
            unsigned fn = 0xcafe0001u; std::memcpy(arena + table + 188, &fn, 4);
            arena[obj + 570] = b;
            ncalls = 0; seen_byte = -1;
            unsigned char ref[0x2000]; std::memcpy(ref, arena, sizeof ref);
            msg_trigger__4Souli(arena + obj, trig);
            if (trig != 0) { if (ncalls != 0 || std::memcmp(ref, arena, sizeof ref)) bad = 1; continue; }
            if (ncalls != 1 || seen_fn != 0xcafe0001u || seen_obj != arena + obj + 8 || seen_arg != 10) bad = 1;
            if (seen_byte != 0) bad = 1;
            if (arena[obj + 570] != b) bad = 1;
            if (std::memcmp(ref, arena, sizeof ref)) bad = 1;
        }
    }
    return bad;
}
