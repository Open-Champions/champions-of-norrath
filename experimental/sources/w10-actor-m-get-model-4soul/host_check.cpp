// Host contract for getModel__4Soul; not an EE input. Words are offsets from a host base.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
unsigned char* w10_actor_m_host_base;
unsigned w10_actor_m_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
#include "get_model.cpp"
alignas(16) static unsigned char arena[0x2000];
int main() {
    w10_actor_m_host_base = arena;
    int bad = 0;
    for (unsigned minion : {0u, 1u, 0x1234u, 0x80000000u}) {
        std::memset(arena, 0, sizeof arena);
        const unsigned table = 0x1000, obj = 0x100;
        std::memcpy(arena + obj + 36, &table, 4);
        std::memcpy(arena + obj + 960, &minion, 4);
        const unsigned w12 = 0xaaaa0012u, w28 = 0xbbbb0028u;
        std::memcpy(arena + table + 12, &w12, 4);
        std::memcpy(arena + table + 28, &w28, 4);
        const unsigned want = minion ? w28 : w12;
        if (getModel__4Soul(arena + obj) != want) bad = 1;
    }
    return bad;
}
