// Host contract for dropItem__8Skeletoni; not an EE input. Address-holding words are offsets from a host base.
// The allocator and both prop constructors are recorded stubs.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>
#include "../w10-actor-m-common/access.hpp"
unsigned char* w10_actor_m_host_base;
unsigned w10_actor_m_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
extern "C" unsigned char skeletonResMap[800];
unsigned char skeletonResMap[800];
struct Call { char kind; void* self; Point3 pos; long angle; void* model; void* anim; char *a, *b, *c; unsigned long long bits; long flag; void* desc; float scale; };
static std::vector<Call> calls; static int nalloc; static int fail_every; static unsigned char pool[8][600];
extern "C" void* blockAlloc__Fi(int size) { ++nalloc; if (size != 584) return reinterpret_cast<void*>(1); return (fail_every && nalloc % fail_every == 0) ? nullptr : pool[nalloc % 8]; }
#include "drop_item.cpp"
extern "C" void PhPropWithModel(void* self, Point3 position, short angle, void* model, void* animation, char* a, char* b, char* c, unsigned long long bits, int flag, void* description, float scale) {
    calls.push_back(Call{'A', self, position, angle, model, animation, a, b, c, bits, flag, description, scale});
}
extern "C" void PhPropWithNames(void* self, Point3 position, char* a, char* b, char* c, long bits, int flag, void* description, float scale) {
    calls.push_back(Call{'B', self, position, 0, nullptr, nullptr, a, b, c, static_cast<unsigned long long>(bits), flag, description, scale});
}
static unsigned long long expect_list(int kind, std::vector<unsigned long long>& out) {
    out.clear();
    switch (kind) {
    case 1: out = {0xfe00ull << 40, 192}; break;
    case 2: out = {0x80000, 0x100000, 0x200000, 0x10000010ull << 32}; break;
    case 3: out = {0x10000, 0x20000, 0x40000, 0x20000020ull << 32}; break;
    case 4: out = {2, 16384, 0x8000}; break;
    case 5: out = {1, 4096, 8192}; break;
    case 8: out = {192, 824, 3076, 0x10000, 0x20000, 0x80000, 0x100000, 1, 2, 4096, 16384, 0x40000, 0x200000, 0x8000, 8192, 0xfe00ull << 40}; break;
    default: break;
    }
    return out.size();
}
int main() {
    int bad = 0;
    alignas(16) static unsigned char arena[0x4000];
    w10_actor_m_host_base = arena;
    const unsigned O = 0x100, T = 0x2000;
    for (int i = 0; i < 800; i += 4) { unsigned v = 0x3400 + i; std::memcpy(skeletonResMap + i, &v, 4); }
    const unsigned long long flagsets[] = {0ull, ~0ull, 0x40000ull, 0x200000ull, 0x240000ull, 0xfe00ull << 40, 0x80001ull, 0x10000010ull << 32, 192ull | 824ull | 2ull};
    for (int kind : {1, 2, 3, 4, 5, 6, 7, 8, 0, 9, -1}) for (unsigned long long flags : flagsets) for (unsigned w : {0u, 5u, 82u, 83u, 90u, 97u, 98u})
    for (unsigned j : {0u, 4u}) for (int fe : {0, 2}) {
        std::memset(arena, 0, sizeof arena);
        unsigned tb = T; std::memcpy(arena + O + 36, &tb, 4);
        for (unsigned off : {0u, 8u, 12u, 16u}) { unsigned v = 0x3000 + off; std::memcpy(arena + T + off, &v, 4); }
        float x = 1.5f, y = -2.0f, z = 7.0f; std::memcpy(arena + O + 16, &x, 4); std::memcpy(arena + O + 20, &y, 4); std::memcpy(arena + O + 24, &z, 4);
        short ang = -9; std::memcpy(arena + O + 56, &ang, 2);
        std::memcpy(arena + O + 552, &flags, 8); std::memcpy(arena + O + 976, &w, 4); std::memcpy(arena + O + 1120, &j, 4);
        for (unsigned long long& e : partlist) e = 0x7777;
        calls.clear(); nalloc = 0; fail_every = fe;
        unsigned char ref[sizeof arena]; std::memcpy(ref, arena, sizeof arena);
        dropItem__8Skeletoni(arena + O, kind);
        std::vector<unsigned long long> list; expect_list(kind, list);
        for (size_t i = 0; i < list.size(); ++i) if (partlist[i] != list[i]) bad = 1;
        if (list.empty() && partlist[0] != 0) bad = 1;
        // model the expected calls
        size_t ci = 0; int alloc_no = 0; unsigned long long bits = flags;
        for (unsigned long long m : list) {
            const unsigned long long hit = bits & m;
            if (!hit) continue;
            ++alloc_no;
            const bool ok = !(fe && alloc_no % fe == 0);
            if (ok) {
                if (ci >= calls.size()) { bad = 1; break; }
                const Call& c = calls[ci++];
                if (c.kind != 'A' || c.pos.x != x || c.pos.y != y || c.pos.z != z || c.angle != ang || c.model != arena + 0x3000 + 12 || c.anim != arena + O + 288 ||
                    c.a != reinterpret_cast<char*>(arena + 0x3000) || c.b != reinterpret_cast<char*>(arena + 0x3008) || c.c != reinterpret_cast<char*>(arena + 0x3010) ||
                    c.bits != hit || c.flag != 263 || c.desc != nullptr || c.scale != 1.0f) bad = 1;
            }
            bits &= ~m;
        }
        unsigned long long stored; std::memcpy(&stored, arena + O + 552, 8);
        if (stored != bits) bad = 1;
        unsigned w_after, j_after; std::memcpy(&w_after, arena + O + 976, 4); std::memcpy(&j_after, arena + O + 1120, 4);
        auto map_drop = [&](unsigned idx) {
            ++alloc_no;
            const bool ok = !(fe && alloc_no % fe == 0);
            if (!ok) return;
            if (ci >= calls.size()) { bad = 1; return; }
            const Call& c = calls[ci++];
            unsigned m0, m1, m2; std::memcpy(&m0, skeletonResMap, 4); std::memcpy(&m1, skeletonResMap + 8 * idx, 4); std::memcpy(&m2, skeletonResMap + 8 * idx + 8, 4);
            if (c.kind != 'B' || c.pos.x != x || c.pos.y != y || c.pos.z != z + 36.0f || c.a != reinterpret_cast<char*>(arena + m0) || c.b != reinterpret_cast<char*>(arena + m1) ||
                c.c != reinterpret_cast<char*>(arena + m2) || c.bits != 1 || c.flag != 16643 || c.desc != nullptr || c.scale != 1.0f) bad = 1;
        };
        const bool first = (w != 0 && (kind == 6 || !(bits & 0x40000ull))) || (w >= 83 && w <= 97 && !(bits & 0x200000ull));
        if (first) { map_drop(w); if (w_after != 0) bad = 1; } else if (w_after != w) bad = 1;
        const bool second = j != 0 && (kind == 7 || !(bits & 0x200000ull));
        if (second) { map_drop(j); if (j_after != 0) bad = 1; } else if (j_after != j) bad = 1;
        if (ci != calls.size()) bad = 1;
        if (nalloc != alloc_no) bad = 1;
    }
    return bad;
}
