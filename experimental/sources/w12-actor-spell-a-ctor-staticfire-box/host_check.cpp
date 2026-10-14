// Host contract for the StaticFire box constructor; not an EE input. Address-holding words are offsets from a host base.
// Stubs record the call order and arguments; the box search fills the list from a scripted array.
#define W10_ACTOR_M_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "ctor.cpp"
unsigned char* w10_actor_m_host_base;
unsigned char vt_StaticFire[8];
alignas(16) static unsigned char arena[0x4000];
static unsigned char* const type_a = arena + 0x3000;
static unsigned char* const static_fire_type = arena + 0x3010;
static std::string order;
static std::vector<unsigned> entries;
static const unsigned FN_A = 0xaaaa0001u, FN_B = 0xaaaa0002u;
unsigned w10_actor_m_host_call(unsigned fn, void* a0, int, unsigned long, unsigned long) {
    (void)a0;
    if (fn == FN_A) return w10m::to_word(type_a);
    if (fn == FN_B) return w10m::to_word(static_fire_type);
    order += "BADFN;";
    return 0;
}
static std::string num(float f) { return std::to_string(f); }
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    order += "go(" + std::to_string(self == arena + 0x100) + "," + num(p.x) + "," + num(p.y) + "," + num(p.z) + "," + std::to_string(map == nullptr) + "," + std::to_string(names == nullptr) + ");";
    std::memset(self, 0x21, 0x100);
    const unsigned flags = 0x100;
    std::memcpy(static_cast<unsigned char*>(self) + 12, &flags, 4);
    const float pos[3] = {10.0f, -4.5f, 7.25f};
    std::memcpy(static_cast<unsigned char*>(self) + 16, pos, 12);
    const float f104 = 1.5f;
    std::memcpy(static_cast<unsigned char*>(self) + 104, &f104, 4);
}
extern "C" void init__10StaticFire(void* self) { order += "init(" + std::to_string(self == arena + 0x100) + ");"; }
extern "C" void lightAdd__FP5Light(void* l) {
    order += "ladd(" + std::to_string(static_cast<unsigned char*>(l) - (arena + 0x100)) + ");";
}
extern "C" void lightDeactivate__FP5Light(void* l) { order += "ldeact(" + std::to_string(static_cast<unsigned char*>(l) - arena) + ");"; }
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 lo, Point3 hi, unsigned* out, int cap) {
    order += "box(" + num(lo.x) + "," + num(lo.y) + "," + num(lo.z) + "," + num(hi.x) + "," + num(hi.y) + "," + num(hi.z) + "," + std::to_string(cap) + ");";
    for (size_t i = 0; i < entries.size(); ++i) out[i] = entries[i];
    out[entries.size()] = 0;
}
extern "C" void* __tf10StaticFire() { order += "tf;"; return static_fire_type; }
extern "C" int __eq__C9type_infoRC9type_info(const void* a, const void* b) { order += "eq;"; return a == b; }
extern "C" __attribute__((noreturn)) void __throw_bad_typeid() { throw 7; }

int main() {
    w10_actor_m_host_base = arena;
    unsigned char* self = arena + 0x100;
    int bad = 0;
    // other objects at 0x400 + 0x100*j; even ones have the other type, odd ones the StaticFire type
    struct Case { std::vector<int> list; int n; };  // list entries: -1 = self, j >= 0 = other object
    const Case cases[] = {{{}, 5}, {{-1}, 5}, {{0}, 5}, {{0, -1, 1}, 5}, {{0, 1, 2, 3}, -7}, {{0, 1, 2, 3}, 9}, {{-1, 0, 1, 2, 3, 4}, 7}, {{0, 1, 2}, 7}, {{0, 1, 2, 3}, -8}, {{-1, -1, -1, -1, -1}, 8}};
    for (const Case& k : cases) {
        std::memset(arena, 0x5a, sizeof arena);
        for (int j = 0; j < 8; ++j) {
            unsigned char* ob = arena + 0x400 + 0x100 * j;
            const unsigned table = 0x2000 + 16 * j;
            std::memcpy(ob + 72, &table, 4);
            const unsigned fn = (j & 1) ? FN_B : FN_A;
            std::memcpy(arena + table + 4, &fn, 4);
        }
        entries.clear();
        for (int e : k.list) entries.push_back(e < 0 ? w10m::to_word(self) : w10m::to_word(arena + 0x400 + 0x100 * e));
        order.clear();
        __10StaticFireG6Point3fif(self, Point3{3.0f, 4.0f, 5.0f}, 2.5f, k.n, 4.0f);
        std::string want = "go(1,3.000000,4.000000,5.000000,1,1);ladd(128);init(1);box(-20.000000,-34.500000,-22.750000,40.000000,25.500000,37.250000,128);";
        int count = 0;
        for (int e : k.list) {
            if (e < 0) continue;
            want += "tf;eq;";
            if (e & 1) want += "ldeact(" + std::to_string(0x400 + 0x100 * e + 128) + ");";
            ++count;
        }
        if (order != want) { std::fprintf(stderr, "calls %s\nwant  %s\n", order.c_str(), want.c_str()); bad = 1; }
        int expect_n = k.n;
        if (count >= 4) expect_n = k.n / 2;
        float f80, f104;
        std::memcpy(&f80, self + 80, 4); std::memcpy(&f104, self + 104, 4);
        unsigned w124, vt, w12; std::memcpy(&w124, self + 124, 4); std::memcpy(&vt, self + 72, 4); std::memcpy(&w12, self + 12, 4);
        unsigned short h138, h140, h142, h144, h146, h148, h150, h152;
        std::memcpy(&h138, self + 138, 2); std::memcpy(&h140, self + 140, 2); std::memcpy(&h142, self + 142, 2); std::memcpy(&h144, self + 144, 2);
        std::memcpy(&h146, self + 146, 2); std::memcpy(&h148, self + 148, 2); std::memcpy(&h150, self + 150, 2); std::memcpy(&h152, self + 152, 2);
        float l156, l164, l172, glow[3]; unsigned l160, l168;
        std::memcpy(&l156, self + 156, 4); std::memcpy(&l164, self + 164, 4); std::memcpy(&l172, self + 172, 4); std::memcpy(&l160, self + 160, 4); std::memcpy(&l168, self + 168, 4);
        std::memcpy(glow, self + 176, 12);
        const bool ok = static_cast<int>(w124) == expect_n && f80 == 2.5f && f104 == 6.0f && vt == w10m::to_word(vt_StaticFire) && w12 == 0x100u &&
                        self[110] == 1 && self[111] == 0 && self[112] == 0 && self[136] == 0 && self[137] == 0 && h138 == 255 && h146 == 255 && h140 == 128 && h148 == 128 &&
                        h142 == 0 && h144 == 0 && h150 == 0 && h152 == 0 && l156 == 120.0f && l164 == 120.0f && l160 == 0x3891a2b3u && l168 == 0x3891a2b3u && l172 == 1.0f &&
                        glow[0] == 3.0f && glow[1] == 4.0f && glow[2] == 41.0f;
        if (!ok) { std::fprintf(stderr, "fields wrong (n %d)\n", k.n); bad = 1; }
        if (self[113] != 0x21 || self[109] != 0x21 || self[188] != 0x21) { std::fprintf(stderr, "stray store\n"); bad = 1; }
    }
    if (!bad) std::puts("staticfire box ctor ok");
    return bad;
}
