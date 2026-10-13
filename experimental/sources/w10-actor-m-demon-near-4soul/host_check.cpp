// Host contract for demonNear__4Soul; not an EE input. The box query, the slot-0 type getter and the type
// comparison are stubs. Address-holding words (the table word) are offsets from a host base.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>
#include "../w10-actor-m-common/access.hpp"
unsigned char* w10_actor_m_host_base;
static std::vector<void*> found; static Point3 box_lo, box_hi; static int box_cap, nbox, ntype, neq; static std::vector<int> eq_results; static unsigned char demon_ti[4];
static const void* eq_b; static std::vector<unsigned> eq_a_seen;
unsigned w10_actor_m_host_call(unsigned fn, void*, int, unsigned long, unsigned long) { ++ntype; return 0x500 + ntype; }
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 lo, Point3 hi, void** list, int cap) {
    ++nbox; box_lo = lo; box_hi = hi; box_cap = cap;
    for (size_t i = 0; i < found.size(); ++i) list[i] = found[i];
    list[found.size()] = nullptr;
}
extern "C" void* __tf5Demon() { return demon_ti; }
extern "C" int __eq__C9type_infoRC9type_info(const void* a, const void* b) {
    eq_b = b; eq_a_seen.push_back(static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(a))); const int r = eq_results[neq % eq_results.size()]; ++neq; return r; }
#include "demon_near.cpp"
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x4000];
    w10_actor_m_host_base = arena;
    const unsigned O = 0x100, VT = 0x2000;
    for (int n : {0, 1, 2, 3}) for (unsigned bits : {0u, 8u, 1u}) for (int mask = 0; mask < 8; ++mask) {
        std::memset(arena, 0, sizeof arena);
        Point3 pos{10.0f, -20.0f, 30.0f}; std::memcpy(arena + O + 16, &pos, 12);
        found.clear(); eq_results.clear(); eq_a_seen.clear();
        for (int i = 0; i < n; ++i) {
            const unsigned obj = 0x400 + 0x100 * i; found.push_back(arena + obj);
            unsigned vt = VT; std::memcpy(arena + obj + 72, &vt, 4);
            const unsigned w12 = (i == 1) ? bits : 0u; std::memcpy(arena + obj + 12, &w12, 4);
            eq_results.push_back((mask >> i) & 1);
        }
        if (eq_results.empty()) eq_results.push_back(0);
        nbox = ntype = neq = 0;
        const int r = demonNear__4Soul(arena + O);
        if (nbox != 1 || box_cap != 128 || box_lo.x != 10.0f - 300.0f || box_lo.y != -20.0f - 300.0f || box_lo.z != 30.0f - 300.0f ||
            box_hi.x != 10.0f + 300.0f || box_hi.y != -20.0f + 300.0f || box_hi.z != 30.0f + 300.0f) bad = 1;
        int want = 0, used = 0;
        for (int i = 0; i < n; ++i) {
            ++used;
            const bool eq = (mask >> i) & 1; const unsigned w12 = (i == 1) ? bits : 0u;
            if (eq && (w12 & 8) == 0) { want = 1; break; }
        }
        if (r != want || ntype != used || neq != used || (used > 0 && eq_b != demon_ti)) bad = 1;
        for (int i = 0; i < used; ++i) if (eq_a_seen[i] != 0x500u + 1 + i) bad = 1;
    }
    return bad;
}
